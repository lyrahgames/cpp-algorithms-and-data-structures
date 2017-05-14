#pragma once
// #ifndef __BINARY_HEAP_H__
// #define __BINARY_HEAP_H__


namespace xutil{


template <class T>
struct binary_heap{
	T* data;
	uint size;
	uint max_size;
};


inline uint parent_idx(uint node){
	return (node - 1) >> 1; // if (node == 0) function returns uint(-1)
}

inline uint child_idx(uint node){
	return (node << 1) + 1;
}

inline uint lchild_idx(uint node){
	return child_idx(node);
}

inline uint rchild_idx(uint node){
	return lchild_idx(node) + 1;
}

template <class T>
inline void swap(binary_heap<T>* heap, uint node1, uint node2){
	T tmp = heap->data[node1];
	heap->data[node1] = heap->data[node2];
	heap->data[node2] = tmp;
}

template <class T>
inline void sift_down(binary_heap<T>* heap, uint node){
	uint root = node;
	uint left;

	while ((left = child_idx(root)) < heap->size){
		uint max_node = root;
		uint right = left + 1;

		if (heap->data[max_node] < heap->data[left]){
			max_node = left;
		}

		if ((right < heap->size) && (heap->data[max_node] < heap->data[right])){
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
inline void heapify(binary_heap<T>* heap){
	const uint start = parent_idx(heap->size - 1);

	for (uint i = start; i < heap->size; i--){
		sift_down(heap, i);
	}
}

template <class T>
inline void heap_sort(binary_heap<T>* heap){
	heapify(heap);

	for (uint i = heap->size - 1; i > 0; i--){
		swap(heap, 0, i);

		heap->size--;
		sift_down(heap, 0);
	}
}

template <class T>
inline void heap_sort(T* seq, uint size){
	binary_heap<T> heap{seq, size, size};

	heap_sort(&heap);
}

template <class T>
inline T max(const binary_heap<T>* heap){
	return heap->data[0];
}

template <class T>
inline T max(const binary_heap<T>& heap){
	return heap.data[0];
}

template <class T>
inline int extract_max(binary_heap<T>* heap, T* max){
	if (heap->size < 1)
		return -1;

	*max = heap->data[0];
	heap->size--;
	heap->data[0] = heap->data[heap->size];

	sift_down(heap, 0);

	return 0;
}

template <class T>
inline int insert(binary_heap<T>* heap, const T& key){
	if (heap->size >= heap->max_size)
		return -1;

	heap->data[heap->size] = key;

	uint node = heap->size;
	uint par = parent_idx(node);
	while ((node > 0) && (heap->data[par] < heap->data[node])){
		swap(heap, par, node);

		node = par;
		par = parent_idx(par);
	}

	heap->size++;

	return 0;
}


} // xutil


// #endif // __BINARY_HEAP_H__