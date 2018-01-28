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

HINSTANCE			gI;

ID3D11Device			*d3dev = nullptr;
ID3D11DeviceContext		*d3dc = nullptr;
IDXGIDevice				*dxdev = nullptr;

IWICImagingFactory	*gWICFac = nullptr;
ID2D1Factory1		*gD2D1Fac = nullptr;
IDWriteFactory		*gDWriteFac = nullptr;

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

class errorinitall_window : public Cwindow, public xScroll {
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
			if(errmessage) {
				int	n = 0;
				while(errmessage[n] != 0) {
					n++;
				}
				TextOut(hdc, r.right/2, r.bottom/2, errmessage, n);
			}
			EndPaint(gw(), &ps);
		} return 0;
		}
		return Cwindow::wp(msg, wParam, lParam);
	}
public:
	const wchar_t	*errmessage = nullptr;
	errorinitall_window() {}
	~errorinitall_window() {}
	void finish() {}
};

class errorinitall_window_main : public Cwindow {
public:
	HWND		szb = nullptr;
	XscrollWindow<errorinitall_window>		*sco = nullptr;
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);
			switch (wmId) {
			case ID_FICHIER_OUVRIRUNEIMAGE:
				break;
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
	errorinitall_window_main() {}
	~errorinitall_window_main() {
		
	}
};

int errorinitall(const wchar_t *errmessage) {
	{errorinitall_window_main	*mwi = Cwindow::createwindowex<errorinitall_window_main>(
		0, L"ALLÔ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
		LoadMenu(gI, MAKEINTRESOURCEW(IDC_TOYS1APP))
	);
	if(mwi) {
		ShowWindow(mwi->gw(), SW_SHOW);

		RECT	r;
		GetClientRect(mwi->gw(), &r);

		mwi->sco = Cwindow::createwindowex<XscrollWindow<errorinitall_window>>(
		0, L"", WS_CHILD|WS_VISIBLE, 0, 0, r.right, r.bottom, mwi->gw(), nullptr
		); if(mwi->sco) {
			mwi->sco->gx()->errmessage = errmessage;
			mwi->sco->finish();
			
		}

		mwi->szb = CreateWindow(L"SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|
		SBS_SIZEGRIP|SBS_SIZEBOXBOTTOMRIGHTALIGN,
		r.left, r.top, r.right, r.bottom, 
		mwi->sco->gw(), (HMENU)-1, GetModuleHandle(NULL), NULL);
	}}

	return infiniteloop();
}

int APIENTRY wWinMain(HINSTANCE pI, HINSTANCE pPI, LPWSTR pCL, int pCS) {
	int	reterr;
	gI = pI;
	gfAT = LoadAccelerators(pI, MAKEINTRESOURCE(IDC_TOYS1APP));
	/////////////

	if(SUCCEEDED(CoInitialize(NULL)) == 0) return 0;
	
	HRESULT	hr;
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
	D3D_FEATURE_LEVEL m_featureLevel;
	hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,          
            &d3dev, &m_featureLevel, &d3dc);
	if(SUCCEEDED(hr)) {
		hr = d3dev->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxdev));
	}

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&gWICFac));

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(&gD2D1Fac));

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&gDWriteFac));

		/////////////

	Cwindow::registerclassex();

			/////////////

	if(gDWriteFac && gD2D1Fac && dxdev)
		reterr = initall();
	else
		reterr = errorinitall(L"Insufficient ressources on your system.");

		/////////////
	if(dxdev)		dxdev->Release();
	if(d3dc)		d3dc->Release();
	if(d3dev)		d3dev->Release();

	if(gDWriteFac)	gDWriteFac->Release();
	if(gWICFac)		gWICFac->Release();
	if(gD2D1Fac)	gD2D1Fac->Release();
	CoUninitialize();

	/////////////

	return (int) reterr;
}