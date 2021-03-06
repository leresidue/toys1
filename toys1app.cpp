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


class BeauPortraitold : public Cwindow, public xScroll {
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
	~BeauPortraitold() {}
};



class BeauPortrait2 : public virtual Dcontext {
private:
	friend class ROOTwindow;
	friend class BeauPortrait;
	Dtext	fmt;
	Dtext::Dtexti	a120, a16, a10;
	void paint() {
		ID2D1Brush	*br;
		br = getbrush(255, 128, 64);
		HRESULT					hr = S_OK;
		
		if(br == nullptr) return;

		rT->DrawTextW(L"Hello, World!", 13, fmt.gettf(a120), D2D1::RectF(0, 0, 800, 800), br);
		
		std::wstring	infos;
		
		infos += L"Toy Santa Applicator "; infos += L"1.0.0"; infos += L"\n";
		rT->DrawTextW(infos.c_str(), infos.size(), fmt.gettf(a16), D2D1::RectF(12, 138, 800, 800), br);
		
		infos = L"Copyright (c) 2018 Frédérique Brisson-Lambert\n\n";
		infos +=	L"Permission is hereby granted, free of charge, to any person obtaining a copy "
					L"of this software and associated documentation files (the \"Software\"), to deal "
					L"in the Software without restriction, including without limitation the rights "
					L"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
					L"copies of the Software, and to permit persons to whom the Software is "
					L"furnished to do so, subject to the following conditions:\n\n";
		infos +=	L"The above copyright notice and this permission notice shall be included in all "
					L"copies or substantial portions of the Software.\n\n";
		infos +=	L"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
					L"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
					L"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
					L"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
					L"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
					L"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";
		rT->DrawTextW(infos.c_str(), infos.size(), fmt.gettf(a10), D2D1::RectF(12, 162, 620, 1200), br);

		if(br) rT->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(100.0, 100.0), 800.0, 340.0), br, 50.0);

	}

	void initfonts() {
		a120 = fmt.createformat(L"Arial", 120.0f, true, false);
		a16 = fmt.createformat(L"Arial", 16.0f, false, true);
		a10 = fmt.createformat(L"Arial", 10.0f, false, false);
	}
protected:
	friend class ZOfile;
	
	
public:
	BeauPortrait2() {
		initfonts();
	}
	BeauPortrait2(ID2D1RenderTarget *pr):Dcontext(pr) {
		initfonts();
	}
	void finish() {}
	~BeauPortrait2() {}
};

class BeauPortrait : public Dwindow, public BeauPortrait2 {
private:
protected:
	friend class ZOfile;
	

public:
	BeauPortrait() { osw = 2000; osh = 800;}
	void finish() {}
	~BeauPortrait() {}

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
			case ID_FICHIER_OUVRIRUNEIMAGE: {
				Dprint	*prt;
				ID2D1Device	*dev = nullptr;
				HRESULT hr = gD2D1Fac->CreateDevice(dxdev, &dev);
				prt = new Dprint(dev);
				if(prt->Setup(gw(), nullptr, true)) {
					if(prt->Print()) {
						if(prt->Start(L"BeauPortrait2paint")) {

							ID2D1CommandList	*commandList = nullptr;
							HRESULT hr = prt->dev->CreateCommandList(&commandList);
							prt->dev->SetTarget(commandList);
							prt->dev->SetDpi(prt->m_dpiw, prt->m_dpih);

							prt->dev->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
							prt->dev->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
							

							prt->dev->BeginDraw();
							
							BeauPortrait2		*prr = new BeauPortrait2(prt->dev);
							prr->paint();
							delete prr;

							prt->dev->EndDraw();


							commandList->Close();
							prt->AddPage(commandList);
							commandList->Release();

							prt->Stop();
						}
						
					}

				}
				delete prt;
			} break;
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

		mwi->sco = Cwindow::createwindowex<XscrollWindow<BeauPortrait>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, mwi->gw(), nullptr
		); if(mwi->sco) {
			mwi->sco->finish();
			
		}

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
	}


	return infiniteloop();
}
