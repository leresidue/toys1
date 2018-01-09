/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/

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

#include "stdafx.h"
#include "toys1app.h"

#include "whole1.h"
#include "rwhole1.h"

using namespace std;

EACBbitmap::EACBbitmap(ID2D1RenderTarget *rT) {
	bool	used = true;
	if(rT) {
		HRESULT	hr;
		D2D1_BITMAP_PROPERTIES	p;
		p.pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE);
		p.dpiX = p.dpiY = 96.0;
		hr = rT->CreateBitmap(D2D1::SizeU(1024, 1024), p, &bmp);
		used = false;
	}
	for(int i = 0; i < 16; i++) {
		ptu[i] = false;
		ptc[i] = false;
	}
}

EACBbitmap::~EACBbitmap() {
	if(bmp) bmp->Release();
}

void EACBbitmap::CopyScale(int scale, POINT orig, UINT pw, UINT ph, BYTE *cache, BYTE *src, UINT32 stride, int item) {
	if(orig.y > ph || orig.x > pw) return;
	if(scale < 1) return;

	uint32_t	*fsrc, *fdst;


	for(int y = orig.y; y < orig.y+256; y++) {
		fdst = (uint32_t*)cache;
		fdst += (y-orig.y)*256;

		fsrc = (uint32_t*)src;
		fsrc += y*scale*(stride/4);
		fsrc += orig.x*scale;
		if(y >= ph/scale) break;
		for(int x = orig.x; x < orig.x+256; x++) {
			if(x >= pw/scale) break;
			*fdst++ = *fsrc;
			fsrc+=scale;
		}
	}

	D2D1_RECT_U	itr;

	itr.top = item/4;			itr.left = item%4;
	itr.top *= 256;				itr.left *= 256;
	itr.bottom = itr.top+256;	itr.right = itr.left+256;

	if(itr.right-itr.left > pw-orig.x) itr.right = itr.left+(pw-orig.x);
	if(itr.bottom-itr.top > ph-orig.y) itr.bottom = itr.top+(ph-orig.y);

	if(bmp)
		bmp->CopyFromMemory(&itr, cache, sizeof(uint32_t)*256);
}

void EACBbitmap::CopyFrom(POINT orig, UINT pw, UINT ph, BYTE *cache, IWICBitmapSource *src, int item) {
	if(cache == nullptr) return;

	WICRect		ctr;
	ctr.X = orig.x;
	ctr.Y = orig.y;
	ctr.Width = 256;
	ctr.Height = 256;
	if(ctr.X+256 > pw) ctr.Width = pw-orig.x;
	if(ctr.Y+256 > ph) ctr.Height = ph-orig.y;
	src->CopyPixels(&ctr, sizeof(uint32_t)*256, sizeof(uint32_t)*256*256, cache);

	D2D1_RECT_U	itr;

	itr.top = item/4;			itr.left = item%4;
	itr.top *= 256;				itr.left *= 256;
	itr.bottom = itr.top+256;	itr.right = itr.left+256;

	if(itr.right-itr.left > pw-orig.x) itr.right = itr.left+(pw-orig.x);
	if(itr.bottom-itr.top > ph-orig.y) itr.bottom = itr.top+(ph-orig.y);

	if(bmp) {
		bmp->CopyFromMemory(&itr, cache, sizeof(uint32_t)*256);
	}
}

void EACBbitmap::CopyBits(POINT orig, UINT pw, UINT ph, BYTE *src, UINT32 stride, int item) {
	if(orig.y > ph || orig.x > pw) return;

	D2D1_RECT_U	itr;

	itr.top = item/4;			itr.left = item%4;
	itr.top *= 256;				itr.left *= 256;
	itr.bottom = itr.top+256;	itr.right = itr.left+256;

	if(itr.right-itr.left > pw-orig.x) itr.right = itr.left+(pw-orig.x);
	if(itr.bottom-itr.top > ph-orig.y) itr.bottom = itr.top+(ph-orig.y);

	if(bmp)
		bmp->CopyFromMemory(&itr, src+(orig.y*stride)+(orig.x*sizeof(uint32_t)), stride);
}

