#pragma once
// #ifndef __VEC_V256D_H__
// #define __VEC_V256D_H__

#include <xmath/vec.h>
#include <xmath/simd_util/simd_util.h>
#include <xmath/simd_util/v256d.h>


namespace xmath{
namespace simd_util{


template <uint N>
using vec_v256d = vec<v256d, N>;
using v256d_3 = vec<v256d,3u>;


template <uint N>
inline vec_v256d<N> min(const vec_v256d<N>& v1, const vec_v256d<N>& v2){
	return pack_op<N, v256d_op>(min, v1, v2);
}

inline v256d_3 aos_to_soa(const v256d_3& v){
	const __m256d tmp1 = _mm256_permute2f128_pd(v[0].v256, v[1].v256, 0x30);
	const __m256d tmp2 = _mm256_permute2f128_pd(v[0].v256, v[2].v256, 0x21);
	const __m256d tmp3 = _mm256_permute2f128_pd(v[1].v256, v[2].v256, 0x30);

	return v256d_3(
		_mm256_shuffle_pd(tmp1, tmp2, 0x0a),
		_mm256_shuffle_pd(tmp1, tmp3, 0x05),
		_mm256_shuffle_pd(tmp2, tmp3, 0x0a)
	);
}


} // simd_util
} // xmath


// #endif // __VEC_V256D_H__