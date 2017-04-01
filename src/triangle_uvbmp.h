#pragma once

#include <util.h>
#include <math.h>


struct triangle_uvbmp{
	float *data;
	uint order;

	float operator()(float u, float v) const{
		const float orderf = (float)order;
		const uint p = floorf(u * orderf);
		const uint q = floorf(v * orderf);
		const uint r = floorf((u+v) * orderf);

		return data[r*(r+1) + q - p];
	}
};

