#pragma once
// #ifndef __INSERTION_SORT_H__
// #define __INSERTION_SORT_H__

#include <xutil/seq_t.h>


namespace xutil{


template <class T>
inline void insertion_sort(T* sequence, uint size){
	for (uint i = 1; i < size; i++){
		T tmp = sequence[i];
		uint k = i - 1;

		while ((k < size) && (sequence[k] > tmp)){
			sequence[k+1] = sequence[k];
			k--;
		}

		sequence[k+1] = tmp;
	}
}

template <class T>
inline void insertion_sort_r(T* sequence, uint size){
	for (uint i = 1; i < size; i++){
		T tmp = sequence[i];
		uint k = i - 1;

		while ((k < size) && (sequence[k] < tmp)){
			sequence[k+1] = sequence[k];
			k--;
		}

		sequence[k+1] = tmp;
	}
}


} // xutil

// #endif // __INSERTION_SORT_H__