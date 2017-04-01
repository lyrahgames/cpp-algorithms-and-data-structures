#pragma once

#include <util.h>
#include <math.h>

inline uint memidx(uint u, uint v){
	const uint sum = u+v;
	return ((sum*(sum+1))>>1) + v;
}

inline uint size(uint order){
	return ((order+1)*(order+2))>>1;
}

struct triangle_uvmap{
	float *data;
	uint order;

	float operator()(float u, float v) const{
		const float nu = order * u;
		const float nv = order * v;
		const float uidx = floorf(nu);
		const float vidx = floorf(nv);
		const float wu = nu - uidx;
		const float wv = nv - vidx;
		const float sum_w = wu + wv;

		float interp_sample;

		if (sum_w > 1.0f){
			const uint idx = memidx(uidx+1,vidx);
			const uint sum = uidx + vidx + 1;

			interp_sample =
				(1.0f-wu) * data[idx + 1] + // memidx(u,v+1)
				(1.0f-wv) * data[idx] + // memidx(u+1,v)
				(sum_w-1.0f) * data[idx + sum + 2]; // memidx(u+1,v+1)
		}else{
			const uint idx = memidx(uidx, vidx);
			const uint sum = uidx + vidx;

			interp_sample =
				wu * data[idx + sum + 1] + // memidx(u+1,v)
				wv * data[idx + sum + 2] + // memidx(u,v+1)
				(1.0f-sum_w) * data[idx]; // memidx(u,v)
		}

		return interp_sample;
	}

	float nn(float u, float v) const{
		const float nu = order * u;
		const float nv = order * v;
		const float uidx = roundf(nu);
		const float vidx = roundf(nv);
		return data[memidx(uidx,vidx)];
		// const float wu = nu - uidx;
		// const float wv = nv - vidx;
		// const float sum_w = wu + wv;

		// float interp_sample;

		// if (sum_w > 1.0f){
		// 	const uint idx = memidx(uidx+1,vidx);
		// 	const uint sum = uidx + vidx + 1;

		// 	interp_sample =
		// 		(1.0f-wu) * data[idx + 1] + // memidx(u,v+1)
		// 		(1.0f-wv) * data[idx] + // memidx(u+1,v)
		// 		(sum_w-1.0f) * data[idx + sum + 2]; // memidx(u+1,v+1)
		// }else{
		// 	const uint idx = memidx(uidx, vidx);
		// 	const uint sum = uidx + vidx;

		// 	interp_sample =
		// 		wu * data[idx + sum + 1] + // memidx(u+1,v)
		// 		wv * data[idx + sum + 2] + // memidx(u,v+1)
		// 		(1.0f-sum_w) * data[idx]; // memidx(u,v)
		// }

		// return interp_sample;
	}
};