#pragma once
// #ifndef __VEC_H__
// #define __VEC_H__

#include <cmath>
#include <algorithm>

#include <xmath/xmath_util.h>


namespace xmath{


template <class T, uint N>
struct vec{
	// static
	using type = T;
	static constexpr uint _size_ = N;
	static constexpr uint size(){return N;}

	// default
	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;

	// conversion vec
	template <class U, uint M>
	vec(const vec<U,M>& src);
	template <class U, uint M>
	vec& operator=(const vec<U,M>& src);


	vec(std::initializer_list<T> l){
		auto it = l.begin();
		for (uint i = 0; it != l.end(); i++, it++)
			v[i] = *it;
	}

	vec(T val){
		for (uint i = 0; i < N; i++)
			v[i] = val;
	}


	// access
	T& operator[](uint idx){return v[idx];}
	constexpr const T& operator[](uint idx) const{return v[idx];}


	// data
	T v[N];
};


template <class T>
struct vec<T,2>{
	// static
	using type = T;
	static constexpr uint _size_ = 2;
	static constexpr uint size(){return 2;}

	// default
	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;

	// conversion vec
	template <class U, uint M>
	vec(const vec<U,M>& src);
	template <class U>
	vec(const vec<U,1>& src);
	template <class U, uint M>
	vec& operator=(const vec<U,M>& src);
	template <class U>
	vec& operator=(const vec<U,1>& src);

	// x, y
	vec(T v_x, T v_y): x(v_x), y(v_y){}
	vec(T val): x(val), y(val){}


	// access
	T& operator[](uint idx){return v[idx];}
	constexpr const T& operator[](uint idx) const{return v[idx];}


	// data
	union{
		T v[2];
		struct{T x, y;};
	};
};


template <class T>
struct vec<T,3>{
	// static
	using type = T;
	static constexpr uint _size_ = 3;
	static constexpr uint size(){return 3;}

	// default
	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;

	// conversion vec
	template <class U, uint M>
	vec(const vec<U,M>& src);
	template <class U, uint M>
	vec& operator=(const vec<U,M>& src);

	// x, y, z
	vec(T v_x, T v_y, T v_z): x(v_x), y(v_y), z(v_z){}
	vec(T val): x(val), y(val), z(val){}


	// access
	T& operator[](uint idx){return v[idx];}
	constexpr const T& operator[](uint idx) const{return v[idx];}


	// data
	union{
		T v[3];
		struct{T x, y, z;};
	};
};


template <class T>
struct vec<T,4>{
	// static
	using type = T;
	static constexpr uint _size_ = 4;
	static constexpr uint size(){return 4;}

	// default
	vec() = default;
	vec(const vec&) = default;
	vec& operator=(const vec&) = default;
	vec(vec&&) = default;
	vec& operator=(vec&&) = default;

	// conversion vec
	template <class U, uint M>
	vec(const vec<U,M>& src);
	template <class U, uint M>
	vec& operator=(const vec<U,M>& src);

	// x, y, z, w
	vec(T v_x, T v_y, T v_z, T v_w): x(v_x), y(v_y), z(v_z), w(v_w){}
	vec(T val): x(val), y(val), z(val), w(val){}


	// access
	T& operator[](uint idx){return v[idx];}
	constexpr const T& operator[](uint idx) const{return v[idx];}


	// data
	union{
		T v[4];
		struct{T x, y, z, w;};
	};
};



// definition of conversion
template <class T, uint N>
template <class U, uint M>
inline vec<T,N>::vec(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,N); i++){
		v[i] = static_cast<T>(src[i]);
	}
}

template <class T>
template <class U, uint M>
inline vec<T,2>::vec(const vec<U,M>& src): x(static_cast<T>(src[0])), y(static_cast<T>(src[1])){}

template <class T>
template <class U>
inline vec<T,2>::vec(const vec<U,1>& src): x(static_cast<T>(src[0])){}

template <class T>
template <class U, uint M>
inline vec<T,3>::vec(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,3u); i++){
		v[i] = static_cast<T>(src[i]);
	}
}

template <class T>
template <class U, uint M>
inline vec<T,4>::vec(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,4u); i++){
		v[i] = static_cast<T>(src[i]);
	}
}

