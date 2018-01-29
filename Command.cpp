#include "Command.h"
#include "ListCommand.h"
#include "PackCommand.h"
#include "UnpackCommand.h"

Command::Command() : huffman(nullptr)
{
}

Command * Command::create(HuffmanCoding * receiver, char **& argv, int & i, int size)
{
	if (*argv == "list")
	{	
		argv++;
		i++;
		return new List(receiver, *argv);
	}
	else if (*argv == "pack")
	{
		argv++;
		i++;
		char * arg1 = *argv;
		argv++;
		i++;
		if (i >= size)
			return nullptr;
		char * arg2 = *argv;
		return new Pack(receiver, arg1, arg2);
	}
	else if (*argv == "unpack")
	{
		argv++;
		i++;
		char * arg1 = *argv;
		argv++;
		i++;
		if (i >= size)
			return nullptr;
		char * arg2 = *argv;
		return new Unpack(receiver, arg1, arg2);
	}
	else
	{
		return nullptr;
	}
}