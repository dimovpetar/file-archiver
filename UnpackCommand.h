#ifndef _UNPACKCOMMAND_HEADER_INCLUDED_
#define _UNPACKCOMMAND_HEADER_INCLUDED_

#include "Command.h"

class Unpack : public Command
{
public:
	Unpack(HuffmanCoding * huffman, fmi::String toUnpack, fmi::String toCreate)
		: Command(huffman)
		, toUnpack(toUnpack)
		, toCreate(toCreate) {}

	void execute() { huffman->unpack(toUnpack, toCreate); }

private:
	fmi::String toUnpack;
	fmi::String toCreate;
};

#endif //_UNPACKCOMMAND_HEADER_INCLUDED_