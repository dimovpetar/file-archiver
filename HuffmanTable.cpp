#include "HuffmanTable.h"
#include "MinHeap.h"
#include "QuickSort.h"


ByteEncoding * HuffmanTable::buildTable(unsigned char * content, size_t size)
{
	int totalFr[256] = { 0, };
	countFr(totalFr, size, content);
	DataNode * root = buildHuffmanTree(totalFr, 256);
	ByteEncoding * table = cannonicalHuffman(root);
	clear(root);
	return table;
}

void HuffmanTable::countFr(int * totalFr, size_t size, unsigned char * content)
{
	for (size_t i = 0; i < size; i++)
		totalFr[content[i]]++;
}

DataNode * HuffmanTable::buildHuffmanTree(int * freq, size_t size)
{
	fmi::MinHeap  mh;

	for (size_t i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
			mh.insert(new DataNode(freq[i], (unsigned char)i, false));
	}

	while (mh.size() != 1)
	{
		DataNode * left = mh.getMin();
		mh.removeMin();

		DataNode * right = mh.getMin();
		mh.removeMin();

		DataNode * top = new DataNode(left->frequency + right->frequency, '$', true);
		top->left = left;
		top->right = right;
		mh.insert(top);
	}

	DataNode * root = mh.getMin();
	mh.removeMin();

	if (!root->isInternal) //special case of a single element
	{
		DataNode * top = new DataNode(root->frequency + 1, '$', true);
		top->left = root;
		return top;
	}

	return root;
}

ByteEncoding * HuffmanTable::cannonicalHuffman(DataNode * root)
{
	ByteEncoding * sortedTable = new ByteEncoding[256];
	initTable(sortedTable);
	generateLength(root, 0, sortedTable);
	fmi::quickSort(sortedTable, 256);

	int firstLetter = 0;
	while (firstLetter < 256 && sortedTable[firstLetter].codelength == 0)
		firstLetter++;

	if (firstLetter < 256)
		sortedTable[firstLetter].code = 0;


	int last = firstLetter;
	for (size_t i = firstLetter + 1; i < 256; i++)
	{
		if (sortedTable[i].codelength != 0)
		{
			sortedTable[i].code = sortedTable[last].code + 1;
			int diff = sortedTable[i].codelength - sortedTable[last].codelength;
			sortedTable[i].code <<= diff;
			last = i;
		}
	}

	ByteEncoding * table = new ByteEncoding[256];
	finalTable(sortedTable, table, 256);
	delete[] sortedTable;

	return table;
}

void HuffmanTable::clear(DataNode * root)
{
	if (!root) return;
	clear(root->left);
	clear(root->right);
	delete root;
}

void HuffmanTable::generateLength(DataNode* root, int len, ByteEncoding *& arr)
{
	if (!root)
		return;

	if (!root->isInternal)
		arr[root->key].codelength = len;

	generateLength(root->left, len + 1, arr);
	generateLength(root->right, len + 1, arr);
}


void HuffmanTable::initTable(ByteEncoding * table)
{
	for (size_t i = 0; i < 256; i++)
	{
		table[i].code = 0;
		table[i].key = (unsigned char)i;
		table[i].codelength = 0;
	}
}

void HuffmanTable::finalTable(ByteEncoding sortedTable[], ByteEncoding finalTable[], size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		finalTable[sortedTable[i].key].code = sortedTable[i].code;
		finalTable[sortedTable[i].key].codelength = sortedTable[i].codelength;
		finalTable[sortedTable[i].key].key = sortedTable[i].key;
	}
}


/* modifying countArr and sym Arr */
void HuffmanTable::restoreCodes(unsigned char * content, size_t size, short * countArr, short * symArr)
{
	ByteEncoding * basicTable = new ByteEncoding[256];
	initTable(basicTable);
	int pos = 0;
	int shiftPos = 0;
	unsigned char buffer = 0;

	
	/* decode tree lengths */
	for (size_t i = 0; i < 160; i++)
	{
		size_t lengthToRead = content[i];
		for (int j = 0; j < 8; j++)
		{
			buffer = buffer | ((lengthToRead & 1) << shiftPos);
			shiftPos++;

			if (shiftPos == 5)
			{
				basicTable[pos].codelength = (size_t)buffer;
				pos++;
				shiftPos = 0;
				buffer = 0;
			}
			lengthToRead = lengthToRead >> 1;
			shiftPos = shiftPos % 8;
		}
	}

	/* restore tree codes */

	fmi::quickSort(basicTable, 256);
	int p = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (basicTable[i].codelength != 0)
		{
			symArr[p] = basicTable[i].key;
			p++;
			countArr[basicTable[i].codelength]++;
		}
	}
}