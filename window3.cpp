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

extern ID2D1Factory1		*gD2D1Fac;
extern IDWriteFactory		*gDWriteFac;

IDWriteTextFormat *Dtext::gettf(Dtexti i) {
	IDWriteTextFormat	*ret;
	ret = dwtf[i];
	if(ret == nullptr) {
		ret = dwtf[0];
	}
	return ret;
}

Dtext::Dtexti Dtext::createformat(const wchar_t *fn, float size, bool italic, bool bold) {
	Dtexti		ret = 0;
	HRESULT	hr;
	IDWriteTextFormat	*tl = nullptr;
	hr = gDWriteFac->CreateTextFormat(fn, nullptr,
		(bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_MEDIUM),
		(italic?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL),
		DWRITE_FONT_STRETCH_NORMAL, size, fn, &tl);
	if(SUCCEEDED(hr)) {
		IDWriteTextFormat *ol;
		ol = dwtf[gmin];
		if(ol != nullptr) {
			ol->Release();
		}
		dwtf[gmin] = tl;
		ret = gmin;
		gmin++;
	}
	return ret;
}

Dtext::Dtext() {
	HRESULT	hr;
	IDWriteTextFormat	*tl = nullptr;
	hr = gDWriteFac->CreateTextFormat(L"Arial", nullptr,
		DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"Arial", &tl);
	if(SUCCEEDED(hr)) {
		dwtf[0] = tl;
	}
}

Dtext::~Dtext() {
	for(auto i = std::begin(dwtf); i != std::end(dwtf); i++) {
		if(i->second) i->second->Release();
	}

}

LRESULT Dwindow::wp(UINT msg, WPARAM wParam, LPARAM lParam) {
	HRESULT	hr;
	int	pX, pY;
	switch(msg) {
	case WM_CHAR:
		this->keyboardDispatch(gw(), msg, wParam, lParam);
		InvalidateRect(gw(), nullptr, false);
		return 0;
	case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_MOUSEMOVE:
		this->mouseDispatch(gw(), msg, wParam, lParam);
		InvalidateRect(gw(), nullptr, false);
		return 0;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(gw(), &ps);

		if(rT == nullptr) {
			create_rendertarget();
		}

		D2D1_SIZE_F dim = D2D1::SizeF(
			osx-ps.rcPaint.left, osy-ps.rcPaint.top
		);

		if(drt && rT) {
			drt->BindDC(ps.hdc, &ps.rcPaint);
			rT->BeginDraw();
			
			rT->Clear(D2D1::ColorF(D2D1::ColorF::LightGray));
			rT->SetTransform(
				D2D1::Matrix3x2F::Identity() *
				D2D1::Matrix3x2F::Translation(-ps.rcPaint.left+osx, -ps.rcPaint.top+osy)
			);
			this->paint(dim);

			hr = rT->EndDraw();
			if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
				destroy_rT();
				rT = nullptr;
			}
		}

		EndPaint(gw(), &ps);
	}
	return 0;
	}
	return Cwindow::wp(msg, wParam, lParam);
}

void Dcontext::paint() {

}
void Dcontext::paint(D2D1_SIZE_F dim) {
	this->paint();
}

void Dwindow::destroy_rT() {
	for(auto i = std::begin(bli); i != std::end(bli); i++) {
		if(i->second) i->second->Release();
	}
	if(rT) {
		rT->Release();
		drt = nullptr; rT = nullptr;
	}
	bli.clear();
}

void Dwindow::create_rendertarget() {
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	HRESULT		hr = S_OK;
	ID2D1DCRenderTarget	*prT = nullptr;
	hr = gD2D1Fac->CreateDCRenderTarget(&props, &prT);
	if(SUCCEEDED(hr)) {
		destroy_rT();
		rT = drt = prT;
	}
}

Dcontext::Dcontext() {
	rT = nullptr;
}

Dcontext::Dcontext(ID2D1RenderTarget *pr) {
	rT = pr;
}

Dcontext::~Dcontext() {

}


ID2D1SolidColorBrush *Dcontext::getbrush(short r, short g, short b, short a) {
	ID2D1SolidColorBrush	*rb = nullptr;
	uint64_t	co = 0;
	co += r; co <<= 16; co += g; co <<= 16; co += b; co <<=16 ; co += a;
	if(rT == nullptr) return nullptr;
	HRESULT hr = E_FAIL;
	rb = bli[co];
	if(rb == nullptr) {
		D2D1_COLOR_F	cof = D2D1::ColorF(((float)r)/255.0, ((float)g)/255.0, ((float)b)/255.0, ((float)a)/255.0);
		hr = rT->CreateSolidColorBrush(cof, &rb);
		if(SUCCEEDED(hr)) {
			bli[co] = rb;
		}
	}
	return rb;
}

void Dwindow::finish() {
	create_rendertarget();
}
Dwindow::Dwindow():Emouse()
{

}
Dwindow::~Dwindow() {
	destroy_rT();
}



