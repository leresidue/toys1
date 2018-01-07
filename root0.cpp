/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/


#include "stdafx.h"

#include "resource.h"

HINSTANCE			gI;
IWICImagingFactory	*gWICFac = nullptr;
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
		wcex.hIcon          = LoadIcon(gI, MAKEINTRESOURCE(IDI_TOYS1APP));
		wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground  = 0;//(HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName   = NULL;//MAKEINTRESOURCEW(IDC_EXPOSCAN1);
		wcex.lpszClassName  = CnA;
		wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		CwA =  RegisterClassExW(&wcex);
	}
}
HACCEL	gfAT = 0;
int infiniteloop() {
	//HACCEL	fAT = LoadAccelerators(gI, MAKEINTRESOURCE(IDC_TOYS1APP));
	MSG		fMs;
	while(GetMessage(&fMs, nullptr, 0, 0)) {
		if(!TranslateAccelerator(fMs.hwnd, gfAT, &fMs)) {
			TranslateMessage(&fMs);
			DispatchMessage(&fMs);
		}
	}
	return fMs.wParam;
}

int APIENTRY wWinMain(HINSTANCE pI, HINSTANCE pPI, LPWSTR pCL, int pCS) {
	int	reterr;
	gI = pI;
	gfAT = LoadAccelerators(pI, MAKEINTRESOURCE(IDC_TOYS1APP));
	/////////////

	if(SUCCEEDED(CoInitialize(NULL)) == 0) return 0;
	
	HRESULT	hr;
	
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&gWICFac));
	if(SUCCEEDED(hr) == 0) return 0;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(&gD2D1Fac));
	if(SUCCEEDED(hr) == 0) return 0;



		/////////////

	Cwindow::registerclassex();

			/////////////

	reterr = initall();

		/////////////

	if(gWICFac)		gWICFac->Release();
	if(gD2D1Fac)	gD2D1Fac->Release();
	CoUninitialize();

	/////////////

	return (int) reterr;
}