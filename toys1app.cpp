/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/

// toys1app.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "toys1app.h"

class ROOTwindow : public Cwindow, public VerbCommand {
public:
	HWND		szb = nullptr;
	XscrollWindow<ROOTwindow>		*sco = nullptr;
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
		case WM_DESTROY:	PostQuitMessage(0);	return 0;
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
	ROOTwindow	*mwi = Cwindow::createwindowex<ROOTwindow>(
	0, L"ALLÔ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(gI, MAKEINTRESOURCEW(IDC_TOYS1APP))
	);

	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->sco = Cwindow::createwindowex<XscrollWindow<ROOTwindow>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, mwi->gw(), nullptr
		); if(mwi->sco) {
			mwi->sco->finish();
		}

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
		
	}

	if(mwi) {


		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
		
	}
	return infiniteloop();
}
