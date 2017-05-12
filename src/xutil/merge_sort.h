#pragma once
// #ifndef __MERGE_SORT_H__
// #define __MERGE_SORT_H__

#include <string.h>


namespace xutil{


template <class T>
inline void merge(T* in_seq1, uint size1, T* in_seq2, uint size2, T* out_seq){
	uint idx1 = 0;
	uint idx2 = 0;
	uint idx = 0;

	while ((idx1 < size1) && (idx2 < size2)){
		if (in_seq1[idx1] <= in_seq2[idx2]){
			out_seq[idx] = in_seq1[idx1];
			idx1++;
		}else{
			out_seq[idx] = in_seq2[idx2];
			idx2++;
		}

		idx++;
	}

	const uint tmp1 = size1 - idx1;
	memcpy(out_seq + idx, in_seq1 + idx1, tmp1 * sizeof(T));
	const uint tmp2 = size2 - idx2;
	memcpy(out_seq + idx, in_seq2 + idx2, tmp2 * sizeof(T));
}

template <class T>
inline void merge_sort(T* in_seq, uint size, T* out_seq){
	if (size <= 1)
		return;

	const uint size1 = size >> 1;
	const uint size2 = size - size1;

	merge_sort(in_seq, size1, out_seq);
	merge_sort(in_seq + size1, size2, out_seq + size1);
	merge(in_seq, size1, in_seq + size1, size2, out_seq);
	memcpy(in_seq, out_seq, size * sizeof(T));
}

// bottom-up non-recursive merge sort
// template <class T>
// inline void merge_sort_bu(T* in_seq, uint size, T* out_seq){
// 	T* in = in_seq;
// 	T* out = out_seq;
// 	uint r1 = 0;
// 	uint r2 = 0;

// 	for (uint w = 1; w < size; w = w << 1){
// 		const uint w2 = w << 1;
// 		const uint merge_count = size / w2;
// 		const uint rem = size % w;
// 		const uint rem2 = size % w2;

// 		for (uint i = 0; i < merge_count; i++){
// 			merge(in_seq + i*w2, w, in_seq + i*w2 + w, w, out_seq + i*w2);
// 		}

// 		merge(in_seq + merge_count*w2, rem2-rem, in_seq + merge_count*w2 + (rem2-rem), rem, out_seq + merge_count*w2);


// 		memcpy(in_seq, out_seq, size * sizeof(T));
// 	}
// }

// bottom-up non-recursive merge sort
template <class T>
inline void merge_sort_bu(T* in_seq, uint size, T* out_seq){
	T* in = in_seq;
	T* out = out_seq;
	uint exp = 0;
	uint w = 1;
	uint w2 = 2;
	uint merge_count = size >> 1;

	while (w < size){
		const uint r1 = size & (w-1);
		const uint r2 = size & w;
		const uint full = merge_count << (exp + 1);

		for (uint i = 0; i < full; i+=w2){
			merge(in + i, w, in + i + w, w, out + i);
		}

		merge(in + full, r2, in + full + r2, r1, out + full);

		w = w2;
		w2 = w2 << 1;
		exp++;
		merge_count = merge_count >> 1;

		// swap
		T* tmp = in;
		in = out;
		out = tmp;
	}

	if (!(exp & 0x01)){
		memcpy(out_seq, in_seq, size * sizeof(T));
	}
}


} // xutil

// #endif // __MERGE_SORT_H__