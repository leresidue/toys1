/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



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

	void CopyTile(EACBbitmap *pdst, int scaling, POINT pfromwhere, uint32_t *cache, int item);
	void StopTile();

	HRESULT initialize(IWICBitmapSource *ps);
	HRESULT initialize(uint32_t pw, uint32_t ph, WICPixelFormatGUID pp);

//	void copymap(ID2D1Bitmap *dst, uint32_t px, uint32_t py, uint32_t pw, uint32_t ph);

	//HRESULT setfromsource(IWICBitmapSource *ps);
	//HRESULT copytobitmap(ID2D1Bitmap *dst, int x = 0, int y = 0);
	//void filter();
};