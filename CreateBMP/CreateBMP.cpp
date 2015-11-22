// CreateBMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <algorithm>
#include <memory>

const LONG width = 1920;
const LONG height = 1080;
const WORD depth = 24;

void createBMP(BYTE* bitmapBits, LONG width, LONG height, WORD depth, LPCTSTR fileName)
{
	unsigned long headersSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	unsigned long pixelDataSize = height * ((width * (depth / 8)));

	BITMAPFILEHEADER bmpFileHeader = { 0 };
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfOffBits = headersSize;
	bmpFileHeader.bfSize = headersSize + pixelDataSize;

	BITMAPINFOHEADER bmpInfoHeader = { 0 };
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = depth;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = pixelDataSize;

	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
		return;

	DWORD dwWritten = 0;
	WriteFile(hFile, &bmpFileHeader, sizeof(bmpFileHeader), &dwWritten, NULL);
	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
	WriteFile(hFile, bitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);

	CloseHandle(hFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	BYTE* buffer = new BYTE[width * 3 * height];
	int c = 0;
	int radius = 300;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (((height / 2 - i)*(height / 2 - i) + (width / 2 - j)*(width / 2 - j)) <= radius*radius)
			{
				buffer[c + 0] = (BYTE)0;
				buffer[c + 1] = (BYTE)255;
				buffer[c + 2] = (BYTE)0;

				c += 3;
			}
			else
			{
				buffer[c + 0] = (BYTE)133;   
				buffer[c + 1] = (BYTE)133;   
				buffer[c + 2] = (BYTE)133;   

				c += 3;
			}
		}
	}

	createBMP((BYTE*)buffer, width, height, depth, L"new_image.bmp");

	delete[] buffer;
	return 0;
}

