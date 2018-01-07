/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/


#include "stdafx.h"
#include "toys1app.h"
#include "whole1.h"
#include "rwhole1.h"

extern IWICImagingFactory	*gWICFac;

void ESbitmap::bmp_destroy() {
	StopTile();
	if(conv) conv->Release();
	conv = nullptr;
	if(bmp) bmp->Release();
	bmp = nullptr;
}


ESbitmap::ESbitmap() {
	
}

ESbitmap::~ESbitmap() {
	bmp_destroy();
}

IWICBitmap *ESbitmap::gb() {
	return bmp;
}

void ESbitmap::InitTile() {
	WICRect		rect;
	HRESULT		hr;

	if(tileinited == false) {
		if(lock == nullptr && bmp) {
			bmp->GetSize(&fw, &fh);
			rect.X = 0; rect.Y = 0;
			rect.Width = fw; rect.Height = fh;

			hr = bmp->Lock(&rect, WICBitmapLockRead, &lock);
			if(SUCCEEDED(hr)) {
				hr = lock->GetStride(&stride);
			}
			if(SUCCEEDED(hr)) {
				hr = lock->GetDataPointer(&size, &src);
				tileinited = true;
			}
		}
	}
}

void ESbitmap::StopTile() {
	if(lock) lock->Release();
	lock = nullptr;
	tileinited = false;
}

void ESbitmap::CopyTile(EACBbitmap *psdt, int scaling, POINT pfromwhere, uint32_t *cache, int item) {
	InitTile();
	if(tileinited) {
		if(scaling > 1) {
			psdt->CopyScale(scaling, pfromwhere, fw, fh, (BYTE*)cache, src, stride, item);
		} else {
			psdt->CopyBits(pfromwhere, fw, fh, src, stride, item);
		}
	}
}

HRESULT ESbitmap::initialize(IWICBitmapSource *ps) {
	if(ps == nullptr) return E_ABORT;
	bmp_destroy();
	bmp = nullptr;
	HRESULT	hr = gWICFac->CreateBitmapFromSource(ps, WICBitmapCacheOnLoad, &bmp);
	return hr;
}

HRESULT ESbitmap::initialize(uint32_t pw, uint32_t ph, WICPixelFormatGUID pp) {
	bmp_destroy();
	HRESULT	hr = gWICFac->CreateBitmap(pw, ph, pp, WICBitmapCacheOnLoad, &bmp);
	return hr;
}

/*
void ESbitmap::copymap(ID2D1Bitmap *dst, uint32_t px, uint32_t py, uint32_t pw, uint32_t ph) {
	if(bmp == nullptr) return;
	if(dst == nullptr) return;

	WICRect	r;
	r.X = px;		if(r.X != px) return;
	r.Y = py;		if(r.Y != py) return;
	r.Width = pw;	if(r.Width != pw) return;
	r.Height = ph;	if(r.Height != ph) return;

	D2D1_RECT_U	r2;
	r2.left = px;		if(r2.left != px) return;
	r2.top = py;		if(r2.top != ph) return;
	r2.right = px+pw;	if(r2.right != px+pw) return;
	r2.bottom = py+ph;	if(r2.bottom != py+ph) return;


	HRESULT	hr = S_OK;
	WICPixelFormatGUID	src_pxf;
	hr = bmp->GetPixelFormat(&src_pxf);
	if(FAILED(hr)) return;

	D2D1_PIXEL_FORMAT	dst_pxf;
	dst_pxf = dst->GetPixelFormat();



	if(SUCCEEDED(hr))
	if(conv == nullptr) {
		if(dst_pxf.format != DXGI_FORMAT_B8G8R8A8_UNORM) {
			return;
		}
		hr = gWICFac->CreateFormatConverter(&conv);
		if(hr != S_OK) {
			conv = nullptr;
			return;
		}
		hr = conv->Initialize(
			bmp,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom 
		);
	}
	if(SUCCEEDED(hr)) {
		bool	iscachenull = cache == nullptr;
		set_cache(pw, ph);

		conv->CopyPixels(&r, cache_stride, cache_size*sizeof(uint32_t), (BYTE*)cache);
		dst->CopyFromMemory(&r2, cache, cache_stride);


		if(cache_size > 2048*1024) iscachenull = true;
		if(iscachenull) cache_destroy();
	}
}*/

/*
HRESULT ESbitmap::setfromsource(IWICBitmapSource *ps) {
	HRESULT	hr = S_OK;

	if(SUCCEEDED(hr)) {
		if(bmp) bmp->Release();
		bmp = nullptr;
	}

	if(ps == nullptr) hr = E_ABORT;
	
	if(SUCCEEDED(hr)) {
		hr = gWICFac->CreateBitmapFromSource(ps, WICBitmapCacheOnLoad, &bmp);
	}
	//filter();

	return hr;
}*/
/*
HRESULT RCbitmap::copytobitmap(ID2D1Bitmap *dst, int x, int y) {
	HRESULT	hr = S_OK;

	D2D1_PIXEL_FORMAT	dst_pxf;

	if(dst == nullptr) hr = E_ABORT;

	if(SUCCEEDED(hr)) {
		dst_pxf = dst->GetPixelFormat();
	}
	if(SUCCEEDED(hr))
	if(conv == nullptr) {
		if(FAILED(gWICFac->CreateFormatConverter(&conv))) {
			conv = nullptr;
		} else {
			hr = conv->Initialize(
				bmp,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom 
			);
		}
	}
	if(conv == nullptr) hr = E_ABORT;

	if(SUCCEEDED(hr)) {
		hr = conv
	}

	return hr;
}*/
/*
void ESbitmap::filter() {
	if(bmp) {
		WICRect	r;
		r.X = 0;
		r.Y = 0;
		
		UINT	w = 0, h = 0;
		bmp->GetSize(&w, &h);

		r.Width = w;
		r.Height = h;

		HRESULT	hr;
		IWICBitmapLock	*lock = nullptr;
		hr = bmp->Lock(&r, WICBitmapLockWrite, &lock);
		if(SUCCEEDED(hr)) {
			UINT	ps = 0;
			BYTE	*p = nullptr;
			UINT	stride = 0;

			WICPixelFormatGUID		pxf;
			hr = lock->GetDataPointer(&ps, &p);
			if(SUCCEEDED(hr)) {
				hr = lock->GetStride(&stride);
			}
			if(SUCCEEDED(hr)) {
				hr = lock->GetSize(&w, &h);
			}
			if(SUCCEEDED(hr)) {
				hr = lock->GetPixelFormat(&pxf);
			}
			if(SUCCEEDED(hr)) {
				if(pxf == GUID_WICPixelFormat32bppPBGRA) {
					for(int y = 0; y < h; y++) {
						for(int x = 0; x < w; x++) {
							float	fx, fy, fz;
							fx = (p+(y*stride)+(x*4))[0];
							fy = (p+(y*stride)+(x*4))[1];
							fz = (p+(y*stride)+(x*4))[2];

							float	m;
							m = fx*fx;
							m+= fy*fy;
							m+= fz*fz;
							m = sqrt(m)/2.0;
							if(m > 0.0) {
								fx /= m;
								fy /= m;
								fz /= m;
							
								fx *= 255.0;
								fy *= 255.0;
								fz *= 255.0;
							}


							(p+(y*stride)+(x*4))[0] = fx;
							(p+(y*stride)+(x*4))[1] = fy;
							(p+(y*stride)+(x*4))[2] = fz;
						}
					}
				}
			}
			lock->Release();
		}
	}
}*/


