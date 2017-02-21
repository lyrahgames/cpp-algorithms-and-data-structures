#pragma once
#ifndef __VEC_V256D_H__
#define __VEC_V256D_H__

#include <xmath/vec.h>
#include <xmath/simd_util/v256d.h>


namespace xmath{
namespace simd_util{


template <uint N>
using vec_v256d = vec<v256d, N>;
using v256d_3 = vec<v256d, 3>;


template <uint N>
inline vec_v256d<N> min(const vec_v256d<N>& v1, const vec_v256d<N>& v2){
	return pack_op<N, v256d_op>(min, v1, v2);
}


} // simd_util
} // xmath


#endif // __VEC_V256D_H__