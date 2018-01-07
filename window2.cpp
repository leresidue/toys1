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