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

void Ekeyboard::keyboardDispatch(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_CHAR:
		this->keyDown(wparam);
		break;
	}
}

Ekeyboard::Ekeyboard() {

}

Ekeyboard::~Ekeyboard() {

}

void Ekeyboard::keyDown(wchar_t pk) {

}

void Emouse::mouseDispatch(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	POINT	pt;
	pt.x = GET_X_LPARAM(lparam);
	pt.y = GET_Y_LPARAM(lparam);
	lwp = wparam;
	switch(msg) {
	case WM_LBUTTONDOWN:
		fpt = lpt = pt;
		SetCapture(wnd);
		SetFocus(wnd);
		this->mouseOn(pt.x, pt.y);
		left = true;
		break;
	case WM_LBUTTONUP:
		lpt = pt;
		this->mouseUp(pt.x, pt.y);
		ReleaseCapture();
		left = false;
		break;
	case WM_MOUSEMOVE:
		lpt = pt;
		if(left)
			this->mouseMv(pt.x, pt.y);
		break;
	}
}

Emouse::Emouse() {

}

Emouse::~Emouse() {

}

void Emouse::mouseOn(float px, float py) {

}

void Emouse::mouseUp(float px, float py) {
	
}

void Emouse::mouseMv(float px, float py) {

}