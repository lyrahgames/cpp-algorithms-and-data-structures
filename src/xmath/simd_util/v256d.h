#pragma once
// #ifndef __V256D_H__
// #define __V256D_H__

#include <immintrin.h>

#include <xmath/xmath_util.h>


namespace xmath{
namespace simd_util{


struct v256d{
	// default
	v256d() = default;
	v256d(const v256d&) = default;
	v256d& operator=(const v256d&) = default;
	v256d(v256d&&) = default;
	v256d& operator=(v256d&&) = default;

	// v256
	v256d(const __m256d& vec_256): v256(vec_256){}
	v256d& operator=(const __m256d& vec_256){v256 = vec_256;}

	// v[4]
	v256d(const double* vec_4): x(vec_4[0]), y(vec_4[1]), z(vec_4[2]), w(vec_4[3]){}
	v256d& operator=(const double* vec_4){x = vec_4[0];y = vec_4[1];z = vec_4[2];w = vec_4[3];}

	// x,y,z,w
	v256d(double vec_x, double vec_y, double vec_z, double vec_w = 0.0): x(vec_x), y(vec_y), z(vec_z), w(vec_w){}


	// v[4] access
	double& operator[](uint idx){return v[idx];}
	const double& operator[](uint idx) const{return v[idx];}


	// data
	union{
		__m256d v256;
		double v[4];
		struct{double x, y, z, w;};
	};
};


inline v256d operator+(const v256d& v1, const v256d& v2){
	return _mm256_add_pd(v1.v256, v2.v256);
}

inline v256d operator-(const v256d& v1, const v256d& v2){
	return _mm256_sub_pd(v1.v256, v2.v256);
}

inline v256d operator*(const v256d& v1, const v256d& v2){
	return _mm256_mul_pd(v1.v256, v2.v256);
}

inline v256d operator/(const v256d& v1, const v256d& v2){
	return _mm256_div_pd(v1.v256, v2.v256);
}

inline v256d min(const v256d& v1, const v256d& v2){
	return _mm256_min_pd(v1.v256, v2.v256);
}


using v256d_op = v256d (*)(const v256d&, const v256d&);


} // simd_util
} // xmath


// #endif // __V256D_H__
