#pragma once
// #ifndef __XUTIL_TERM_OUT_H__
// #define __XUTIL_TERM_OUT_H__

#include <iostream>

#include <xutil/seq_t.h>


template <class T>
std::ostream& operator<<(std::ostream& os, const xutil::seq_t<T>& s){
	if (!s.size)
		return os << "()";

	os << "(" << s.data[0];
	for (uint i = 1; i < s.size; i++){
		os << "," << s.data[i];
	}
	return os << ")";
}


// #endif // __XUTIL_TERM_OUT_H__