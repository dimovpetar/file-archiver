#ifndef _BYTE_ENCODING_HEADER_INCLUDED_
#define _BYTE_ENCODING_HEADER_INCLUDED_


struct ByteEncoding
{
	ByteEncoding();
	ByteEncoding& operator=(const ByteEncoding& rhs);
	ByteEncoding(unsigned char key, size_t codelength = 0, unsigned short code = 0);

	bool operator < (const ByteEncoding& rhs) const;
	bool operator > (const ByteEncoding& rhs) const;
	
	void print() const;

	unsigned char key;
	unsigned short code;
	size_t codelength;
};

#endif //_BYTE_ENCODING_HEADER_INCLUDED_