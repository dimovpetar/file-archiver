#ifndef _SCANDIR_HEADER_INCLUDED_
#define _SCANDIR_HEADER_INCLUDED_

#include "MyStack.h"
#include "MyVector.h"
#include <Windows.h>


struct FileType
{
	std::wstring path;
	bool isDirectory;
	size_t nestedFilesCnt;
};

/**
* Using windows API to get vector of all the files in a directory
* and the number of subfiles in each directory
*/

bool listFiles(std::wstring path, std::wstring mask, fmi::Vector<FileType>& items)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	std::wstring spec;
	fmi::Stack<std::wstring> directories;

	directories.push(path);

	while (!directories.isEmpty())
	{
		size_t nestedCnt = 0;
		do
		{
			path = directories.top();
			spec = path + L"\\" + mask;
			
			directories.pop();
		
			hFind = FindFirstFile(spec.c_str(), &ffd);

		} while (hFind == INVALID_HANDLE_VALUE && !directories.isEmpty()); //skipping the files without permission
	
		
		fmi::Vector<FileType> helpVector;
			
		do
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L".."))
			{
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					directories.push(path + L"\\" + ffd.cFileName);
				}
				else
				{
					FileType file;
					file.isDirectory = false;
					file.path = path + L"\\" + ffd.cFileName;
					helpVector.push_back(file);
				}
				nestedCnt++;
			}

		} while (FindNextFile(hFind, &ffd) != 0);

		FileType dir;
		dir.isDirectory = true;
		dir.path = path;
		dir.nestedFilesCnt = nestedCnt;
		items.push_back(dir);

		for (size_t i = 0; i < helpVector.getSize(); i++)
		{
			items.push_back(helpVector[i]);
		}
			
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			std::cout << GetLastError() << hFind;
			FindClose(hFind);
			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}

fmi::Vector<FileType> getFiles(fmi::String toPack)
{
	fmi::Vector<FileType> files;
	wchar_t arr[1024];
	size_t retValue = 0;

	mbstowcs_s(&retValue, arr, 1024, toPack.c_str(), toPack.getLen());
	listFiles(arr, L"*", files);

	return files;
}


#endif //_SCANDIR_HEADER_INCLUDED_