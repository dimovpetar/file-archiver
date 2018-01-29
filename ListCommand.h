#ifndef _LIST_HEADER_INCLUDED_
#define _LIST_HEADER_INCLUDED_

#include "Command.h"

class List : public Command
{
public:
	List(HuffmanCoding * huffman, fmi::String toList)
		: Command(huffman)
		, toList(toList) { }

	void execute() { huffman->list(toList); }

private:
	fmi::String toList;
};

#endif //_LIST_HEADER_INCLUDED_