/***************************************************************************************

	Copyright (c) 2018 Frédérique Brisson-Lambert

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

***************************************************************************************/


#pragma once

class EACBbitmap;
class ESbitmap {
protected:
	IWICFormatConverter	*conv = nullptr;
	IWICBitmap			*bmp = nullptr;

	IWICBitmapLock	*lock = nullptr;
	UINT			size, stride;
	BYTE			*src = nullptr;
	void bmp_destroy();

	void InitTile();
	bool tileinited = false;
	UINT			fw, fh;
public:
	
	ESbitmap();
	~ESbitmap();

	IWICBitmap *gb();

	int getpointer(BYTE **psrc, UINT *psize, UINT *pstride);
	int releasepointer(BYTE **psrc, UINT *psize, UINT *pstride);

	void CopyTile(EACBbitmap *pdst, int scaling, POINT pfromwhere, uint32_t *cache, int item);
	void StopTile();

	HRESULT initialize(IWICBitmapSource *ps);
	HRESULT initialize(uint32_t pw, uint32_t ph, WICPixelFormatGUID pp);

//	void copymap(ID2D1Bitmap *dst, uint32_t px, uint32_t py, uint32_t pw, uint32_t ph);

	//HRESULT setfromsource(IWICBitmapSource *ps);
	//HRESULT copytobitmap(ID2D1Bitmap *dst, int x = 0, int y = 0);
	//void filter();
};