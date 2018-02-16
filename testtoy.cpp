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
#include "resource.h"
#include "toys1app.h"

#include "whole1.h"

const wchar_t	*MITLICENCE =
L"Permission is hereby granted, free of charge, to any person obtaining a copy "
L"of this software and associated documentation files (the \"Software\"), to deal "
L"in the Software without restriction, including without limitation the rights "
L"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
L"copies of the Software, and to permit persons to whom the Software is "
L"furnished to do so, subject to the following conditions:\n\n"
L"The above copyright notice and this permission notice shall be included in all "
L"copies or substantial portions of the Software.\n\n"
L"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
L"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
L"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
L"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
L"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
L"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";


template <class T>
class SIMPLEwin : public Cwindow {
public:
	HWND		szb = nullptr;
	XscrollWindow<T>		*sco = nullptr;
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
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
public:
	
	void finish() {
		RECT	r;
		GetClientRect(gw(), &r);

		sco = Cwindow::createwindowex<XscrollWindow<T>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, gw(), nullptr
		); if(sco) {
			sco->finish();
			
		}

		szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
	}
	~SIMPLEwin() {
		
	}
};

class SIMPLEwrite : public virtual Dcontext, public virtual Emouse, public virtual Ekeyboard {
protected:
	std::wstring	texa;
	IDWriteTextLayout	*laya = nullptr;
private:
	friend class ROOTwindow;
	friend class BeauPortrait;
	Dtext	fmt;
	Dtext::Dtexti	a120, a16, a10;

	void paintSelection() {
		if(selection.length > 0) {
			ID2D1Brush			*ibr = getbrush(128, 128, 255);
			IDWriteTextLayout	*itl = laya;

			if(ibr != nullptr && itl != nullptr) {
			
				rT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			
				UINT32	fac = 0, rfac;
				itl->HitTestTextRange(selection.startPosition, selection.length, 0, 0, NULL, 0, &fac); {
					DWRITE_HIT_TEST_METRICS	*lfac = new DWRITE_HIT_TEST_METRICS[fac];
					if(SUCCEEDED(itl->HitTestTextRange(selection.startPosition, selection.length, 0, 0, lfac, fac, &rfac))) {
						for(int i = 0; i < fac; i++) {
							D2D1_RECT_F	r;
							r.left = lfac[i].left;
							r.top = lfac[i].top;
							r.right = r.left+lfac[i].width;
							r.bottom = r.top+lfac[i].height;
							rT->FillRectangle(r, ibr);
						}
					}
					delete [] lfac;
				}
				rT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			}
		}
	}
	void paintCaret() {
		if(laya && timer_on) {
			DWRITE_HIT_TEST_METRICS		ra;
			FLOAT	X, Y;
			laya->HitTestTextPosition(selection.startPosition, true, &X, &Y, &ra);
			D2D1_RECT_F	r;
			r.left = ra.left; r.right = r.left+2;
			r.top = ra.top; r.bottom = r.top+ra.height;
			rT->FillRectangle(r, getbrush(0,0,0));
		}
	}
	void paint() {
		ID2D1Brush	*br;
		br = getbrush(255, 128, 64);
		HRESULT					hr = S_OK;
		
		if(br == nullptr) return;

		paintSelection();

		if(laya) rT->DrawTextLayout(D2D1::Point2F(0,0), laya, br);
		
		paintCaret();
	}
	
	void settext(const wchar_t *txt, size_t	len) {
		texa.assign(txt, len);
		if(laya) laya->Release(); laya = nullptr;
		gDWriteFac->CreateTextLayout(texa.c_str(), texa.size(), fmt.gettf(a16), 1024, 1024, &laya);
	}

	void initfonts() {
		a120 = fmt.createformat(L"Arial", 120.0f, true, false);
		a16 = fmt.createformat(L"Arial", 16.0f, false, true);
		a10 = fmt.createformat(L"Arial", 10.0f, false, false);
		settext(MITLICENCE, wcslen(MITLICENCE));
	}
protected:
	friend class ZOfile;
	
	DWRITE_TEXT_RANGE	selection;
	bool	timer_on = true;
public:
	SIMPLEwrite() {
		initfonts();
	}
	SIMPLEwrite(ID2D1RenderTarget *pr):Dcontext(pr) {
		initfonts();
	}
	void finish() {}
	~SIMPLEwrite() {
		if(laya) laya->Release(); laya = nullptr;
	}

	DWRITE_HIT_TEST_METRICS		fht, lht;
	BOOL						ftr, ltr, fin, lin;
	
	void keyDown(wchar_t pk) {
		std::wstring	str;
		str.assign(texa.c_str(), selection.startPosition);
		str += pk;
		str.append(texa.c_str()+selection.startPosition+selection.length, texa.size()-(selection.startPosition+selection.length));
		selection.startPosition++;
		selection.length = 0;
		settext(str.c_str(), str.size());
	}
	
	void doSelect() {
		DWRITE_TEXT_RANGE		tr;
		if(lht.textPosition < fht.textPosition) {
			tr.startPosition = lht.textPosition;
			tr.length = fht.textPosition-lht.textPosition;
		} else {
			tr.startPosition = fht.textPosition;
			tr.length = lht.textPosition-fht.textPosition;
		}
		selection = tr;
		timer_on = true;
	}
	void mouseOn(float px, float py) {
		HRESULT	hr;
		if(laya) {
			hr = laya->HitTestPoint(px, py, &ftr, &fin, &fht);

			if(fht.textPosition == texa.size()-1 && fin == false) {
				fht.textPosition = texa.size();
			}
		}
		lht = fht;
		doSelect();
	}
	void mouseMv(float px, float py) {
		
		HRESULT	hr;
		if(laya) {
			hr = laya->HitTestPoint(px, py, &ltr, &lin, &lht);
			if(lht.textPosition == texa.size()-1 && lin == false) {
				lht.textPosition = texa.size();
			}
		}
		doSelect();
	}
	void mouseUp(float px, float py) {
		
	}
};

class WRITEedit : public Dwindow, public SIMPLEwrite {
private:
protected:
	friend class ZOfile;
	UINT_PTR	caret_timer = 0;
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		if(msg == WM_SETFOCUS) {
			UINT	ms;
			ms = GetCaretBlinkTime();
			if(caret_timer == 0) {
				caret_timer = SetTimer(gw(), 36, ms, 0);
			}
		}
		if(msg == WM_TIMER) {
			if(wParam == 36) {
				if(timer_on == true) timer_on = false;
				else timer_on = true;
				InvalidateRect(gw(), 0, 0);
			}
		}
		if(msg == WM_KILLFOCUS) {
			if(caret_timer) {
				KillTimer(gw(), 36);
				caret_timer = 0;
			}
		}
		return Dwindow::wp(msg, wParam, lParam);
	}
public:
	WRITEedit() { osw = 2000; osh = 800;}
	void finish() {}
	~WRITEedit() {}

};

int initall() {
	SIMPLEwin<WRITEedit>	*mwi = Cwindow::createwindowex<SIMPLEwin<WRITEedit>>(
		0, L"WRITEedit example", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(gI, MAKEINTRESOURCEW(IDC_TOYS1APP))
	);
	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);
		mwi->finish();
	}


	return infiniteloop();
}