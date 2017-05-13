#pragma once
// #ifndef __XUTIL_TERM_OUT_H__
// #define __XUTIL_TERM_OUT_H__

#include <iostream>

#include <xutil/seq_t.h>
#include <xutil/heap_sort.h>


template <class T>
inline std::ostream& operator<<(std::ostream& os, const xutil::seq_t<T>& s){
	if (!s.size)
		return os << "()";

	os << "(" << s.data[0];
	for (uint i = 1; i < s.size; i++){
		os << "," << s.data[i];
	}
	return os << ")";
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, const xutil::binary_heap_t<T>& h){
	return os << xutil::seq_t<T>{h.data, h.max_size};
}


// #endif // __XUTIL_TERM_OUT_H__