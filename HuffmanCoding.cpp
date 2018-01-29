#include "HuffmanCoding.h"
#include "ScanDir.h"
#include "QuickSort.h"
#include "MyVector.h"

#include <fstream>
#include <iostream>

HuffmanCoding::HuffmanCoding() : KB(1024), BUFFER_SIZE(1024 * 100), ENCODED_TREE_SIZE(160), size(0)
{
	huffmanTable = new HuffmanTable;
}

HuffmanCoding::HuffmanCoding(const HuffmanCoding & rhs) : KB(1024), BUFFER_SIZE(1024 * 100), ENCODED_TREE_SIZE(160), size(rhs.size)
{
	huffmanTable = new HuffmanTable(*rhs.huffmanTable);
}

HuffmanCoding& HuffmanCoding::operator = (const HuffmanCoding & rhs)
{
	if (this == &rhs)
		return *this;

	size = rhs.size;	
	huffmanTable = new HuffmanTable(*rhs.huffmanTable);

	return *this;
}

HuffmanCoding::~HuffmanCoding()
{
	delete huffmanTable;
}

void HuffmanCoding::pack(fmi::String toPack, fmi::String toCreate)
{	
	std::ofstream fileToPack(toCreate.c_str(), std::ios::binary | std::ios::trunc);
	if (!fileToPack)
	{
		std::cout << "Problem with " << toCreate;
		return;
	}

	std::cout << "\nPACKING:\n\n";

	size_t nameLength;
	unsigned char * content = new unsigned char[BUFFER_SIZE];
	unsigned char *	encodedContent = new unsigned char[BUFFER_SIZE];
	unsigned char	currFileName[1024];
	unsigned char	flag;	
	fmi::Stack	<size_t>	itemsRemaining; /* files and folders to archive*/
	fmi::Vector	<FileType>	items = getFiles(toPack); /*files and folders*/

	/* loop through all files and folders */
	for (size_t i = 0; i < items.getSize(); i++)
	{	
		if (items[i].isDirectory)
		{			
			if (items[i].nestedFilesCnt == 0)
			{
				flag = flag::ED;
			}
			else
			{
				flag = flag::SOD;
				itemsRemaining.push(items[i].nestedFilesCnt); /* going for the current folder nested files */
			}		
		}
		else
		{
			flag = flag::SOF;		
		}

		wstringToCharArr(items[i].path.data(), currFileName); /* current file name */
		nameLength = strlen((char *)currFileName);

		fileToPack.write((const char *)&flag, sizeof(flag));
		fileToPack.write((const char *)&nameLength, sizeof(nameLength));
		fileToPack.write((const char *)&currFileName, nameLength);

		if (flag == flag::SOF)
		{
			std::ifstream fileToRead(items[i].path, std::ios::binary);
			if (!fileToRead)
			{
				std::cout << "Problem with " << currFileName;
				break;
			}

			/* encoding file */
			while (fileToRead)
			{
				fileToRead.read((char *)&content[0], 16 * KB);
				size = fileToRead.gcount();

				if (size <= 0)
					break;

				flag = flag::CONTENT;
				fileToPack.write((const char *)&flag, sizeof(flag));				
				encodeChunk(fileToPack, content, encodedContent);			
			}

			flag = flag::FE;
			fileToPack.write((const char *)&flag, sizeof(flag));
			std::cout << '\t' << currFileName << " archived.\n";

			bool popped;
			do
			{
				popped = false;
				itemsRemaining.top()--;
				if (itemsRemaining.top() == 0)
				{
					flag = flag::EOD;
					fileToPack.write((const char *)&flag, sizeof(flag));
					itemsRemaining.pop();
					popped = true;
				}
			} while (!itemsRemaining.isEmpty() && popped);

			fileToRead.close();
		} //flag == start of file
	}

	delete[] content;
	delete[] encodedContent;
	fileToPack.close();
}

