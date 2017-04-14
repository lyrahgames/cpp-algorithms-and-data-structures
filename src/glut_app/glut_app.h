#pragma once
// #ifndef __GLUT_APP_H__
// #define __GLUT_APP_H__

#include <string.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
#include <GL/glew.h>
// #include <glew.h>
#include <GL/glut.h>

#include <scene.h>
#include <cqtree.h>


namespace glut_app{


unsigned char* pixel_buffer;
int width;
int height;
scene_t scene;
scene_t::primitive_t* ptmp;


void init(cqtree* tree, uint depth){
	tree->max_depth = depth;

	const uint leaf_count = 1 << (2*depth);
	tree->leaf_count = leaf_count;
	tree->ldata = new cqtree::leaf[leaf_count];

	const uint node_count = ((1 << (2*depth + 2)) - 1) / 3;
	tree->node_count = node_count;
	tree->ndata = new cqtree::node[node_count];

	printf("leaf count = %u\tnode count = %u\n", leaf_count, node_count);
}

void gen_uv_offset(cqtree* tree, uint depth = 0, uint idx = 0, float u = 0.0f, float v = 0.0f){
	if (depth >= tree->max_depth){
		tree->ldata[idx].u = u;
		tree->ldata[idx].v = v;
	}else{
		const uint step = 1 << (depth+1);
		const float inv_step = 1.0f / float(step);

		const float u2 = u + inv_step;
		const float v2 = v + inv_step;

		gen_uv_offset(tree, depth+1, 4*idx, u, v);
		gen_uv_offset(tree, depth+1, 4*idx+1, u2, v);
		gen_uv_offset(tree, depth+1, 4*idx+2, -u2, -v2);
		gen_uv_offset(tree, depth+1, 4*idx+3, u, v2);
	}
}

void gen_samples(cqtree* tree, const scene_t& scene, const scene_t::primitive_t& prim){
	const uint offset = ((1 << (2*tree->max_depth)) - 1) /3;
	const float step = 1.0f / float(1<<tree->max_depth);

	for (uint l = 0; l < tree->leaf_count; l++){
		float urnd, vrnd;
		triangle_rand(urnd, vrnd);
		float u = fabsf(tree->ldata[l].u + step * urnd);
		float v = fabsf(tree->ldata[l].v + step * vrnd);

		tree->ndata[offset + l] = map(&scene, &prim, u, v);
		tree->ldata[l].count = 1;
	}

	for (int d = tree->max_depth-1; d >= 0; d--){
		const uint count = 1 << (2*d);
		const uint offset = (count - 1) / 3;

		for (uint idx = 0; idx < count; idx++){
			const uint child = offset + count + 4*idx;
			tree->ndata[offset + idx] = 0.25f * (
				tree->ndata[child] +
				tree->ndata[child+1] +
				tree->ndata[child+2] +
				tree->ndata[child+3]
			);
		}
	}
}

void accum_samples(cqtree* tree, const scene_t& scene, const scene_t::primitive_t& prim, uint d = 0, uint i = 0){

	const uint offset = ((1 << (2*tree->max_depth)) - 1) /3;
	const float step = 1.0f / float(1<<tree->max_depth);

	const uint depth = tree->max_depth - d;
	const uint node_count = 1 << (2*depth);

	const uint l0 = node_count * i;
	const uint l1 = l0 + node_count;

	for (uint l = l0; l < l1; l++){

		const uint count = tree->ldata[l].count;

		float accum_buffer = 0.0f;
		for (uint sid = 0; sid < 3*count; sid++){
			float urnd, vrnd;
			triangle_rand(urnd, vrnd);
			float u = fabsf(tree->ldata[l].u + step * urnd);
			float v = fabsf(tree->ldata[l].v + step * vrnd);
			accum_buffer += map(&scene, &prim, u, v);
		}
		accum_buffer /= float(4*count);


		tree->ndata[offset + l] = 0.25f * tree->ndata[offset+l] + accum_buffer;
		tree->ldata[l].count = 4*count;
	}

	for (int did = tree->max_depth-1; did >= (int)d; did--){
		const uint count = 1 << (2*did);
		const uint offset = (count - 1) / 3;

		const uint depth = did - d;
		const uint node_count = 1 << (2*depth);

		const uint l0 = node_count * i;
		const uint l1 = l0 + node_count;

		for (uint idx = l0; idx < l1; idx++){
			const uint child = offset + count + 4*idx;
			tree->ndata[offset + idx] = 0.25f * (
				tree->ndata[child] +
				tree->ndata[child+1] +
				tree->ndata[child+2] +
				tree->ndata[child+3]
			);
		}
	}
}

void compress_and_accum_cqtree(cqtree* tree, uint d, uint idx, int node_id, uint step){

	const uint max_step = tree->max_depth;
	const float err_eps = 0.05f;
	const uint count = 1 << (2*d);
	const uint offset = (count - 1) / 3;


	if (step >= max_step){
		accum_samples(tree, scene, *ptmp, d, idx);
		ptmp->qtmap.qtree[node_id].child = ~(ptmp->qtmap.sample.size());
		ptmp->qtmap.sample.push_back(tree->ndata[offset + idx]);
		return;
	}

	const float sample = tree->ndata[offset + idx];
	const float inv_sample = 1.0f / (sample + 0.05);
	const uint child = offset + count + 4*idx;

	float err0 = fabsf(tree->ndata[child] - sample) * inv_sample;
	float err1 = fabsf(tree->ndata[child+1] - sample) * inv_sample;
	float err2 = fabsf(tree->ndata[child+2] - sample) * inv_sample;
	float err3 = fabsf(tree->ndata[child+3] - sample) * inv_sample;

	const bool finish = (err0 <= err_eps) && (err1 <= err_eps) && (err2 <= err_eps) && (err3 <= err_eps);

	if (finish){
		ptmp->qtmap.qtree[node_id].child = ~(ptmp->qtmap.sample.size());
		ptmp->qtmap.sample.push_back(sample);
		return;
	}

	accum_samples(tree, scene, *ptmp, d, idx);

	ptmp->qtmap.qtree[node_id].child = ptmp->qtmap.qtree.size();
	ptmp->qtmap.qtree.push_back(node{});
	ptmp->qtmap.qtree.push_back(node{});
	ptmp->qtmap.qtree.push_back(node{});
	ptmp->qtmap.qtree.push_back(node{});

	compress_and_accum_cqtree(tree, d+1, 4*idx, ptmp->qtmap.qtree[node_id].child, step+1);

	compress_and_accum_cqtree(tree, d+1, 4*idx+1, ptmp->qtmap.qtree[node_id].child+1, step+1);

	compress_and_accum_cqtree(tree, d+1, 4*idx+2, ptmp->qtmap.qtree[node_id].child+2, step+1);

	compress_and_accum_cqtree(tree, d+1, 4*idx+3, ptmp->qtmap.qtree[node_id].child+3, step+1);
}

inline void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const uint sample_count = 4;
	const float inv_sample_count = 1.0f / float(sample_count);

	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){

			const int idx = j*width + i;

			float accum_buffer = 0.0f;
			float caccum_buffer = 0.0f;
			for (uint s = 0; s < sample_count; s++){
				float u,v;
				quad_rand(u, v);
				const vecf2 pos(
					(float(i)+u)*3.0f/float(width),
					(float(j)+v)*3.0f/float(height)
				);

				unsigned char tmp;
				const scene_t::primitive_t* p;
				uint pid;
				vecf2 uv;

				if (traversal(&scene, pos, p, uv, pid)){
					// tmp = 255.0f * map(&scene,&(scene.p_data[pid]),uv.x,uv.y);
					// tmp = 255.0f * map(&scene,p,uv.x,uv.y);
					// tmp = 255.0f * p->uvbmp(uv.x, uv.y);
					// pixel_buffer[3*idx] += 255.0f * uv.x;
					// pixel_buffer[3*idx+1] += 255.0f * uv.y;
					// pixel_buffer[3*idx+2] += 255.0f * sum(uv);
					// memset(pixel_buffer+3*idx, tmp, 3*sizeof(unsigned char));
					// accum_buffer += scene.p_data[pid].uvbmp(uv.x, uv.y);
					// accum_buffer += scene.p_data[pid].uvmap(uv.x, uv.y);
					// accum_buffer += scene.p_data[pid].uvmap.nn(uv.x, uv.y);
					accum_buffer += scene.p_data[pid].qtmap(uv.x, uv.y);
					// accum_buffer += scene.p_data[pid].treemap(uv.x, uv.y);
					// caccum_buffer += scene.p_data[pid].qtmap.step / 9.0f;
					caccum_buffer += log2f(scene.p_data[pid].treemap.buffer(uv.x,uv.y)) / 12.0f;
				}
			}

			accum_buffer *= inv_sample_count;
			caccum_buffer *= inv_sample_count;

			memset(pixel_buffer+3*idx, (unsigned char)(255.0f*accum_buffer), 3*sizeof(unsigned char));
			// memset(pixel_buffer+3*idx, (unsigned char)(255.0f*(caccum_buffer)), 3*sizeof(unsigned char));
		}
	}

	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer);

	// glBegin(GL_TRIANGLES);
	// 	glVertex3f(-2,-2,-5.0);
	// 	glVertex3f(2,0.0,-5.0);
	// 	glVertex3f(0.0,2,-5.0);
	// glEnd();

	glutSwapBuffers();
}

