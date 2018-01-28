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



#pragma once

class xScroll {
public:
	int	osx = 0, osy = 0;
	int	osw, osh;
	virtual void update(int nx, int ny, HWND wnd = 0);
};

template <typename T>
class XscrollWindow : public Cwindow {
private:
	void createscrollbars() {
		RECT	r;
		int		iHThumb, iVThumb;
		GetClientRect(gw(), &r);
		iHThumb = GetSystemMetrics(SM_CXHTHUMB);
		iVThumb = GetSystemMetrics(SM_CYVTHUMB);

		oH = CreateWindow(L"SCROLLBAR",  NULL,
			WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
			r.left, r.bottom-iVThumb, r.right-iHThumb, iVThumb,
			gw(), 0, GetModuleHandle(NULL), NULL);

		oV = CreateWindow(L"SCROLLBAR",  NULL,
			WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN,
			r.right-iHThumb, r.top, iHThumb, r.bottom-iVThumb,
			gw(), 0, GetModuleHandle(NULL), NULL);
	}
	void resizescrollbars() {
		RECT	r;
		int		iHThumb, iVThumb;
		GetClientRect(gw(), &r);
		iHThumb = GetSystemMetrics(SM_CXHTHUMB);
		iVThumb = GetSystemMetrics(SM_CYVTHUMB);

		int	hpre, hpost;
		int vpre, vpost;

		if(oLC) {
			RECT	cr;
			GetClientRect(oLC, &cr);
			SetWindowPos(oLC, HWND_TOP,
				r.left, r.bottom-iVThumb,
				cr.right, iVThumb, 0);
			r.left = cr.right;
		}
		if(oTC) {
			RECT	cr;
			GetClientRect(oTC, &cr);
			SetWindowPos(oTC, HWND_TOP,
				r.right-iHThumb, r.top,
				iHThumb, cr.bottom, 0);
			r.top = cr.bottom;
		}

		hpre = r.left;
		hpost = (r.right-hpre);

		vpre = r.top;
		vpost = (r.bottom-vpre);



		if(oH) SetWindowPos(oH, HWND_BOTTOM,
				hpre, r.bottom-iVThumb, 
				hpost-iHThumb, iVThumb,
				0);
		if(oV) SetWindowPos(oV, HWND_BOTTOM,
				r.right-iHThumb, vpre,
				iHThumb, vpost-iVThumb,
				0);
	}

	void createwinframe() {
		RECT	r;
		GetClientRect(gw(), &r);
		oC = CreateWindowEx(
		0,
		CnA,
		nullptr,
		WS_CHILD|WS_VISIBLE,
		0,
		0,
		r.right-GetSystemMetrics(SM_CXHTHUMB), 
		r.bottom-GetSystemMetrics(SM_CYVTHUMB),
		gw(),
		0,
		gI,
		nullptr);
	}
	void resizewinframe() {
		RECT	r;
		GetClientRect(gw(), &r);
		if(oC) {
			SetWindowPos(oC, HWND_TOP,
			0, 0,
			r.right-GetSystemMetrics(SM_CXHTHUMB), 
			r.bottom-GetSystemMetrics(SM_CYVTHUMB),
			0);

			if(oX && oS) {
				GetClientRect(oC, &r);
				SetWindowPos(oX->gw(), 0, r.left, r.top, r.right, r.bottom, 0);
			}
		}
	}