void HuffmanCoding::unpack(fmi::String toUnpack, fmi::String dir)
{
	std::ifstream fileToUnpack(toUnpack.c_str(), std::ios::binary);

	if (!fileToUnpack)
	{
		std::cout << "Problem with " << toUnpack;
		return;
	}

	std::cout << "\nUNPACKING:\n\n";

	size_t length = 0;
	unsigned char * content = new unsigned char[BUFFER_SIZE];
	unsigned char * decodedContent = new unsigned char[BUFFER_SIZE];
	unsigned char	fileName[1024];
	unsigned char	flag;
	fmi::Stack<fmi::String> paths;
	paths.push(dir);

	while (fileToUnpack)
	{
		fileToUnpack.read((char *)&flag, sizeof(flag));
		
		switch (flag)
		{
			case flag::SOD:
			{
				fileToUnpack.read((char *)&length, sizeof(length));
				fileToUnpack.read((char *)&fileName, length);
				fileName[length] = '\0';
				fmi::String directory(paths.top());
				directory += "\\";
				directory += (char *)fileName;
				CreateDirectoryA(directory.c_str(), NULL);
				paths.push(directory);
				break;
			}
			case flag::ED:
			{
				fileToUnpack.read((char *)&length, sizeof(length));
				fileToUnpack.read((char *)&fileName, length);
				fileName[length] = '\0';
				fmi::String directory(paths.top());
				directory += "\\";
				directory += (char *)fileName;
				CreateDirectoryA(directory.c_str(), NULL);
				break;
			}
			case flag::EOD:
			{
				paths.pop();
				break;
			}
			default:
			{
				fileToUnpack.read((char *)&length, sizeof(length));
				fileToUnpack.read((char *)&fileName, length);
				fileName[length] = '\0';
				fmi::String path(paths.top());
				path += "\\";
				path += (char *)fileName;
				std::ofstream fileToPack((char *)path.c_str(), std::ios::binary | std::ios::trunc);
				if (!fileToPack)
				{
					std::cout << "Problem with " << path;
					delete[] content;
					return;
				}

				std::cout << '\t' <<path << " created.\n  ";

				/* decoding file */
				while (fileToUnpack)
				{
					size_t bitsToRead = 0;
					size_t bytesToRead = 0;

					fileToUnpack.read((char *)&flag, sizeof(flag));
					if (flag == flag::FE)
						break;

					fileToUnpack.read((char *)&bitsToRead, sizeof(bitsToRead));
					fileToUnpack.read((char *)&bytesToRead, sizeof(bytesToRead));
					fileToUnpack.read((char *)&content[0], bytesToRead);

					size = fileToUnpack.gcount();
					if (size == 0)
						break;

					decodeChunk(fileToPack, bitsToRead, content, decodedContent);
				}
				fileToPack.close();
			}
		} //switch
	} 

	delete[] content;
	delete[] decodedContent;
	fileToUnpack.close();
}

void HuffmanCoding::list(fmi::String toList)
{
	std::ifstream fileToList(toList.c_str(), std::ios::binary);

	if (!fileToList)
	{
		std::cout << "Problem with " << toList;
		return;
	}

	std::cout << "\nList:\n\n";

	size_t length = 0;
	unsigned char fileName[1024];
	unsigned char flag;
	
	while (fileToList)
	{
		fileToList.read((char *)&flag, sizeof(flag));

		switch (flag)
		{
			case flag::SOD:
			{
				fileToList.read((char *)&length, sizeof(length));
				fileToList.seekg(length, std::ios::cur);
				break;
			}
			case flag::ED:
			{
				fileToList.read((char *)&length, sizeof(length));
				fileToList.seekg(length, std::ios::cur);
				break;
			}
			case flag::EOD:
			{
				break;
			}
			default:
			{
				fileToList.read((char *)&length, sizeof(length));
				fileToList.read((char *)&fileName, length);
				fileName[length] = '\0';
				fmi::String fileName((char *)fileName);
				std::cout << '\t' << fileName << "\n";

				while (fileToList)
				{
					size_t bytesToRead = 0;

					fileToList.read((char *)&flag, sizeof(flag));
					if (flag == flag::FE)
						break;

					fileToList.seekg(sizeof(size_t), std::ios::cur); //bits to read
					fileToList.read((char *)&bytesToRead, sizeof(bytesToRead));
					fileToList.seekg(bytesToRead, std::ios::cur);
				}
			}
		} //switch
	}

	fileToList.close();
}

