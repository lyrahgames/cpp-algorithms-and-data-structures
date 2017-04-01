#pragma once

#include <vector>


struct node{
	int child; // index in qtree of first child; other ones saved after this location; if negative then bitwise negative is sample index
};

struct triangle_qtree_uvbmp{
	std::vector<node> qtree;
	std::vector<float> sample;
	uint step;

	float operator()(float u, float v){
		step = 0;
		return get(0, u, v);
	}

	float get(int node_id ,float u, float v){
		int child = qtree[node_id].child;
		step++;

		if (child < 0){
			return sample[~child];
		}else{
			if (u+v < 0.5f){
				return get(child, 2.0f*u, 2.0f*v);
			}else if (u >= 0.5f){
				return get(child+1, 2.0f*u-1.0f, 2.0f*v);
			}else if (v >= 0.5f){
				return get(child+3, 2.0f*u, 2.0f*v-1.0f);
			}else{
				return get(child+2, 1.0f-2.0f*u, 1.0f-2.0f*v);
			}
		}
	}
};