#ifndef _HUFFMAN_TABLE_HEADER_INCLUDED_
#define _HUFFMAN_TABLE_HEADER_INCLUDED_

#include "ByteEncoding.h"
#include "MinHeap.h"

class HuffmanTable
{
public:
	HuffmanTable() {};
	HuffmanTable(const HuffmanTable& rhs) {};
	~HuffmanTable() {};

	void countFr		(int * totalFr, size_t size, unsigned char * content);
	void initTable		(ByteEncoding * table);
	void generateLength	(DataNode* root, int len, ByteEncoding *& arr);
	void finalTable		(ByteEncoding sortedTable[], ByteEncoding finalTable[], size_t size);
	void restoreCodes	(unsigned char * content, size_t size, short * countArr, short *symArr);

	DataNode	 * buildHuffmanTree		(int * freq, size_t size);
	ByteEncoding * cannonicalHuffman	(DataNode * root);
	ByteEncoding * buildTable			(unsigned char * content, size_t size);
	
private:
	void clear(DataNode * root);

};

#endif //_HUFFMAN_TABLE_HEADER_INCLUDED_