inline void resize(int w, int h){
	glViewport(0, 0, w, h);
	width = w;
	height = h;

	delete[] pixel_buffer;
	pixel_buffer = new unsigned char[w*h*3];

	const float ar = float(w) / float(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,ar,1,100);
	glMatrixMode(GL_MODELVIEW);
}

void gen(float, int, uint, float, float, bool);

inline void init(int argc, char** argv){
	width = 1000;
	height = 500;
	pixel_buffer = new unsigned char[width*height*3];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(width, height);
	glutCreateWindow("opengl_drawer");

	glClearColor(0.1, 0.1, 0.1, 1);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);


	GLenum err = glewInit();
	if(GLEW_OK != err){
	  printf("Error: %s\n",glewGetErrorString(err));
	}

	printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//check
	bool check_pbo = glewGetExtension("GL_ARB_pixel_buffer_object");
	printf("OGL: support for PBO [%c]\n", check_pbo ? 'X' : ' ');

	bool check_tnp2 = glewGetExtension("GL_ARB_texture_non_power_of_two");
	printf("OGL: texture_non_power_of_two [%c]\n", check_tnp2 ? 'X' : ' ');


	// scene description
	scene.v_data.push_back(vecf2(0.5f, 2.0f));
	scene.v_data.push_back(vecf2(1.0f, 0.6f));
	scene.v_data.push_back(vecf2(0.8f, 2.0f));
	scene.v_data.push_back(vecf2(2.0f, 0.8f));
	scene.v_data.push_back(vecf2(2.0f, 2.0f));

	scene.p_data.push_back(scene_t::primitive_t{1,0,2});
	scene.p_data.push_back(scene_t::primitive_t{1,3,2});
	scene.p_data.push_back(scene_t::primitive_t{2,3,4});


	// generate uvbmp for primitives
	const uint exp = 6;
	const uint order = 1 << exp;
	const float inv_order = 1.0f / (float)order;
	const uint sample_count = 16;
	const float inv_sample_count = 1.0f / (float)sample_count;

	for (uint i = 0; i < scene.p_data.size(); i++){
		scene_t::primitive_t* p = &scene.p_data[i];

		p->uvbmp.order = order;
		p->uvbmp.data = new float[order*order];

		uint r = 0;

		for (uint i = 0; i < order; i++){
			uint uidx = i;
			uint vidx = 0;

			for (uint j = 0; j < i; j++){
				p->uvbmp.data[r] = 0;
				for (uint s = 0; s < sample_count; s++){
					float u,v;
					triangle_rand(u,v);
					u = (float(uidx) + u) * inv_order;
					v = (float(vidx) + v) * inv_order;
					p->uvbmp.data[r] += map(&scene, p, u, v);
				}
				p->uvbmp.data[r] *= inv_sample_count;

				vidx++;
				r++;

				p->uvbmp.data[r] = 0;
				for (uint s = 0; s < sample_count; s++){
					float u,v;
					triangle_rand(u,v);
					u = (float(uidx) - u) * inv_order;
					v = (float(vidx) - v) * inv_order;
					p->uvbmp.data[r] += map(&scene, p, u, v);
				}
				p->uvbmp.data[r] *= inv_sample_count;

				uidx--;
				r++;
			}

			p->uvbmp.data[r] = 0;
			for (uint s = 0; s < sample_count; s++){
				float u,v;
				triangle_rand(u,v);
				u = (float(uidx) + u) * inv_order;
				v = (float(vidx) + v) * inv_order;
				p->uvbmp.data[r] += map(&scene, p, u, v);
			}
			p->uvbmp.data[r] *= inv_sample_count;

			r++;
		}
	}
	// end of uvmap generation




	const uint map_order = 1 << 6;
	const float inv_map_order = 1.0f / float(map_order);
	const uint map_scount = 2;
	const float c1o3 = 1.0f/3.0f;
	const float c2o3 = 2.0f/3.0f;

	for (uint pid = 0; pid < scene.p_data.size(); pid++){
		scene_t::primitive_t* p = &scene.p_data[pid];

		p->uvmap.order = map_order;
		p->uvmap.data = new float[size(map_order)];
		uint idx = 0;
		for (uint r = 0; r <= p->uvmap.order; r++){
			for (uint j = 0; j <= r; j++){
				uint i = r - j;
				float u = inv_map_order * float(i);
				float v = inv_map_order * float(j);

				float ab = 0.0f;
				for (uint sid = 0; sid < map_scount; sid++){
					float urnd, vrnd;


					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (c1o3 * urnd - c1o3 * vrnd), v + inv_map_order * (c1o3 * urnd + c2o3 * vrnd));

					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (-c1o3 * urnd - c2o3 * vrnd), v + inv_map_order * (c2o3 * urnd + c1o3 * vrnd));

					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (-c2o3 * urnd - c1o3 * vrnd), v + inv_map_order * (c1o3 * urnd - c1o3 * vrnd));

					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (-c1o3 * urnd + c1o3 * vrnd), v + inv_map_order * (-c1o3 * urnd - c2o3 * vrnd));

					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (c1o3 * urnd + c2o3 * vrnd), v + inv_map_order * (-c2o3 * urnd - c1o3 * vrnd));

					triangle_rand(urnd, vrnd);
					ab += map(&scene, p, u + inv_map_order * (c2o3 * urnd + c1o3 * vrnd), v + inv_map_order * (-c1o3 * urnd + c1o3 * vrnd));
				}

				ab /= float(6*map_scount);
				p->uvmap.data[idx] = ab;
				// p->uvmap.data[idx] = map(&scene, p, u, v);
				idx++;
			}
		}
	}

	// sample tree
	for (uint pid = 0; pid < scene.p_data.size(); pid++){
		// cqtree tree;
		scene_t::primitive_t* p = &scene.p_data[pid];
		init(&p->treemap, 5);
		p->treemap.cur_depth = 5;
		gen_uv_offset(&p->treemap);
		gen_samples(&p->treemap, scene, *p);
		// accum_samples(&p->treemap, scene, *p);
		// accum_samples(&p->treemap, scene, *p, 1, 2);
		// accum_samples(&p->treemap, scene, *p, 2, 9);

	}



	// qtree map generation
	for (uint i = 0; i < scene.p_data.size(); i++){
		scene_t::primitive_t* p = &scene.p_data[i];

		ptmp = p;
		// float sample = map(&scene, p, 0.333333f, 0.333333f);
		p->qtmap.qtree.push_back(node{});
		// gen(sample, 0, 0, 0.0f, 0.0f, false);
		compress_and_accum_cqtree(&p->treemap, 0, 0, 0, 0);
	}

	// for (uint i = 0; i < ptmp->qtmap.qtree.size(); i++){
	// 	printf("%i\t", ptmp->qtmap.qtree[i].child);
	// }
	// printf("\n");
	printf("size qtree = %i\n", ptmp->qtmap.qtree.size());
}

