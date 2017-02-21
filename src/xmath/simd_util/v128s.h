#pragma once
// #ifndef __V128S_H__
// #define __V128S_H__

#include <immintrin.h>

#include <xmath/xmath_util.h>


namespace xmath{
namespace simd_util{


struct v128s{
	// default
	v128s() = default;
	v128s(const v128s&) = default;
	v128s& operator=(const v128s&) = default;
	v128s(v128s&&) = default;
	v128s& operator=(v128s&&) = default;

	// v128s
	v128s(const __m128& vec_128): v128(vec_128){}
	v128s& operator=(const __m128& vec_128){v128 = vec_128;}

	// v[4]
	v128s(const float* vec_4): x(vec_4[0]), y(vec_4[1]), z(vec_4[2]), w(vec_4[3]){}
	v128s& operator=(const float* vec_4){x = vec_4[0];y = vec_4[1];z = vec_4[2];w = vec_4[3];}

	// x,y,z,w
	v128s(float vec_x, float vec_y, float vec_z, float vec_w = 0.0f): x(vec_x), y(vec_y), z(vec_z), w(vec_w){}


	// v[4] access
	float& operator[](uint idx){return v[idx];}
	const float& operator[](uint idx) const{return v[idx];}


	// data
	union{
		__m128 v128;
		float v[4];
		struct{float x, y, z, w;};
	};
};


inline v128s operator+(const v128s& v1, const v128s& v2){
	return _mm_add_ps(v1.v128, v2.v128);
}

inline v128s operator-(const v128s& v1, const v128s& v2){
	return _mm_sub_ps(v1.v128, v2.v128);
}

inline v128s operator*(const v128s& v1, const v128s& v2){
	return _mm_mul_ps(v1.v128, v2.v128);
}

inline v128s operator/(const v128s& v1, const v128s& v2){
	return _mm_div_ps(v1.v128, v2.v128);
}

inline v128s min(const v128s& v1, const v128s& v2){
	return _mm_min_ps(v1.v128, v2.v128);
}


using v128s_op = v128s (*)(const v128s&, const v128s&);


} // simd_util
} // xmath


// #endif // __V128S_H__
