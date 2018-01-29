#ifndef _MINHEAP_HEADER_INCLUDED_
#define _MINHEAP_HEADER_INCLUDED_

#include "MyVector.h"

struct DataNode
{
	unsigned char key;
	int frequency;
	DataNode * left, *right;
	bool isInternal;

	DataNode(const DataNode & rhs)
	{
		key = rhs.key;
		frequency = rhs.frequency;
		left = rhs.left;
		right = rhs.right;
	}

	DataNode(int frequency, unsigned char key, bool isInternal) 
		: left(nullptr), right(nullptr), key(key), frequency(frequency), isInternal(isInternal)
	{ }

	bool operator > (const DataNode& rhs) { return frequency > rhs.frequency; }
};

namespace fmi
{
	class MinHeap
	{
	public:
		MinHeap();
		MinHeap(const MinHeap& rhs);
		MinHeap& operator=(const MinHeap& rhs);
		~MinHeap();
		MinHeap(DataNode  * arr[], size_t size);


		void	insert	(DataNode * val);
		void	removeMin();
		size_t	size() const { return data.getSize(); }
		DataNode *	getMin	();
		
	private:
		void heapify();
		void minHeapify	(int indx); //put lighter elements upper
		void bubbleUp	(int indx);

	private:
		fmi::Vector<DataNode *> data;

	};

}//fmi


#endif //_MINHEAP_HEADER_INCLUDED_