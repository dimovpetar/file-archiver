#ifndef _ARCHIVER_HEADER_INCLUDED_
#define _ARCHIVER_HEADER_INCLUDED_

#include "MyVector.h"
#include "HuffmanCoding.h"
#include "Command.h"

class Archiver
{
public:
	Archiver();
	Archiver(const Archiver& rhs);
	Archiver& operator=(const Archiver& rhs);
	~Archiver();

	void startProgram(int argc, char * argv[]);

private:
	void order(Command * command) 	{ command->execute(); }

private:
	HuffmanCoding  * huffman;
	fmi::Vector<Command *> log;
};

#endif //_ARCHIVER_HEADER_INCLUDED_