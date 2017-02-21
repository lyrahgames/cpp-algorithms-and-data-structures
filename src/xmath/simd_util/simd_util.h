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
	return dot3(v_x, v_y, v_z, v_x, v_y, v_z);
}

inline __m256d sqlen3(const __m256d* v){
	return dot3(v, v);
}

inline void aos_to_soa_3(__m128& v1, __m128& v2, __m128& v3){
	const __m128 tmp1 = _mm_shuffle_ps(v1, v2, 0x49);
	const __m128 tmp2 = _mm_shuffle_ps(v2, v3, 0x9e);

	v1 = _mm_shuffle_ps(v1, tmp2, 0x8c);
	v2 = _mm_shuffle_ps(tmp1, tmp2, 0xd8);
	v3 = _mm_shuffle_ps(tmp1, v3, 0xcd);
}

inline void aos_to_soa_3(__m256d& v1, __m256d& v2, __m256d& v3){
	const __m256d tmp1 = _mm256_permute2f128_pd(v1, v2, 0x30);
	const __m256d tmp2 = _mm256_permute2f128_pd(v1, v3, 0x21);
	const __m256d tmp3 = _mm256_permute2f128_pd(v2, v3, 0x30);

	v1 = _mm256_shuffle_pd(tmp1, tmp2, 0x0a);
	v2 = _mm256_shuffle_pd(tmp1, tmp3, 0x05);
	v3 = _mm256_shuffle_pd(tmp2, tmp3, 0x0a);
}


} // simd_util
} // xmath


// #endif // __SIMD_UTIL_H__