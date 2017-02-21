#pragma once
// #ifndef __XMATH_UTIL_H__
// #define __XMATH_UTIL_H__


namespace xmath{


using uint = unsigned int;


template <class T>
inline constexpr T min(const T& a, const T& b){
	return (a<b)?(a):(b);
}

template <class T>
inline constexpr T max(const T& a, const T& b){
	return (a>b)?(a):(b);
}

template <class T>
inline constexpr T sq(const T& x){
	return x*x;
}


} // xmath


// #endif // __XMATH_UTIL_H__