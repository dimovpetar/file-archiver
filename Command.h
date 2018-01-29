#ifndef _COMMAND_HEADER_INCLUDED_
#define _COMMAND_HEADER_INCLUDED_

#include "HuffmanCoding.h"
#include "MyString.h"

class Command
{
public:
	static Command * create(HuffmanCoding * receiver, char **& argv, int & i, int size);

	Command();
	Command(HuffmanCoding * huffman) : huffman(huffman) {}
	virtual void execute() = 0;

protected:
	HuffmanCoding * huffman;
};

#endif //_COMMAND_HEADER_INCLUDED_