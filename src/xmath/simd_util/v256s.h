#pragma once
// #ifndef __V256S_H__
// #define __V256S_H__

#include <immintrin.h>

#include <xmath/xmath_util.h>


namespace xmath{
namespace simd_util{


struct v256s{
	// default
	v256s() = default;
	v256s(const v256s&) = default;
	v256s& operator=(const v256s&) = default;
	v256s(v256s&&) = default;
	v256s& operator=(v256s&&) = default;

	// v256
	v256s(const __m256& vec_256): v256(vec_256){}
	v256s& operator=(const __m256& vec_256){v256 = vec_256;}

	// v[8]
	v256s(const float* vec_8): v256(_mm256_loadu_ps(vec_8)){}
	v256s& operator=(const float* vec_8){v256 = _mm256_loadu_ps(vec_8);}

	// elements
	v256s(float vec_0, float vec_1 = 0.0f, float vec_2 = 0.0f, float vec_3 = 0.0f, float vec_4 = 0.0f, float vec_5 = 0.0f, float vec_6 = 0.0f, float vec_7 = 0.0f):
		v0(vec_0), v1(vec_1), v2(vec_2), v3(vec_3), v4(vec_4), v5(vec_5), v6(vec_6), v7(vec_7){}


	// v[8] access
	float& operator[](uint idx){return v[idx];}
	const float& operator[](uint idx) const{return v[idx];}


	// data
	union{
		__m256 v256;
		float v[8];
		struct{float v0, v1, v2, v3, v4, v5, v6, v7;};
	};
};


inline v256s operator+(const v256s& v1, const v256s& v2){
	return _mm256_add_ps(v1.v256, v2.v256);
}

inline v256s operator-(const v256s& v1, const v256s& v2){
	return _mm256_sub_ps(v1.v256, v2.v256);
}

inline v256s operator*(const v256s& v1, const v256s& v2){
	return _mm256_mul_ps(v1.v256, v2.v256);
}

inline v256s operator/(const v256s& v1, const v256s& v2){
	return _mm256_div_ps(v1.v256, v2.v256);
}

inline v256s min(const v256s& v1, const v256s& v2){
	return _mm256_min_ps(v1.v256, v2.v256);
}


using v256s_op = v256s (*)(const v256s&, const v256s&);


} // simd_util
} // xmath


// #endif // __V256S_H__