EACBrect::EACBrect(const EACBrect & r)
{
	this->rect = r.rect;
	this->bitmap = r.bitmap;
	this->item = r.item;
}

EACBrect::EACBrect(RECT r, EACBbitmap *b, int i) {
	rect = r; bitmap = b; item = i;
}

bool EACBrect::operator==(const RECT &b) {
	if(item > 15 || item < 0) {
		return false;
	}
	if(memcmp(&rect, &b, sizeof(RECT))==0) return true;
	return false;
}

bool EACBrect::operator>(const RECT &b) {
	if(item > 15 || item < 0) {
		return false;
	}
	if(
		(rect.left >= b.right || rect.right <= b.left)
		||
		(rect.top >= b.bottom || rect.bottom <= b.top)
		) {
		return true;
	}

	return false;
}

EAcachebmp::EAcachebmp() {

}

EAcachebmp::~EAcachebmp() {
	flush();
	if(cache) delete [] cache;
}

void EAcachebmp::clear() {
	zones.clear();
	EACBbitmap	*item = list;
	while(item) {
		for(int i = 0; i < 16; i++) {
			item->ptc[i] = false;
			item->ptu[i] = false;
		}
		item = item->next;
	}
}

void EAcachebmp::flush() {
	zones.clear();
	EACBbitmap	*it;
	while(list) {
		it = list->next;
		delete list;
		list = it;
	}
}

void EAcachebmp::add(RECT r) {
	EACBbitmap	*it;
	int			item = 16;
	it = list;
	while(it) {
		for(int i = 0; i < 16; i++) {
			if(it->ptu[i] == false) {
				item = i;
				break;
			}
		}
		if(item < 16) break;
		it = it->next;
	}
	if(item == 16) {
		it = new EACBbitmap(rT);
		it->next = list;
		list = it;
		item = 0;
	}
	if(it && item < 16) {
		it->ptu[item] = true;
		it->ptc[item] = true;
		zones.push_back(EACBrect(r, it, item));
	}
}

void EAcachebmp::update_view(int px, int py, int pw, int ph) {
	if(cdim == 0) return;
	left = px; top = py;
	width = pw; height = ph;

	int	twidth = eswidth;
	int theight = esheight;
	if(usescaler && scaling > 0) {
		twidth /= scaling;
		theight /= scaling;
	} else {
		twidth *= cdim/256;
		theight *= cdim/256;
	}
	if(width > twidth) width = twidth;
	if(height > theight) height = theight;

	long	addx, addy;
	addx = left-(left%cdim);
	addy = top-(top%cdim);

	RECT	vr;
	vr.left = addx;
	vr.top = addy;

	long	addw, addh;
	addw = width-(width%cdim);
	addh = height-(height%cdim);

	if(addw != width && width != twidth)
		addw+=cdim;
	if(addh != height && height != theight)
		addh+=cdim;


	vr.right = addx+addw;
	vr.bottom = addy+addh;

	int i = 0, i_end;
	i_end = zones.size();
	while(i < i_end) {
		if(zones[i] > vr) {
			if(zones[i].bitmap) {
				if(zones[i].item >= 0 && zones[i].item < 16) {
					zones[i].bitmap->ptc[zones[i].item] = false;
					zones[i].bitmap->ptu[zones[i].item] = false;
				}
			}
			zones.erase(zones.begin()+i);
			i_end--;
		} else {
			i++;
		}
	}

	for(uint32_t y = 0; y <= addh; y+=cdim) {
		for(uint32_t x = 0; x <= addw; x+=cdim) {
			RECT	r;
			r.left = x+addx;
			r.top = y+addy;
			r.right = x+addx+cdim;
			r.bottom = y+addy+cdim;
			bool	invalid = true;

			i = 0; i_end = zones.size();
			while(i < i_end && invalid == true) {
				if(zones[i] == r) {
					invalid = false;
				}
				i++;
			}
			if(invalid) {
				add(r);
			}
		}
	}
}

