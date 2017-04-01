#pragma once
// #ifndef __SCENE_H__
// #define __SCENE_H__

#include <vector>
#include <xmath/vec.h>
#include <util.h>
#include <triangle_uvbmp.h>
#include <triangle_qtree_uvbmp.h>
#include <triangle_uvmap.h>


struct scene_t{
	using vertex_t = vecf2;

	struct primitive_t{
		uint v_id[3];
		triangle_uvbmp uvbmp;
		triangle_qtree_uvbmp qtmap;
		triangle_uvmap uvmap;
	};

	std::vector<vertex_t> v_data;
	std::vector<primitive_t> p_data;
};

inline int intersect(const scene_t* scene, const scene_t::primitive_t* p, const vecf2& pos, vecf2& uv){
	const vecf2 edge1 = scene->v_data[p->v_id[1]] - scene->v_data[p->v_id[0]];
	const vecf2 edge2 = scene->v_data[p->v_id[2]] - scene->v_data[p->v_id[0]];
	const vecf2 xrel = pos - scene->v_data[p->v_id[0]];

	const float det = edge1.x * edge2.y - edge1.y * edge2.x;
	const float inv_det = 1.0f / det;

	uv[0] = inv_det * (edge2.y * xrel.x - edge2.x * xrel.y);

	if (uv[0] < 0.0f || uv[0] > 1.0f)
		return 0;

	uv[1] = inv_det * (-edge1.y * xrel.x + edge1.x * xrel.y);

	if (uv[1] < 0.0f || uv[1] + uv[0] > 1.0f)
		return 0;

	return 1;
}

inline int traversal(const scene_t* scene, const vecf2& pos, const scene_t::primitive_t* &p, vecf2& uv, uint& pid){
	uint count = 0;
	for (uint i = 0; i < scene->p_data.size(); i++){
		if (intersect(scene, &scene->p_data[i], pos, uv)){
			p = &(scene->p_data[i]);
			pid = i;
			return 1;
		}
	}

	return 0;
}

inline float map(const scene_t* scene, const scene_t::primitive_t* p, float u, float v){
	vecf2 pos =
		scene->v_data[p->v_id[0]] * (1.0f - u - v) +
		scene->v_data[p->v_id[1]] * u +
		scene->v_data[p->v_id[2]] * v;
	const float omega = 5.5f;
	return sq(sinf(omega*sq(pos.x) - 2.0f*omega*pos.y)) * sqrt(sq(cosf(2.0f*omega*sq(pos.y) + omega*pos.x*pos.y)));
	// return u+v;
	// return (u*u + v*v < 0.5f)?(0.1f):(0.9f);
}

inline void triangle_rand(float& u, float& v){
	u = float(rand()) / float(RAND_MAX);
	v = float(rand()) / float(RAND_MAX);

	if (u+v > 1.0f){
		u = 1.0f - u;
		v = 1.0f - v;
	}
}

inline void quad_rand(float& u, float& v){
	u = float(rand()) / float(RAND_MAX);
	v = float(rand()) / float(RAND_MAX);
}


// #endif // __SCENE_H__