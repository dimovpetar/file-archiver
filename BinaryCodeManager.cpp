#include "BinaryCodeManager.h"

#include <iostream>

ByteEncoding::ByteEncoding() 
{ 
}

ByteEncoding& ByteEncoding::operator = (const ByteEncoding& rhs)
{
	if (this == &rhs)
		return *this;
	key = rhs.key;
	codelength = rhs.codelength;
	code = rhs.code;

	return *this;
}

ByteEncoding::ByteEncoding(unsigned char key, size_t codelength , unsigned short code )
	: key(key), codelength(codelength), code(code)
{
}

bool ByteEncoding::operator < (const ByteEncoding& rhs) const
{
	if (codelength < rhs.codelength)
		return true;
	else if (codelength == rhs.codelength)
		return key < rhs.key;

	return false;
}

bool ByteEncoding::operator >(const ByteEncoding& rhs)const
{
	return rhs < *this;
}

void ByteEncoding::print() const
{
	std::cout << key << " " << codelength << " code: ";
	for (int i = codelength - 1; i >= 0; i--)
	{
		std::cout << (code & 1 << i ? "1" : "0");
	}
	std::cout << std::endl;
}