void EAcachebmp::DrawBitmap(ESbitmap *eB, D2D1_RECT_F const &rrr, float pf) {
	HRESULT				hr;
	UINT				fw, fh;
	IWICBitmapSource	*fbm;
	
	fbm = eB->gb();
	if(fbm == nullptr) return;
	hr = fbm->GetSize(&fw, &fh);
	if(FAILED(hr)) return;

	WICRect			rect;
	rect.X = rect.Y = 0;
	rect.Width = fw;
	rect.Height = fh;

	D2D1_RECT_F	cr;
	cr.left = left;
	cr.top = top;
	cr.right = left+width;
	cr.bottom = top+height;

	rT->PushAxisAlignedClip(cr, D2D1_ANTIALIAS_MODE_ALIASED);

	int	i, i_end;
	i = 0; i_end = zones.size();
	for(; i < i_end; i++) {
		if(zones[i].item >= 0 && zones[i].item < 16) {
			if(zones[i].bitmap != nullptr) {
				if(zones[i].bitmap->ptc[zones[i].item] == true) {
					zones[i].bitmap->ptc[zones[i].item] = false;

					POINT	tpr;
					if(usescaler) {
						tpr.x = zones[i].rect.left;
						tpr.y = zones[i].rect.top;
					} else {
						int		scd = 1;
						if((cdim/256)) {
							scd = cdim/256;
						}
						tpr.x = zones[i].rect.left/scd;
						tpr.y = zones[i].rect.top/scd;
					}
					if(cache == nullptr) {
						cache = new(std::nothrow) uint32_t[256*256];
					}
					if(cache) eB->CopyTile(zones[i].bitmap, usescaler?scaling:1, tpr, cache, zones[i].item);
				}

				D2D1_RECT_F	r1, r2;
				r1.left = zones[i].rect.left;
				r1.top = zones[i].rect.top;
				r1.right = zones[i].rect.right;
				r1.bottom = zones[i].rect.bottom;

				r2.left = (zones[i].item%4)*256;
				r2.top = (zones[i].item/4)*256;
				r2.right = r2.left+256;
				r2.bottom = r2.top+256;

				rT->DrawBitmap(
					zones[i].bitmap->bmp, r1, pf,
					D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
					r2);
			}
		}
	}
	eB->StopTile();
	
	rT->PopAxisAlignedClip();
}

void EAcachebmp::event_rendertarget_changed(ID2D1RenderTarget *ptr) {
	flush();
	rT = ptr;
}
void EAcachebmp::test_window_redimensioned_and_retrieve_bitmap_size(HWND pwnd, ESbitmap *pLimit) {
	esisvalid = false;
	RECT	r;
	GetClientRect(pwnd, &r);
	if(pLimit) {
		IWICBitmap	*gb;
		gb = pLimit->gb();
		if(gb) {
			HRESULT	hr;
			UINT	fw, fh;
			hr = gb->GetSize(&fw, &fh);
			if(SUCCEEDED(hr)) {
				esisvalid = true;
				eswidth = fw;
				esheight = fh;
			}
		}
	} else {
		r.right = 0;
		r.bottom = 0;
	}
	if(r.right != width || r.bottom != height) {
		event_size_changed(r.right, r.bottom);
	}
}
void EAcachebmp::event_size_changed(uint32_t pwidth, uint32_t pheight) {
	update_view(left, top, pwidth, pheight);
}
void EAcachebmp::event_position_changed(int px, int py) {
	update_view(px, py, width, height);
}
void EAcachebmp::step_event_zoom_changed(int pz) {
	clear();

	if(pz < 0) {
		cdim = -pz;
		if(cdim > 0) {
			cdim = 256;
			usescaler = true;
			scaling = -pz;
		}
	}

	if(pz == 0) pz = 1;

	if(pz > 0) {
		cdim = pz*256;
		usescaler = false;
		scaling = 0;
	}
	in_update_function_set_wnd_to_NULL = true;
}
