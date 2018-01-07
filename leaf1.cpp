/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/


#include "stdafx.h"
#include "toys1app.h"

#include "whole1.h"


////

LRESULT EAwindow::wp(UINT msg, WPARAM wParam, LPARAM lParam) {
	HRESULT	hr;
	switch(msg) {
	case WM_PAINT:
    if(d2mode == false) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(gw(), &ps);
        RECT	r;
		GetClientRect(gw(), &r);
		FillRect(hdc, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		Ellipse(hdc, 0, 0, r.right, r.bottom);
        EndPaint(gw(), &ps);
    } else {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(gw(), &ps);

		if(rT == nullptr) {
			create_rendertarget();
			cB.event_rendertarget_changed(rT);
		}
		cB.test_window_redimensioned_and_retrieve_bitmap_size(gw(), eB);

		D2D1_SIZE_F dim = D2D1::SizeF(
			osx-ps.rcPaint.left, osy-ps.rcPaint.top
		);

		if(rT) {
			rT->BindDC(ps.hdc, &ps.rcPaint);
			rT->BeginDraw();
			
			rT->Clear(D2D1::ColorF(D2D1::ColorF::LightGray));
			if(eB) {

				rT->SetTransform(D2D1::IdentityMatrix()*
				D2D1::Matrix3x2F::Translation(dim));

				D2D1_RECT_F	rrr = D2D1::RectF(0, 0, osw, osh);
				if(eB) cB.DrawBitmap(eB, rrr, 1.0);
			}

			hr = rT->EndDraw();
			if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
				rT->Release();
				rT = nullptr;
			}
		}

		EndPaint(gw(), &ps);
	}
	return 0;
	/*case WM_GETMINMAXINFO:
	{
		MINMAXINFO	*mmi = (MINMAXINFO*)lParam;
		mmi->ptMaxPosition.x = 0x7fffffff;
		mmi->ptMaxPosition.y = 0x7fffffff;
	}
	return 0;
	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS	*pos = (WINDOWPOS*)lParam;
		if(pos) {
			if(customsetwindowpos) {
				customsetwindowpos = false;
				pos->x = custompos.left;
				pos->y = custompos.top;
				pos->cx = custompos.right;
				pos->cy = custompos.bottom;
			}
		}
	
	return 0;*/
	}
	return Cwindow::wp(msg, wParam, lParam);
}

void EAwindow::create_rendertarget() {
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
		if(rT) rT->Release();
		rT = prT;
		cB.rT = rT;
	}
}

void EAwindow::set_bT(ESbitmap *src) {
	//if(bT) bT->Release();
	//bT = nullptr;{
		//if(eB) delete eB;
		eB = src;
		
		cB.clear();
		cB.left = 0;
		cB.top = 0;
		osx = osy = 0;
	//if(SUCCEEDED(rB.setfromsource(pconv))) {
		/*if(rT) {
			IWICBitmap	*bmp = src->gb();
			if(bmp) rT->CreateBitmapFromWicBitmap(bmp, &bT);
		}
		if(bT) osw = bT->GetSize().width;
		if(bT) osh = bT->GetSize().height;*/
		UINT	fw, fh;
		src->gb()->GetSize(&fw, &fh);
		osw = fw; osh = fh;
		//if(bT) ::SetWindowPos(gw(), 0, 0, 0, bT->GetSize().width, bT->GetSize().height, SWP_NOREPOSITION);
	//}
}

void EAwindow::update(int nx, int ny, HWND wnd) {
	if(cB.in_update_function_set_wnd_to_NULL) {
		wnd = NULL;
		cB.in_update_function_set_wnd_to_NULL = false;
	}
	cB.event_position_changed(nx, ny);
	xScroll::update(nx, ny, wnd);
}

void EAwindow::finish() {
	create_rendertarget();
}
EAwindow::EAwindow()
{
}
EAwindow::~EAwindow() {
	//if(bT) bT->Release();
	if(rT) rT->Release();
}
/*
void EAwindow::SetWindowPos(int x, int y, int cx, int cy) {
	customsetwindowpos = true;
	SetRect(&custompos, x, y, cx, cy);
	::SetWindowPos(gw(), 0, x, y, cx, cy, 0);
}*/

/////////////////


bool ZoomWidget::setzoom(int z) {
	int	trealz = realZ;
	realZ = z;
	//if(realZ < -10) realZ = -10;
	//if(realZ > 10) realZ = 10;
	int	tzpos = zPos;
	zPos = 32+(realZ*3);
	if(trealz == realZ) return false;
	if(tzpos == zPos) return false;
	return true;
}

void ZoomWidget::sendzoom_to_target(bool k, POINT pt) {
	if(target) {
		std::wstring	cc = L"setzoomlevel ";
		int		zoom = realZ;//(zPos-32)/3;
		cc += std::to_wstring(zoom);
		if(k) {
			cc += L" x " + std::to_wstring(pt.x);
			cc += L" y " + std::to_wstring(pt.y);
		}
		target->receive(&cc);
	}
}

LRESULT ZoomWidget::wp(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_LBUTTONDOWN:
		SetCapture(gw());
	case WM_MOUSEMOVE:
		if(wParam & MK_LBUTTON) {
			int	z = GET_X_LPARAM(lParam);
			z -= 32;
			z /= 3;
			if(setzoom(z)) {
				InvalidateRect(gw(), NULL, TRUE);
				sendzoom_to_target();
			}
		}
		return 0;
		//break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;
	case WM_PAINT:  {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(gw(), &ps);
		RECT	r;
		GetClientRect(gw(), &r);
		FillRect(hdc, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		SetDCPenColor(hdc, RGB(255, 255, 255));
		SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));

		RECT	rr;
		SetRect(&rr, zPos-6, 0, zPos+6, r.bottom);
		FillRect(hdc, &rr, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
		rr.left++;rr.top+=2;rr.right--;rr.bottom--;
		FillRect(hdc, &rr, (HBRUSH)GetStockObject(GRAY_BRUSH));

		

		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, r.right, 0);
		MoveToEx(hdc, r.right/2, 0, NULL);

		if(rr.left <= r.right/2 && rr.right > r.right/2)
			SetDCPenColor(hdc, RGB(192, 192, 192));
		LineTo(hdc, r.right/2, r.bottom);
		EndPaint(gw(), &ps);
	} return 0;
	}
	return Cwindow::wp(msg, wParam, lParam);
}

void ZoomWidget::toysanta() {
	toy	*nbzoom = nullptr;
	nbzoom = isVerb(L"addzoom");
	if(nbzoom && nbzoom->canNumber()) {
		setzoom(realZ + nbzoom->getInteger());
		
		InvalidateRect(gw(), NULL, TRUE);
		bool	k = false;
		POINT	pt = {0,0};
		toy *xt = getToy(L"x");
		toy *yt = getToy(L"y");
		if(xt && yt && xt->canNumber() && yt->canNumber()) {
			pt.x = xt->getInteger();
			pt.y = yt->getInteger();
			k = true;
		}
		sendzoom_to_target(k, pt);
		
	}
}
