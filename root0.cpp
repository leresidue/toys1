#include "stdafx.h"

#include "resource.h"

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
		wcex.hIcon          = LoadIcon(gI, MAKEINTRESOURCE(IDC_TOYS1APP));
		wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground  = 0;//(HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName   = NULL;//MAKEINTRESOURCEW(IDC_EXPOSCAN1);
		wcex.lpszClassName  = CnA;
		wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		CwA =  RegisterClassExW(&wcex);
	}
}

int infiniteloop() {
	HACCEL	fAT = LoadAccelerators(gI, MAKEINTRESOURCE(IDC_TOYS1APP));
	MSG		fMs;
	while(GetMessage(&fMs, nullptr, 0, 0)) {
		if(!TranslateAccelerator(fMs.hwnd, fAT, &fMs)) {
			TranslateMessage(&fMs);
			DispatchMessage(&fMs);
		}
	}
	return fMs.wParam;
}

int APIENTRY wWinMain(HINSTANCE pI, HINSTANCE pPI, LPWSTR pCL, int pCS) {
	int	reterr;
	gI = pI;

	/////////////

	if(SUCCEEDED(CoInitialize(NULL)) == 0) return 0;
	
	HRESULT	hr;
	

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(&gD2D1Fac));
	if(SUCCEEDED(hr) == 0) return 0;


		/////////////

	Cwindow::registerclassex();

			/////////////

	reterr = initall();

		/////////////

	if(gD2D1Fac)	gD2D1Fac->Release();
	CoUninitialize();

	/////////////

	return (int) reterr;
}