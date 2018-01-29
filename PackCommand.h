#ifndef _PACKCOMMAND_HEADER_INCLUDED_
#define _PACKCOMMAND_HEADER_INCLUDED_

#include "Command.h"

class Pack : public Command
{
public:
	Pack(HuffmanCoding * huffman, fmi::String toPack, fmi::String toCreate)
		: Command(huffman)
		, toPack(toPack)
		, toCreate(toCreate) {}

	void execute() { huffman->pack(toPack, toCreate); }

private:
	fmi::String toPack;
	fmi::String toCreate;
};

#endif //_PACKCOMMAND_HEADER_INCLUDED_