inline void gen(float sample, int node_id, uint step, float u, float v, bool turn){

	const uint max_step = 8;
	const uint min_step = max_step >> 1;
	const float err_eps = 0.05f;
	const uint rs_count = 16;


	if (step >= max_step){
		ptmp->qtmap.qtree[node_id].child = ~(ptmp->qtmap.sample.size());
		ptmp->qtmap.sample.push_back(sample);
		// printf("sample = %f\n", sample);
		// printf("child = %i\n", ptmp->qtmap.qtree[node_id].child);
	}else{
		float s[3];

		const uint order = 1 << step;
		const float inv_order = 1.0f / float(order);
		const float tmp1 = 0.166666f * inv_order;
		const float tmp2 = 0.666666f * inv_order;

		float factor = 1.0f;

		if (turn){
			factor = -1.0f;
		}

		s[0] = map(&scene, ptmp, u + factor * tmp1, v + factor * tmp1);
		s[1] = map(&scene, ptmp, u + factor * tmp2, v + factor * tmp1);
		s[2] = map(&scene, ptmp, u + factor * tmp1, v + factor * tmp2);

		// float err = sqrtf(sq(s[0]-sample) + sq(s[1]-sample) + sq(s[2]-sample));
		float err = max(fabsf(s[0]-sample), max(fabsf(s[1]-sample), fabsf(s[2]-sample))) / (sample + 0.01f);

		if ((err <= err_eps) && (step >= min_step)){

			// generate random samples
			float rs[rs_count];
			float rerr = 0.0f;
			for (uint sid = 0; sid < rs_count; sid++){
				float urnd, vrnd;
				triangle_rand(urnd, vrnd);

				rs[sid] = map(&scene, ptmp, u + factor * inv_order * urnd, v + factor * inv_order * vrnd);
				rerr = max(rerr, fabsf(rs[sid] - sample));
			}
			rerr /= (sample + 0.01f);

			if (rerr <= err_eps){
				float tmp = s[0] + s[1] + s[2] + sample;
				for (uint sid = 0; sid < rs_count; sid++){
					tmp += rs[sid];
				}
				tmp *= 1.0f/(4.0f + float(rs_count));

				ptmp->qtmap.qtree[node_id].child = ~(ptmp->qtmap.sample.size());
				// ptmp->qtmap.sample.push_back(0.25f * (s[0] + s[1] + s[2] + sample));
				ptmp->qtmap.sample.push_back(tmp);
				return;
			}
		}

		ptmp->qtmap.qtree[node_id].child = ptmp->qtmap.qtree.size();
		ptmp->qtmap.qtree.push_back(node{});
		ptmp->qtmap.qtree.push_back(node{});
		ptmp->qtmap.qtree.push_back(node{});
		ptmp->qtmap.qtree.push_back(node{});
		gen(s[0], ptmp->qtmap.qtree[node_id].child, step+1, u, v, turn);
		gen(s[1], ptmp->qtmap.qtree[node_id].child+1, step+1, u + factor * 0.5f*inv_order, v, turn);
		gen(sample, ptmp->qtmap.qtree[node_id].child+2, step+1, u + factor * 0.5f * inv_order, v + factor * 0.5f * inv_order, !turn);
		gen(s[2], ptmp->qtmap.qtree[node_id].child+3, step+1, u, v + factor * 0.5f*inv_order, turn);

	}
}


inline int exec(){
	glutMainLoop();
	return 0;
}


} // glut_app


// #endif // __GLUT_APP_H__