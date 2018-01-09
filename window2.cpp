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

void xScroll::update(int nx, int ny, HWND wnd) {

	POINT	pt1, pt2;
	pt1.x = osx;
	pt1.y = osy;

	osx = -nx;
	osy = -ny;

	pt2.x = osx;
	pt2.y = osy;
	if(wnd && ((pt2.x-pt1.x)!=0||(pt2.y-pt1.y)!=0)) {
		RECT	r;
		if(ScrollWindowEx(wnd, pt2.x-pt1.x, pt2.y-pt1.y, NULL, NULL, NULL, &r, SW_INVALIDATE)==NULLREGION) {
			GetClientRect(wnd, &r);
			//InvalidateRect(wnd, &r, TRUE);
		}
	}
}