template <class T, uint N>
template <class U, uint M>
inline vec<T,N>& vec<T,N>::operator=(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,N); i++){
		v[i] = static_cast<T>(src[i]);
	}
	return *this;
}

template <class T>
template <class U, uint M>
inline vec<T,2>& vec<T,2>::operator=(const vec<U,M>& src){
	v[0] = static_cast<T>(src[0]);
	v[1] = static_cast<T>(src[1]);
	return *this;
}

template <class T>
template <class U>
inline vec<T,2>& vec<T,2>::operator=(const vec<U,1>& src){
	v[0] = static_cast<T>(src[0]);
	return *this;
}

template <class T>
template <class U, uint M>
inline vec<T,3u>& vec<T,3u>::operator=(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,3u); i++){
		v[i] = static_cast<T>(src[i]);
	}
	return *this;
}

template <class T>
template <class U, uint M>
inline vec<T,4u>& vec<T,4u>::operator=(const vec<U,M>& src){
	for (uint i = 0; i < std::min(M,4u); i++){
		v[i] = static_cast<T>(src[i]);
	}
	return *this;
}


// abstract and general packing of operations for vectors
template <uint M, class Op, class T, uint N>
inline auto pack_op(Op op, const vec<T,N>& v1, const vec<T,N>& v2) -> vec<decltype(op(v1[0], v2[0])),N>{
	vec<decltype(op(v1[0], v2[0])),N> res{};

	for (uint i = 0; i < std::min(N,M); i++)
		res[i] = op(v1[i], v2[i]);

	return res;
}

template <uint M, class Op, class T, uint N>
inline auto pack_op(Op op, const vec<T,N>& v) -> vec<decltype(op(v[0])),N>{
	vec<decltype(op(v[0])),N> res{};

	for (uint i = 0; i < std::min(N,M); i++)
		res[i] = op(v[i]);

	return res;
}

template <uint M, class Op, class T, uint N>
inline T hpack_op(Op op, const vec<T,N>& v){
	T res = v[0];

	for (uint i = 1; i < std::min(N,M); i++)
		res = op(res, v[i]);

	return res;
}


// predefined packed operations
template <class T, uint N>
inline vec<T,N> operator+(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x+y;}, v1, v2);
}

template <uint M, class T, uint N>
inline vec<T,N> add(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<M>([](const T& x, const T& y){return x+y;}, v1, v2);
}

template <class T, uint N>
inline vec<T,N> add(const vec<T,N>& v1, const vec<T,N>& v2){
	return v1 + v2;
}

template <class T, uint N>
inline vec<T,N> operator-(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x-y;}, v1, v2);
}

template <class T, uint N>
inline vec<T,N> operator-(const vec<T,N>& v){
	return pack_op<N>([](const T& x){return -x;}, v);
}

template <class T, uint N>
inline vec<T,N> operator*(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x*y;}, v1, v2);
}

template <class T, uint N>
inline vec<T,N> operator/(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>([](const T& x, const T& y){return x/y;}, v1, v2);
}

template <class T, uint N>
inline vec<T,N> operator*(const T& s, const vec<T,N>& v){
	return pack_op<N>([&s](const T& x){return s*x;}, v);
}

template <class T, uint N>
inline vec<T,N> min(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>(min<T>, v1, v2);
}

template <class T, uint N>
inline vec<T,N> max(const vec<T,N>& v1, const vec<T,N>& v2){
	return pack_op<N>(max<T>, v1, v2);
}

template <class T, uint N>
inline vec<T,N> sq(const vec<T,N>& v){
	return pack_op<N>(sq<T>, v);
}

template <class T, uint N>
inline T sum(const vec<T,N>& v){
	return hpack_op<N>([](const T& x, const T& y){return x+y;}, v);
}

template <class T, uint N>
inline T prod(const vec<T,N>& v){
	return hpack_op<N>([](const T& x, const T& y){return x*y;}, v);
}

template <class T, uint N>
inline T dot(const vec<T,N>& v1, const vec<T,N>& v2){
	return sum(v1 * v2);
}


// float specialization
template <uint N>
inline vec<float,N> fl(const vec<float,N>& v){
	return pack_op<N>(floorf, v);
}

template <uint N>
inline vec<float,N> cl(const vec<float,N>& v){
	return pack_op<N>(ceilf, v);
}


} // xmath


// #endif // __VEC_H__