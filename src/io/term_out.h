#pragma once
// #ifndef __TERM_OUT_H__
// #define __TERM_OUT_H__

#include <iostream>

#include <xmath/xmath.h>


inline std::ostream& operator<<(std::ostream& os, const xmath::simd_util::v256d& v){
	return os << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "]";
}

template <class T, uint N>
std::ostream& operator<<(std::ostream& os, const xmath::vec<T,N>& v){
	os << "(" << v[0];
	for (uint i = 1; i < N; i++)
		os << ", " << v[i];
	return os << ")";
}


// #endif // __TERM_OUT_H__