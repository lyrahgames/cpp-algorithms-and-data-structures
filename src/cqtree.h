#pragma once

struct cqtree{
	// struct node{
	// 	float sample;
	// };
	typedef float node;

	struct leaf{
		uint count;
		float u;
		float v;
	};

	uint max_depth;
	uint cur_depth;
	uint leaf_count;
	uint node_count;
	node* ndata;
	leaf* ldata;

	float operator()(float u, float v){
		return get(0, 0, u, v);
	}

	float get(uint d, uint i, float u, float v){
		const uint count = 1 << (2*d);
		const uint offset = (count - 1) / 3;

		if (d >= max_depth){
			return ndata[offset + i];
		}else{
			if (u+v < 0.5f){
				return get(d+1, 4*i, 2.0f*u, 2.0f*v);
			}else if (u >= 0.5f){
				return get(d+1, 4*i+1, 2.0f*u-1.0f, 2.0f*v);
			}else if (v >= 0.5f){
				return get(d+1, 4*i+3, 2.0f*u, 2.0f*v-1.0f);
			}else{
				return get(d+1, 4*i+2, 1.0f-2.0f*u, 1.0f-2.0f*v);
			}
		}
	}

	float buffer(float u, float v){
		return get_buffer(0,0,u,v);
	}

	float get_buffer(uint d, uint i, float u, float v){
		const uint count = 1 << (2*d);
		const uint offset = (count - 1) / 3;

		if (d >= max_depth){
			return float(ldata[i].count);
		}else{
			if (u+v < 0.5f){
				return get_buffer(d+1, 4*i, 2.0f*u, 2.0f*v);
			}else if (u >= 0.5f){
				return get_buffer(d+1, 4*i+1, 2.0f*u-1.0f, 2.0f*v);
			}else if (v >= 0.5f){
				return get_buffer(d+1, 4*i+3, 2.0f*u, 2.0f*v-1.0f);
			}else{
				return get_buffer(d+1, 4*i+2, 1.0f-2.0f*u, 1.0f-2.0f*v);
			}
		}
	}
};