	void createcanvas() {
		RECT	r;
		GetClientRect(gw(), &r);
		oX = Cwindow::createwindowex<T>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, 
		r.right-GetSystemMetrics(SM_CXHTHUMB), 
		r.bottom-GetSystemMetrics(SM_CYVTHUMB),
		oC, nullptr);
	}
	void resizecanvas() {
		adjustscrollbars(false);
		adjustcanvas();
	}

	void adjustscrollbars(bool ppos = true) {
		RECT	r, r2, r3, r4;
		GetClientRect(oX->gw(), &r);
		GetClientRect(oC, &r2);

		SetRect(&r3, 0, 0, 0, 0);
		if(oS == nullptr) {
			GetWindowRect(GetParent(oX->gw()), &r4);
			GetWindowRect(oX->gw(), &r3);
			OffsetRect(&r3, -r4.left, -r4.top);
		} else {
			r3.left = oS->osx;
			r3.top = oS->osy;
			r.right = oS->osw;
			r.bottom = oS->osh;
		}


		SCROLLINFO	info1, info2;

		ZeroMemory(&info1, sizeof(info1));
		info1.cbSize = sizeof(info1);
		info1.fMask = SIF_PAGE|SIF_RANGE|(ppos?SIF_POS:0);

		info1.nPos = -r3.left;
		info1.nMin = 0;
		info1.nMax = r.right-1;
		info1.nPage = r2.right;

		ZeroMemory(&info2, sizeof(info2));
		info2.cbSize = sizeof(info2);
		info2.fMask = SIF_PAGE|SIF_RANGE|(ppos?SIF_POS:0);

		info2.nPos = -r3.top;
		info2.nMin = 0;
		info2.nMax = r.bottom-1;
		info2.nPage = r2.bottom;

		SetScrollInfo(oH, SB_CTL, &info1, TRUE);
		SetScrollInfo(oV, SB_CTL, &info2, TRUE);
	}
	void actionscrollbar(UINT msg, WORD loword, HWND wnd) {
		HWND	scrollbar = 0;
		if(msg == WM_HSCROLL) scrollbar = oH;
		if(msg == WM_VSCROLL) scrollbar = oV;

		if(scrollbar) {
			int			sadd;
			SCROLLINFO	info;
			bool	adjust_canvas = true;
			ZeroMemory(&info, sizeof(info));
			info.cbSize = sizeof(info);
			info.fMask = SIF_TRACKPOS|SIF_POS|SIF_PAGE|SIF_RANGE;
			if(GetScrollInfo(scrollbar, SB_CTL, &info)) {
				sadd = info.nPos;
				switch(loword) {
					case SB_LEFT:		sadd += -12; break;
					case SB_LINELEFT:	sadd += -1; break;
					case SB_PAGELEFT:	sadd += -(int)info.nPage; break;

					case SB_RIGHT:		sadd += 12; break;
					case SB_LINERIGHT:	sadd += 1; break;
					case SB_PAGERIGHT:	sadd += info.nPage; break;

					case SB_THUMBPOSITION:
					case SB_THUMBTRACK:
						sadd = info.nTrackPos;
						break;
					default:
						adjust_canvas = false;
						break;

				}
				if(sadd != info.nPos) {
					ZeroMemory(&info, sizeof(info));
					info.cbSize = sizeof(info);
					info.fMask = SIF_POS;
					info.nPos = sadd;
					SetScrollInfo(scrollbar, SB_CTL, &info, TRUE);
					if(adjust_canvas) adjustcanvas();
				}
			}
		}
	}
	void adjustcanvas() {
		SCROLLINFO	info1, info2;

		ZeroMemory(&info1, sizeof(info1));
		info1.cbSize = sizeof(info1);
		info1.fMask = SIF_POS;

		ZeroMemory(&info2, sizeof(info2));
		info2.cbSize = sizeof(info2);
		info2.fMask = SIF_POS;

		if(GetScrollInfo(oH, SB_CTL, &info1) && GetScrollInfo(oV, SB_CTL, &info2)) {
			if(oS) {
				oS->update(info1.nPos, info2.nPos, oX->gw());
				/*POINT	pt1, pt2;
				pt1.x = oS->osx;
				pt1.y = oS->osy;

				oS->osx = -info1.nPos;
				oS->osy = -info2.nPos;

				pt2.x = oS->osx;
				pt2.y = oS->osy;

				oS->update();
				ScrollWindow(oX->gw(), pt2.x-pt1.x, pt2.y-pt1.y, NULL, NULL);*/
			} else {
				SetWindowPos(oX->gw(), 0, -(int)info1.nPos, -(int)info2.nPos, 0, 0, SWP_NOSIZE);
			}
		}
	}
protected:
	HWND	oH = nullptr, oV = nullptr;
	T		*oX = nullptr;
	xScroll	*oS = nullptr;
	HWND	oC = nullptr;
	HWND	oLC = nullptr, oTC = nullptr;

	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
		/*case WM_ERASEBKGND:
			{
				RECT	r, r2, r3;
				bool	r2valid = false;
				GetClientRect(gw(), &r);
				if(oX) {
					GetClientRect(oX->gw(), &r2);
					r2valid = true;
				} else {
					if(r2valid) {
						r3 = r;
						r3.left = r2.right;
						r3.bottom = r2.bottom;
						//FillRect((HDC)wParam, &r3, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
						r.top = r2.bottom;
					}
					FillRect((HDC)wParam, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
				}
			}
			return 1;*/
		case WM_CREATE:
			createscrollbars();
			createwinframe();
			createcanvas();
			return 0;
		case WM_SIZE:
			resizescrollbars();
			resizewinframe();
			resizecanvas();
			return 0;
		case WM_DESTROY:
			if(oX) delete oX;
			oX = nullptr;
			return 0;
		case WM_HSCROLL:
		case WM_VSCROLL:
			actionscrollbar(msg, LOWORD(wParam), (HWND)lParam);
			//adjustcanvas();
			return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
	
public:
	~XscrollWindow() {
		
	}

	void setleft(HWND pwin) {
		oLC = pwin;
		resizescrollbars();
	}
	void settop(HWND pwin) {
		oTC = pwin;
		resizescrollbars();
	}

	void finish() {
		if(oX) {
			oX->finish();
			oS = dynamic_cast<xScroll*>(oX);
		}
		update();
		//resizecanvas();
	}
	void update() {
		//resizecanvas();
		adjustscrollbars();
		adjustcanvas();
		if(oS) {
			InvalidateRect(oX->gw(), NULL, FALSE);
		}
	}
	T *gx() { return oX; }

	void invalidate() {
		if(oC) {
			RECT	r;
			GetClientRect(oC, &r);
			InvalidateRect(oC, &r, FALSE);
		}
	}
};