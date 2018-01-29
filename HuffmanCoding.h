#ifndef _HUFFMANCODING_HEADER_INCLUDED_
#define _HUFFMANCODING_HEADER_INCLUDED_

#include "MyString.h"
#include "State.h"
#include "ByteEncoding.h"
#include "HuffmanTable.h"

class HuffmanCoding
{
public:
	HuffmanCoding();
	HuffmanCoding(const HuffmanCoding & rhs);
	HuffmanCoding& operator=(const HuffmanCoding & rhs);
	~HuffmanCoding();

	void pack	(fmi::String toPack, fmi::String toCreate);
	void unpack	(fmi::String toUnpack, fmi::String dir);
	void list	(fmi::String toList);

private:
	void encodeChunk	  (std::ofstream& os, unsigned char * content, unsigned char * encodedContent);
	void decodeChunk	  (std::ostream& os, size_t bitsCount, unsigned char * content, unsigned char * decodedContent);	
	void wstringToCharArr (std::wstring ws, unsigned char * fileName);
	int	 decode			  (State * s, short * countArr, short * symbol);

private:
	size_t size;
	const size_t KB;
	const size_t BUFFER_SIZE;
	const size_t ENCODED_TREE_SIZE;
	HuffmanTable * huffmanTable;

	enum flag
	{
		FE, /* file end       */
		CONTENT, /* content    */
		SOF, /* start of file */
		SOD, /* start of directory */
		EOD, /* end of directory    */
		ED, /* empty directory */	
	};
};

#endif //_HUFFMANCODING_HEADER_INCLUDED_