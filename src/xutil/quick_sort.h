#pragma once
// #ifndef __QUICK_SORT_H__
// #define __QUICK_SORT_H__

#include <stdlib.h>


namespace xutil{


template <class T>
inline uint partition(T* seq, uint size){
	T p = seq[size-1];
	uint i = 0;

	for (uint j = 0; j < size-1; j++){
		if (seq[j] <= p){
			// swap
			T tmp = seq[i];
			seq[i] = seq[j];
			seq[j] = tmp;

			i++;
		}
	}

	// swap
	T tmp = seq[i];
	seq[i] = seq[size-1];
	seq[size-1] = tmp;

	return i;
}

template <class T>
inline uint partition_rand(T* seq, uint size){
	uint pidx = floorf(float(size) * float(rand()) / float(RAND_MAX));

	// swap
	T tmp = seq[pidx];
	seq[pidx] = seq[size-1];
	seq[size-1] = tmp;

	return partition(seq, size);
}

template <class T>
inline void quick_sort(T* seq, uint size){
	if (size <= 1){
		return;
	}

	const uint pidx = partition(seq, size);
	quick_sort(seq, pidx);
	quick_sort(seq + pidx, size - pidx);
}

template <class T>
inline void quick_sort_rand(T* seq, uint size){
	if (size <= 1)
		return;

	const uint pidx = partition_rand(seq, size);
	quick_sort_rand(seq, pidx);
	quick_sort_rand(seq+pidx, size - pidx);
}


} // xutil


// #endif // __QUICK_SORT_H__