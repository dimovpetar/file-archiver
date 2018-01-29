#ifndef _STATE_HEADER_INCLUDED_
#define _STATE_HEADER_INCLUDED_

struct State
{
	State(size_t i = 0, size_t shiftPos = 7) :i(i), shiftPos(shiftPos) {};
	
	int bitsShifted() { return 8 - shiftPos; }
	int nextBit()
	{
		if (shiftPos == -1)
		{
			i++;
			shiftPos = 7;
		}

		int temp = content[i] & (1 << (shiftPos));
		temp >>= shiftPos;
		shiftPos--;
		bitsUsed++;
		return temp;
	}

	bool inRange() { return bitsSize > bitsUsed; }

	const unsigned char * content;
	size_t shiftPos;
	size_t i;
	size_t bitsUsed;
	size_t bitsSize;
};


#endif //_STATE_HEADER_INCLUDED_