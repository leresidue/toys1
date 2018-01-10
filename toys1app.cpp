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


// toys1app.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "resource.h"
#include "toys1app.h"

#include "whole1.h"

class CUSTOMZO : public ZOfile {
public:
	CUSTOMZO(IFileOpenDialog *fd):ZOfile(fd) {
		UINT	ix = get_filterIndex_from_extension(L"gif");
		if(ix) {
			fd->SetFileTypeIndex(ix);
		}
	}
};

class ESwindow : public Cwindow, public VerbCommand {
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
		case WM_MOUSEWHEEL: {
			int	keys	= GET_KEYSTATE_WPARAM(wParam);
			int	delta	= GET_WHEEL_DELTA_WPARAM(wParam);
			POINT	pt;	EAwindow	*ea;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			if(keys & MK_CONTROL &&
			sco && (ea = sco->gx())) {
				ScreenToClient(GetParent(sco->gx()->gw()), &pt);
				pt.x -= ea->osx;
				pt.y -= ea->osy;
				if(zoomw && delta != 0) {
					std::wstring	cc = L"addzoom ";
					if(delta > 0)	cc += std::to_wstring(1);
					else			cc += std::to_wstring(-1);
					cc += L" x " + std::to_wstring(pt.x);
					cc += L" y " + std::to_wstring(pt.y);
					zoomw->receive(&cc);
				}
			}
		} return 0;
		case WM_SIZE:
			if(sco) {
				SetWindowPos(sco->gw(), HWND_TOP, 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
			}
			if(szb) {
				SetWindowPos(szb, HWND_TOP,
				LOWORD(lParam)-GetSystemMetrics(SM_CXHTHUMB),
				HIWORD(lParam)-GetSystemMetrics(SM_CYVTHUMB),
				GetSystemMetrics(SM_CXHTHUMB),
				GetSystemMetrics(SM_CYVTHUMB),
				0);
			}
			return 0;
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);
			switch (wmId) {
			case ID_FICHIER_OUVRIRUNEIMAGE: {
				std::wstring n = L"file1.gif";
				ZOfile	*mfi = Cfile::fileopendialog<CUSTOMZO>(&n, gw());
				if(mfi) {
					mfi->getbitmap(&source);
					sco->gx()->set_bT(&source);
					sco->update();
					delete mfi;
				}}
				break;
			/*case ID_FILE_SAVE: {
				std::wstring n = L"";
				ZOfile	*mfi = Cfile::filesavedialog<ZOfile>(&n, gw());
				if(mfi) {
					mfi->setbitmap(&source);
					delete mfi;
				}}
				break;
			case ID_GENERATION_MANDELBROT:

				break;*/
			case IDM_EXIT:
				DestroyWindow(gw());
				break;
			default:
				return DefWindowProc(gw(), msg, wParam, lParam);
			}
		}
		return 0;
		//case WM_DESTROY:	PostQuitMessage(0);	return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
	
	void setzoomlevel(int pz, bool pfp = false, POINT pt = {0,0}) {
		EAwindow	*ea = sco->gx();
		
		if(pz == 0) pz = 1;
		if(pz == -1) pz = 1;
		if(ea && ea->eB && ea->eB->gb()) {
			int		oox, ooy;
			RECT	r;
			GetClientRect(ea->gw(), &r);
			if(pfp == false) {
				pt.x = r.right/2;
				pt.y = r.bottom/2;
				
				pt.x -= ea->osx;
				pt.y -= ea->osy;
			}

			oox = (pt.x+ea->osx);
			ooy = (pt.y+ea->osy);

			double	dx, dy;
			dx = ea->osw;
			dy = ea->osh;
			double	dpx = 0.0, dpy = 0.0;
			if(abs(dx) >= 1.0 && abs(dy) >= 1.0) {
				dpx = double(pt.x)/dx;
				dpy = double(pt.y)/dy;
			}

			UINT	fw, fh;
			ea->eB->gb()->GetSize(&fw, &fh);
			if(pz > 0) {
				dx = fw *= pz;
				dy = fh *= pz;
			} else if(pz < 0) {
				dx = fw /= (-pz);
				dy = fh /= (-pz);
			}
			dpx *= dx;
			dpy *= dy;
			
			ea->osw = fw;
			ea->osh = fh;
			ea->osx = -(dpx-oox);
			ea->osy = -(dpy-ooy);
			if(abs(ea->osx) > ea->osw) ea->osx = -ea->osw;
			if(abs(ea->osy) > ea->osh) ea->osy = -ea->osh;
			ea->cB.left = -ea->osx;
			ea->cB.top = -ea->osy;
			ea->cB.step_event_zoom_changed(pz);
			sco->update();
		}
	}
