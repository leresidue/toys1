/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
	20180123
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



#include "stdafx.h"


void Cprint::clearsetupglobals() {
	if(psd.hDevMode != 0) {
		GlobalFree(psd.hDevMode); psd.hDevMode = 0;
	}
	if(psd.hDevNames != 0) {
		GlobalFree(psd.hDevNames); psd.hDevNames = 0;
	}
}

Cprint::Cprint() {
	ZeroMemory(&psd, sizeof(psd));
	ZeroMemory(&pdx, sizeof(pdx));

	m_pageHeight = m_pageWidth = 0.0f;
	m_dpih = m_dpiw = 96.0f;
}

Cprint::~Cprint() {
	this->reset();
	clearsetupglobals();
}

bool Cprint::reset() {
	return true;
}

bool Cprint::Start(const wchar_t * title) {
	HRESULT	hr = E_ABORT;

	DEVNAMES	*dn = 0;
	DEVMODE		*dm = 0;
	PCWSTR		pn = 0;

	if(pdx.hDevNames != 0) {
		dn = reinterpret_cast<DEVNAMES*>(GlobalLock(pdx.hDevNames));
	}
	if(pdx.hDevMode != 0) {
		dm = reinterpret_cast<DEVMODE*>(GlobalLock(pdx.hDevMode));
	}
	if(dn) {
		pn = reinterpret_cast<PCWSTR>(dn) + dn->wDeviceOffset;
	}
	m_dpih = m_dpiw = 96.0f;
	if(dm) {
		if((dm->dmFields & DM_PAPERLENGTH) && (dm->dmFields & DM_PAPERWIDTH)) {
			if(dm->dmPrintQuality > 0) {
				m_dpih = m_dpiw = float(dm->dmPrintQuality);
			}
			m_pageHeight = float(dm->dmPaperLength) / 254.0f * m_dpih;
			m_pageWidth = float(dm->dmPaperWidth) / 254.0f * m_dpiw;
		} else {
			m_pageHeight = 11.0f * m_dpih;
			m_pageWidth = 8.5f * m_dpiw;
		}
		if(dm->dmOrientation == DMORIENT_LANDSCAPE) {
			float	f;
			f = m_pageHeight; m_pageHeight = m_pageWidth; m_pageWidth = f;
		}
	}
	
	
	bool ret = false;
	if(dn && dm && pn) hr = S_OK;
	if(SUCCEEDED(hr)) {
		ret = this->init(dn, dm, pn, title?title:L"");
	}

	if(pdx.hDevNames != 0) {
		GlobalUnlock(pdx.hDevNames);
	}
	if(pdx.hDevMode != 0) {
		GlobalUnlock(pdx.hDevMode);
	}
	
	return ret;
}

bool Cprint::Setup(HWND w32win, const RECT *pmargin, bool landscape) {
	clearsetupglobals();
	ZeroMemory(&psd, sizeof(psd));
	psd.lStructSize = sizeof(psd);
	psd.hInstance = GetWindowInstance(w32win);
	psd.hwndOwner = w32win;
	psd.Flags = PSD_INTHOUSANDTHSOFINCHES |
		PSD_MARGINS;

	RECT	r;
	r = m_margins;
	if(pmargin) r = *pmargin;
	m_margins = psd.rtMargin = r;

	if(PageSetupDlg(&psd)) {
		m_margins.left = psd.rtMargin.left;
		m_margins.top = psd.rtMargin.top;
		m_margins.right = psd.rtMargin.right;
		m_margins.bottom = psd.rtMargin.bottom;
		return true;
	}
	return false;
}