void HuffmanCoding::encodeChunk(std::ofstream& os, unsigned char * content, unsigned char * encodedContent)
{
	ByteEncoding * table = huffmanTable->buildTable(content, size);
	size_t pos = 0;
	size_t shiftPos = 0;
	unsigned char byteBuffer = 0;

	/* encoding tree lengths */
	for (size_t i = 0; i < 256; i++)
	{	
		size_t lengthToWrite = table[i].codelength;

		/* max length is 16, which is 5 bits */
		for (size_t j = 0; j < 5; j++)
		{
			byteBuffer = byteBuffer | ((lengthToWrite & 1) << shiftPos);
			shiftPos++;

			if (shiftPos == 8)
			{
				encodedContent[pos] = byteBuffer;
				pos++;
				shiftPos = 0;
				byteBuffer = 0;
			}
			lengthToWrite = lengthToWrite >> 1;
		}
	}

	/* encoding content */
	shiftPos = 0;
	int m = 7;
	for (size_t i = 0; i < size; i++)
	{
		unsigned short codeToWrite = table[content[i]].code;
		int lengthToWrite = table[content[i]].codelength;

		for (int j = lengthToWrite - 1; j >= 0; j--)
		{
			int help = codeToWrite & (1 << j);
			help >>= j;

			byteBuffer |= help << m;
			shiftPos++;

			m--;
			if (shiftPos == 8)
			{
				encodedContent[pos++] = byteBuffer;
				shiftPos = 0;
				byteBuffer = 0;
				m = 7;
			}
		}
	}

	size_t bitsCount;
	encodedContent[pos++] = byteBuffer;
	bitsCount = (pos - 1) * 8 + shiftPos;

	os.write((const char *)&bitsCount, sizeof(bitsCount));
	os.write((const char *)&pos, sizeof(pos));
	os.write((const char *)&encodedContent[0], pos);

	delete[] table;
}

void HuffmanCoding::decodeChunk(std::ostream& os, size_t bitsCount ,unsigned char * content, unsigned char * decodedContent)
{
	if (size < ENCODED_TREE_SIZE) 
		return;

	short countArr[16] = { 0, };
	short symbolArr[256];
	huffmanTable->restoreCodes(content, size, countArr, symbolArr);

	/* decode content */
	State * contentState = new State();
	contentState->content = content + 160;
	contentState->bitsUsed = 160 * 8;
	contentState->bitsSize = bitsCount;

	size_t cnt = 0;
	while (contentState->inRange())
	{
		decodedContent[cnt++] = decode(contentState, countArr, symbolArr);
	}

	os.write((const char*)&decodedContent[0], cnt);

	delete contentState;
}

int HuffmanCoding::decode(State * s, short * countArr, short * symbol)
{
	int code = 0;           /* len bits being decoded */
	int first = 0;          /* first code of length len */	
	int index = 0;          /* index of first code of length len in symbol table */
	int count;			    /* number of codes of length len */

	/* len 0 is unused, because no key is encoded with 0 bits */
	for (int len = 1; len <= 15; len++) 
	{
		int nextBit = s->nextBit();
		code |= nextBit;
		count = countArr[len];
		if (code - count < first)   /* if length len, return symbol */
			return symbol[index + (code - first)];
		index += count;                 
		first += count;
		first <<= 1;
		code <<= 1;
	}

	return -1;
}

void HuffmanCoding::wstringToCharArr(std::wstring ws, unsigned char * fileName)
{
	const wchar_t * help = ws.data();
	unsigned char buffer[1024];
	size_t i = 0;
	for (; i < ws.length(); i++)
	{
		buffer[i] = (unsigned char)help[i];
	}
	buffer[i] = '\0';
	size_t pos = 0;

	size_t len = strlen((char *)buffer);
	for (size_t i = 0; i < len; i++)
	{
		if (buffer[i] == '\\')
			pos = i + 1;
	}

	int k = 0;
	for (size_t i = pos; i < ws.length(); i++)
	{
		fileName[k] = buffer[i];
		k++;
	}

	fileName[k] = '\0';
}
