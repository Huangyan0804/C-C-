#include "Bitmap.h"


Bitmap::Bitmap(HDC hDC, LPTSTR szFileName)
  : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
	LoadByAPI(hDC, szFileName);
}

Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
  : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
	Create(hDC, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap()
{
	Free();
}


void Bitmap::Free()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}


RGBQUAD palette[256];

BOOL Bitmap::Load8(HDC hDC, LPTSTR szFileName)
{
	Free();

	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}

	BITMAPINFO bmi;	
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}

	int biheight = bmi.bmiHeader.biHeight;
	int bicount = bmi.bmiHeader.biBitCount;
	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;
	
	//读取调色板信息
	bval = ReadFile(hFile, palette, sizeof(RGBQUAD)*256, &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(RGBQUAD)*256))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}

	PBYTE pBitmapBits;
	m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_PAL_COLORS,  (VOID**)&pBitmapBits, NULL, 0);
	if( (m_hBitmap!=NULL) && (pBitmapBits!=NULL))
	{
		bval = ReadFile(hFile, pBitmapBits, bmi.bmiHeader.biSizeImage, &dwBytesRead, NULL);
		if(bval)
			return true;
	}

	Free();  
	return false;
}


BOOL Bitmap::Load24(HDC hDC, LPTSTR szFileName)
{
	Free();

	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}

	BITMAPINFO bmi;
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}

	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;

 
	PBYTE pBitmapBits;
	m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,  (VOID**)&pBitmapBits, NULL, 0);
	if( (m_hBitmap!=NULL) && (pBitmapBits!=NULL))
	{
		SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
		bval = ReadFile(hFile, pBitmapBits, bmi.bmiHeader.biSizeImage, &dwBytesRead, NULL);
		if(bval)
			return true;
	}
	
	Free();  
	return false;
}

BOOL Bitmap::LoadByAPI(HDC hDC, LPTSTR szFileName)
{
	Free();

	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}

	BITMAPINFO bmi;
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}

	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;

	m_hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0,0,LR_LOADFROMFILE);//|LR_CREATEDIBSECTION  加上该描述时自定义图片加载失败 。
	if(m_hBitmap == NULL)
		return false;
	
	return true;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
	m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);
	if (m_hBitmap == NULL)
		return false;

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	HDC hMemDC = CreateCompatibleDC(hDC);

	HBRUSH hBrush = CreateSolidBrush(crColor);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

	RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
	FillRect(hMemDC, &rcBitmap, hBrush);

	SelectObject(hMemDC, hOldBitmap);
	
	DeleteDC(hMemDC);
	DeleteObject(hBrush);

	return true;
}

void Bitmap::Draw(HDC hDC, int x, int y)
{
	if (m_hBitmap != NULL)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

		//对于8位位图，需要设置逻辑调色板
		SetDIBColorTable(hMemDC, 0, 256, palette);
		
		BitBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
}

void Bitmap::Draw(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor)
{
	if (m_hBitmap != NULL)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

		//对于8位位图，需要设置逻辑调色板
		SetDIBColorTable(hMemDC, 0, 256, palette);

		if (!bTrans)
			BitBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, SRCCOPY);

		else
			TransparentBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, m_iWidth, m_iHeight, crTransColor);

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}

}
