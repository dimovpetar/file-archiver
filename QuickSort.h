#ifndef _QUICKSORT_HEADER_INCLUDED_
#define _QUICKSORT_HEADER_INCLUDED_

#include <stdlib.h>   /* abs */
#include <algorithm> /* swap */

namespace fmi
{
	template <typename T>
	int partition(T * data, int left, int right)
	{
		int size = abs(left - right) + 1;
		if (size == 2)
		{
			if (data[left] > data[right])
				std::swap(data[left], data[right]);
			return left + 1;
		}

		int mid = left + size / 2;

		if (data[left] > data[mid])
			std::swap(data[left], data[mid]);
		if (data[mid] > data[right])
			std::swap(data[mid], data[right]);
		if (data[left] > data[mid])
			std::swap(data[left], data[mid]);

		/* puts median of tree as pivot in the right */
		std::swap(data[mid], data[right]);

		int pivot = right;
		int smaller = left - 1;

		for (int i = left; i <= right - 1; i++)
		{
			if (data[i] < data[pivot])
			{
				smaller++;
				std::swap(data[smaller], data[i]);
			}
		}

		std::swap(data[smaller + 1], data[pivot]);
		return smaller + 1;
	}

	template<typename T>
	void quickSortStep(T * arr, int left, int right)
	{
		if (left < right)
		{
			int p = partition(arr, left, right);
			quickSortStep(arr, left, p - 1);
			quickSortStep(arr, p + 1, right);
		}
	}

	template<typename T>
	void quickSort(T * data, size_t size)
	{
		quickSortStep(data, 0, size - 1);
	}


} //fmi

#endif//_QUICKSORT_HEADER_INCLUDED_