bool Cprint::Print() {
	pdx.lStructSize = sizeof(PRINTDLGEX);
	pdx.hwndOwner = psd.hwndOwner;
	pdx.hInstance = GetWindowInstance(psd.hwndOwner);
	pdx.hDevMode = psd.hDevMode;
	pdx.hDevNames = psd.hDevNames;

	pdx.hDC = NULL;
	pdx.Flags = PD_ALLPAGES | PD_COLLATE;
	pdx.Flags2 = 0;
	pdx.ExclusionFlags = 0;
	pdx.nPageRanges = 0;
	pdx.nMaxPageRanges = 10;
	LPPRINTPAGERANGE	pr;
	pr = (LPPRINTPAGERANGE)GlobalAlloc(GPTR, pdx.nMaxPageRanges * sizeof(PRINTPAGERANGE));
	if(pr == 0) return false;
	pdx.lpPageRanges = pr;
	pdx.nMinPage = 1;
	pdx.nMaxPage = 8;
	pdx.nCopies = 1;
	pdx.hInstance = 0;
	pdx.lpPrintTemplateName = NULL;
	pdx.lpCallback = NULL;
	pdx.nPropertyPages = 0;
	pdx.lphPropertyPages = NULL;
	pdx.nStartPage = START_PAGE_GENERAL;
	pdx.dwResultAction = 0;

	HRESULT	hr = PrintDlgEx(&pdx);

	GlobalFree(pr);

	if(hr == S_OK && pdx.dwResultAction == PD_RESULT_PRINT) {
		return true;
	}
	return false;
}

bool Cprint::Stop() {
	this->reset();
	return true;
}


/////////////

bool Dprint::init(DEVNAMES *dn, DEVMODE *dm, PCWSTR pn, const wchar_t *title) {
	HRESULT		hr = S_OK;

	HPTPROVIDER provider = nullptr;
	if(SUCCEEDED(hr)) hr = CreateStreamOnHGlobal(NULL, TRUE, &printTicketStream);
	if(SUCCEEDED(hr)) hr = PTOpenProvider(pn, 1, &provider);
	if(SUCCEEDED(hr) && printTicketStream != nullptr) hr = PTConvertDevModeToPrintTicket(
		provider, dm->dmSize + dm->dmDriverExtra, dm, kPTJobScope, printTicketStream);
	if(provider != nullptr) PTCloseProvider(provider);
	provider = nullptr;

	if(SUCCEEDED(hr)) if(documentTargetFactory == nullptr) {
		hr = ::CoCreateInstance(__uuidof(PrintDocumentPackageTargetFactory),
			nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&documentTargetFactory));
	}
	if(SUCCEEDED(hr)) if(wicFactory == nullptr) {
		hr = CoCreateInstance(CLSID_WICImagingFactory,
			nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
	}

	if(SUCCEEDED(hr)) {
		hr = documentTargetFactory->CreateDocumentPackageTargetForPrintJob(
			pn, title, nullptr, printTicketStream, &documentTarget);
	}
	if(SUCCEEDED(hr)) {
		hr = device->CreatePrintControl(wicFactory, documentTarget, nullptr, &printControl);
	}
	if(SUCCEEDED(hr)) {
		hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &deviceContext);
	}
	
	if(SUCCEEDED(hr)) {
		if(deviceContext != nullptr) {
			dev = deviceContext;
			return true;
		}
	}
	this->reset();
	return false;
}

bool Dprint::reset() {
	if(printControl) printControl->Close();

	if(printControl)		printControl->Release();
	if(documentTarget)		documentTarget->Release();
	if(printTicketStream)	printTicketStream->Release();

	if(deviceContext)		deviceContext->Release();

	printControl = nullptr;
	documentTarget = nullptr;
	printTicketStream = nullptr;
	
	deviceContext = nullptr;

	dev = nullptr;
	return true;
}

Dprint::Dprint(ID2D1Device *pdevice) {
	
	device = pdevice;
	if(device) device->AddRef();
}

Dprint::~Dprint() {
	if(device)		device->Release();

	if(wicFactory)				wicFactory->Release();
	if(documentTargetFactory)	documentTargetFactory->Release();
}

bool Dprint::AddPage(ID2D1CommandList * pcmd) {
	if(printControl != nullptr) {
		HRESULT hr =
			printControl->AddPage(pcmd, D2D1::SizeF(m_pageWidth, m_pageHeight), nullptr);
		if(SUCCEEDED(hr)) return true;
	}
	return false;
}

