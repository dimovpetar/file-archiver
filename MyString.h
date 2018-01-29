#ifndef _MYSTRING_HEADER_INCLUDED_
#define _MYSTRING_HEADER_INCLUDED_


#include <iostream>

namespace fmi
{	
	class String
	{
		friend std::ostream& operator << (std::ostream& out, const String& s);
		friend std::istream& operator >> (std::istream& in, String& s);

	public:
		String();
		String(const String& rhs);
		String(const char* str);
		String& operator= (const String& rhs);
		~String();

		bool operator == (const String& rhs) const;
		String& operator+=(const char * add);

		char * c_str()	const	{ return str;		}	
		char * getStr() const	{ return str;		}
		size_t getLen() const	{ return size - 1;	}

	private:
		void copy(const String& rhs);
	
	private:
		char * str;
		size_t size;
	};

} //fmi


#endif //_MYSTRING_HEADER_INCLUDED_
