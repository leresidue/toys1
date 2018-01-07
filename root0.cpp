/* 2018 leresidue */

#include "stdafx.h"

#include "whole.h"

HINSTANCE			gI;
ID2D1Factory		*gD2D1Fac = nullptr;

void Cwindow::registerclassex() {
	if(CwA == 0) {
		WNDCLASSEXW		wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		
		wcex.style          = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc    = WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = gI;
		wcex.hIcon          = LoadIcon(gI, MAKEINTRESOURCE(???));
		wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground  = 0;//(HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName   = NULL;//MAKEINTRESOURCEW(???);
		wcex.lpszClassName  = CnA;
		wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		CwA =  RegisterClassExW(&wcex);
	}
}

class ROOTwindow : public Cwindow, public VerbCommand {
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
			return 0;
		case WM_SIZE:
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
		return 0;
		case WM_DESTROY:	PostQuitMessage(0);	return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
public:
	void toysanta() {
		
	}

	HWND		szb = nullptr;
	~ROOTwindow() {
		
	}
};



int APIENTRY wWinMain(HINSTANCE pI, HINSTANCE pPI, LPWSTR pCL, int pCS) {
	
	gI = pI;

	HACCEL	fAT = LoadAccelerators(pI, MAKEINTRESOURCE(???));
	MSG		fMs;

	/////////////

	if(SUCCEEDED(CoInitialize(NULL)) == 0) return 0;
	
	HRESULT	hr;
	

	/*hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(&gD2D1Fac));
	if(SUCCEEDED(hr) == 0) return 0;*/


		/////////////

	Cwindow::registerclassex();

			/////////////


	
	ROOTwindow	*mwi = Cwindow::createwindowex<ROOTwindow>(
	0, L"ALLÔ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(pI, MAKEINTRESOURCEW(????))
	);

	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
		
	}

			/////////////

	while(GetMessage(&fMs, nullptr, 0, 0)) {
		if(!TranslateAccelerator(fMs.hwnd, fAT, &fMs)) {
			TranslateMessage(&fMs);
			DispatchMessage(&fMs);
		}
	}

		/////////////

	//if(gD2D1Fac)	gD2D1Fac->Release();
	CoUninitialize();

	/////////////

	return (int) fMs.wParam;
}
