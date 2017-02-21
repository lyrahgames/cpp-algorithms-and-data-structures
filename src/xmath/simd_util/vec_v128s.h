#pragma once
// #ifndef __VEC_V128S_H__
// #define __VEC_V128S_H__

#include <xmath/vec.h>
#include <xmath/simd_util/simd_util.h>
#include <xmath/simd_util/v128s.h>


namespace xmath{
namespace simd_util{


template <uint N>
using vec_v128s = vec<v128s,N>;
using v128s_3 = vec<v128s,3u>;


template <uint N>
inline vec_v128s<N> min(const vec_v128s<N>& v1, const vec_v128s<N>& v2){
	return pack_op<N, v128s_op>(min, v1, v2);
}

inline v128s_3 aos_to_soa(const v128s_3& v){
	const __m128 tmp1 = _mm_shuffle_ps(v[0].v128, v[1].v128, 0x49);
	const __m128 tmp2 = _mm_shuffle_ps(v[1].v128, v[2].v128, 0x9e);

	return v128s_3(
		_mm_shuffle_ps(v[0].v128, tmp2, 0x8c),
		_mm_shuffle_ps(tmp1, tmp2, 0xd8),
		_mm_shuffle_ps(tmp1, v[2].v128, 0xcd)
	);
}


} // simd_util
} // xmath


// #endif // __VEC_V128S_H__