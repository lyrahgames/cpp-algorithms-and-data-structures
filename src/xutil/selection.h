#pragma once
// #ifndef __SELECTION_H__
// #define __SELECTION_H__

#include <string.h>

#include <xutil/binary_heap.h>


namespace xutil{


template <class T>
inline void sift_down_min(binary_heap<T>* heap, uint node){
	uint root = node;
	uint left;

	while ((left = child_idx(root)) < heap->size){
		uint max_node = root;
		uint right = left + 1;

		if (heap->data[max_node] > heap->data[left]){
			max_node = left;
		}

		if ((right < heap->size) && (heap->data[max_node] > heap->data[right])){
			max_node = right;
		}

		if (max_node == root){
			return;
		}else{
			swap(heap, root, max_node);
			root = max_node;
		}
	}
}

template <class T>
inline void heapify_min(binary_heap<T>* heap){
	const uint start = parent_idx(heap->size - 1);

	for (uint i = start; i < heap->size; i--){
		sift_down_min(heap, i);
	}
}

template <class T>
inline void select(T* seq, uint size, T* sorder, uint k){
	binary_heap<T> heap{sorder, k, k};
	memcpy(sorder, seq, k * sizeof(T));

	heapify_min(&heap);

	for (uint i = k; i < size; i++){
		if (seq[i] > heap.data[0]){
			heap.data[0] = seq[i];
			sift_down_min(&heap, 0);
		}
	}
}

template <class T>
inline uint partition_min(T* seq, uint size){
	T p = seq[size-1];
	uint i = 0;

	for (uint j = 0; j < size-1; j++){
		if (seq[j] >= p){
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
inline uint partition_min_rand(T* seq, uint size){
	uint pidx = floorf(float(size) * float(rand()) / float(RAND_MAX));

	// swap
	T tmp = seq[pidx];
	seq[pidx] = seq[size-1];
	seq[size-1] = tmp;

	return partition_min(seq, size);
}

template <class T>
inline void quick_select(T* seq, uint size, T* sorder, uint k){
	uint len = size;
	uint idx = 0;
	k = k - 1;

	while (len > 1){
		uint pidx = partition_min_rand(seq + idx, len);

		if (k == pidx){
			// idx = pidx;
			break;
		}else if (k < pidx){
			len = pidx;
		}else{
			idx = pidx + 1;
			len = len - pidx - 1;
		}
	}

	memcpy(sorder, seq, (k+1) * sizeof(T));
}


} // xutil


// #endif // __SELECTION_H__