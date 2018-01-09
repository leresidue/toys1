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

extern HINSTANCE	gI;



ATOM	Cwindow::CwA = 0;
INT		Cwindow::CwC = 0;
const wchar_t	*Cwindow::CnA = L"Cwindow";

LRESULT	Cwindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Cwindow		*pW = nullptr;
	if(message == WM_NCCREATE) {
		pW = (Cwindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		if(pW) pW->oW = hWnd;
		CwC++;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pW);
	} else if(message == WM_NCDESTROY) {
		CwC--;
		pW = (Cwindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(pW) {
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)0);
			delete pW;
		}
		if(CwC == 0) PostQuitMessage(0);
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