public:
	void toysanta() {
		toy	*nbzoom = nullptr;
		nbzoom = isVerb(L"setzoomlevel");
		if(nbzoom) {
			if(nbzoom->hasClaus && nbzoom->sled == toy::integer) {
				bool	k = false;
				POINT	pt = {0,0};
				toy *xt = getToy(L"x");
				toy *yt = getToy(L"y");
				if(xt && yt && xt->canNumber() && yt->canNumber()) {
					pt.x = xt->getInteger();
					pt.y = yt->getInteger();
					k = true;
				}
				setzoomlevel(nbzoom->claus.integer, k, pt);
			}
		}
	}

	ESbitmap					source;
	XscrollWindow<EAwindow>		*sco = nullptr;
	ZoomWidget					*zoomw = nullptr;
	HWND		szb = nullptr;
	ESwindow() {

	}
	~ESwindow() {
		
	}
};

class BeauPortrait : public Cwindow, public xScroll {
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		HRESULT	hr;
		switch(msg) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(gw(), &ps);
			RECT	r;
			GetClientRect(gw(), &r);
			FillRect(hdc, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
			Ellipse(hdc, 0, 0, r.right, r.bottom);
			EndPaint(gw(), &ps);
		} return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}

protected:
	friend class ZOfile;
	

public:
	void finish() {}
	~BeauPortrait() {}

	void update(int nx, int ny, HWND wnd) {
		xScroll::update(nx, ny, wnd);
	}
};

class ROOTwindow : public Cwindow, public VerbCommand {
public:
	HWND		szb = nullptr;
	XscrollWindow<BeauPortrait>		*sco = nullptr;
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);
			switch (wmId) {
			case IDM_EXIT:
				DestroyWindow(gw());
				break;
			default:
				return DefWindowProc(gw(), msg, wParam, lParam);
			}

		} return 0;
		case WM_SIZE:
			if(sco) {
				SetWindowPos(sco->gw(), HWND_TOP, 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
			}
			if(szb) {
				SetWindowPos(szb, HWND_TOP,
				LOWORD(lParam)-GetSystemMetrics(SM_CXHTHUMB),
				HIWORD(lParam)-GetSystemMetrics(SM_CYVTHUMB),
				GetSystemMetrics(SM_CXHTHUMB),
				GetSystemMetrics(SM_CYVTHUMB),
				0);
			}
			return 0;
		//case WM_DESTROY:	PostQuitMessage(0);	return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
public:
	void toysanta() {
		
	}

	~ROOTwindow() {
		
	}
};


int initall() {
	{ROOTwindow	*mwi = Cwindow::createwindowex<ROOTwindow>(
		0, L"ALLÔ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(gI, MAKEINTRESOURCEW(IDC_TOYS1APP))
	);
	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->sco = Cwindow::createwindowex<XscrollWindow<BeauPortrait>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, mwi->gw(), nullptr
		); if(mwi->sco) {
			mwi->sco->finish();
		}

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
	}}
	ESwindow	*mwi = Cwindow::createwindowex<ESwindow>(
	0, L"ALLÔ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(gI, MAKEINTRESOURCEW(IDC_TOYS1APP))
	);

	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->sco = Cwindow::createwindowex<XscrollWindow<EAwindow>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, mwi->gw(), nullptr
		); if(mwi->sco) {
			mwi->sco->finish();
			mwi->zoomw = Cwindow::createwindowex<ZoomWidget>(
				0, L"", WS_CHILD|WS_VISIBLE,
				0, r.bottom-17, 64, 17,
				mwi->sco->gw(), nullptr);

			if(mwi->zoomw != nullptr) {
				mwi->sco->setleft(mwi->zoomw->gw());
				mwi->zoomw->target = mwi;
			}
		}

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
		
		//InvalidateRect(mwi->gw(), NULL, false);
	}


	return infiniteloop();
}
