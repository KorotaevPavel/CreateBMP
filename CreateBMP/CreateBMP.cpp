#include "stdafx.h"
// кроме stdafx.h тебе здесь ничто не нужно инклюдить

// используй типы из stdint.h
const uint32_t width = 1920;
const uint32_t height = 1080;
const uint16_t depth = 24;

void createBMP(uint8_t* bitmapBits, uint32_t width, uint32_t height, uint16_t depth, LPCTSTR fileName)
{
	uint32_t headersSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	uint32_t pixelDataSize = height * ((width * (depth / 8)));

	BITMAPFILEHEADER bmpFileHeader = {}; // пустых скобок достаточно
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfOffBits = headersSize;
	bmpFileHeader.bfSize = headersSize + pixelDataSize;

	BITMAPINFOHEADER bmpInfoHeader = {};
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = depth;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = height;
	bmpInfoHeader.biWidth = width;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = pixelDataSize;

	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // код спиздил?) С fopen'ом норм не получилось разобраться, а тут уже WinAPI =)
	if (!hFile)
		return;

	DWORD dwWritten = 0;
	WriteFile(hFile, &bmpFileHeader, sizeof(bmpFileHeader), &dwWritten, NULL);
	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
	WriteFile(hFile, bitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);

	CloseHandle(hFile);
}

int main()
{
	std::vector<uint8_t> buffer(width * 3 * height); // используй стандартные контейнеры, ну в самом-то деле
	const int radius = 300;

	uint8_t* pData = buffer.data();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			const bool isInside = (((height / 2 - i)*(height / 2 - i) + (width / 2 - j)*(width / 2 - j)) <= radius*radius);
			
			pData[0] = isInside ? (uint8_t)0		: 133;
			pData[1] = isInside ? (uint8_t)255		: 133;
			pData[2] = isInside ? (uint8_t)0		: 133;

			pData += 3;
		}
	}

	createBMP((uint8_t*)buffer.data(), width, height, depth, L"new_image.bmp");

	return 0;
}

