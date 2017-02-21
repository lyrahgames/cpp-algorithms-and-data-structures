#pragma once
// #ifndef __SIMD_UTIL_H__
// #define __SIMD_UTIL_H__


namespace xmath{
namespace simd_util{


inline __m256d dot3(const __m256d& a_x, const __m256d& a_y, const __m256d& a_z, const __m256d& b_x, const __m256d& b_y, const __m256d& b_z){
	return _mm256_add_pd(_mm256_add_pd(_mm256_mul_pd(a_x, b_x), _mm256_mul_pd(a_y, b_y)), _mm256_mul_pd(a_z, b_z));
}

inline __m256d dot3(const __m256d* a, const __m256d* b){
	return dot3(a[0], a[1], a[2], b[0], b[1], b[2]);
}


inline __m256d sqlen3(const __m256d& v_x, const __m256d& v_y, const __m256d& v_z){
	return dot3(a_x, a_y, a_z, a_x, a_y, a_z);
}

inline __m256d sqlen3(const __m256d* v){
	return dot3(v, v);
}


} // simd_util
} // xmath


// #endif // __SIMD_UTIL_H__