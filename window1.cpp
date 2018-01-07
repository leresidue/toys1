/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



#include "stdafx.h"

extern HINSTANCE	gI;



ATOM	Cwindow::CwA = 0;
const wchar_t	*Cwindow::CnA = L"Cwindow";

LRESULT	Cwindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Cwindow		*pW = nullptr;
	if(message == WM_NCCREATE) {
		pW = (Cwindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		if(pW) pW->oW = hWnd;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pW);
	} else if(message == WM_NCDESTROY) {
		pW = (Cwindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(pW) {
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)0);
			delete pW;
		}
	} else {
		pW = (Cwindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(pW) {
			return pW->wp(message, wParam, lParam);
		} else {
			if(message == WM_ERASEBKGND) {
				RECT	r;
				HWND	cw = GetWindow(hWnd, GW_CHILD);
				if(cw) {
					GetClientRect(cw, &r);
					MapWindowPoints(cw, hWnd, (LPPOINT)&r, 2);
					ExcludeClipRect((HDC)wParam, r.left, r.top, r.right, r.bottom);
				}
				GetClientRect(hWnd, &r);
				FillRect((HDC)wParam, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
				
				return 1;
			}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Cwindow::wp(UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(oW, &ps);
			EndPaint(oW, &ps);
		}
		return 0;
	}

	return DefWindowProc(oW, message, wParam, lParam);
}

Cwindow::Cwindow() {
	oW = 0;
}

void Cwindow::finish() {
	
}

Cwindow::~Cwindow() {
	if(oW) {
		if((Cwindow*)GetWindowLongPtr(oW, GWLP_USERDATA) == this) {
			SetWindowLongPtr(oW, GWLP_USERDATA, (LONG_PTR)0);
		}
	}
}