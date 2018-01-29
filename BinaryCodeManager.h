#ifndef _BINARY_CODE_MANAGER
#define _BINARY_CODE_MANAGER


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

#endif //_BINARY_CODE_MANAGER