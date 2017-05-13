#pragma once
// #ifndef __HEAP_SORT_H__
// #define __HEAP_SORT_H__


namespace xutil{


template <class T>
struct binary_heap_t{
	T* data;
	uint size;
	uint max_size;
};

template <class T>
inline void max_heapify(binary_heap_t<T>* heap, uint i){
	const uint l = (i << 1) + 1;
	const uint r = l + 1;
	uint max;

	if ((l < heap->size) && (heap->data[l] > heap->data[i])){
		max = l;
	}else{
		max = i;
	}

	if ((r < heap->size) && (heap->data[r] > heap->data[max])){
		max = r;
	}

	if (max != i){
		// swap
		T tmp = heap->data[i];
		heap->data[i] = heap->data[max];
		heap->data[max] = tmp;

		max_heapify(heap, max);
	}
}

template <class T>
inline void build_max_heap(binary_heap_t<T>* heap){
	for (uint i = (heap->size-1) >> 1; i > 0; i--){
		max_heapify(heap, i-1);
	}
}

template <class T>
inline void heap_sort(binary_heap_t<T>* heap){
	build_max_heap(heap);

	for (uint i = heap->size - 1; i > 0; i--){
		// swap
		T tmp = heap->data[0];
		heap->data[0] = heap->data[i];
		heap->data[i] = tmp;

		heap->size--;
		max_heapify(heap, 0);
	}
}


} // xutil


// #endif // __HEAP_SORT_H__