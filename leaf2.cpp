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
#include "toys1app.h"

#include "whole1.h"



void ZOfile::convert_to_filter() {
	std::wstring	str;
	wchar_t			*cW;
	for(int i = 0; i < exts.size(); i++) {
		cW = exts[i];
		if(cW) {
			str.clear();
			for(int u = 0; cW[u] != 0; u++) {
				if(cW[u] == L'.') {
					str += L"*.";
				} else if(cW[u] == L',') {
					str += L";";
				} else {
					str += cW[u];
				}
			}
			exts[i] = nullptr;
			delete [] cW;
			cW = new (std::nothrow) wchar_t[str.size()+1];
			if(cW) {
				memset(cW, 0, sizeof(wchar_t)*(str.size()+1));
				memcpy(cW, str.c_str(), sizeof(wchar_t)*str.size());
				exts[i] = cW;
			}
		}
	}
}
bool ZOfile::if_extension_is_present(const wchar_t *plist, const wchar_t *pext) {
	if(plist == nullptr) return false;

	bool	fstart_compare = false;
	UINT	current_letter;
	do {
		if(*plist == L'.') {
			fstart_compare = true;
			current_letter = 0;
		} else {
			if(fstart_compare) {
				if(pext[current_letter] == 0) {
					if(*plist == L';' || *plist == 0) {
						return true;
					}
					fstart_compare = false;
				} else {
					if(pext[current_letter] != *plist) {
						fstart_compare = false;
					}
					current_letter++;
				}
			}
		}
	} while(*plist++ != 0);

	return false;
}
UINT ZOfile::get_filterIndex_from_extension(const wchar_t *pext) {
	UINT	findex = 1;

	for(UINT i = 0; i < exts.size(); i++) {
		if(if_extension_is_present(exts[i], pext)) {
			findex = i+1;
			break;
		}
	}

	return findex;
}
void ZOfile::fill_filter(IEnumUnknown *ieu, IFileDialog *fd) {
	if(ieu == nullptr) return;
	HRESULT		hr = S_OK;
	while(hr == S_OK) {
		IUnknown	*cU;
		ULONG		nb = 0;
		cU = 0;
		hr = ieu->Next(1, &cU, &nb);
		if(hr != S_OK)	break;
		if(nb != 1)		break;

		IWICBitmapCodecInfo		*cI;
		wchar_t					*cW;
		hr = cU->QueryInterface<IWICBitmapCodecInfo>(&cI);
		if(FAILED(hr))		break;
		if(cI == nullptr)	break;
		HRESULT	hrn, hre;
		UINT	nbn = 0, nbe = 0;
					
		hre = cI->GetFileExtensions(0, 0, &nbe);
		hrn = cI->GetFriendlyName(0, 0, &nbn);

		if(SUCCEEDED(hre) && SUCCEEDED(hrn))
		if(nbe > 0 && nbn > 0) {
			cW = new (std::nothrow) wchar_t[nbe+1];
			if(cW) {
				memset(cW, 0, sizeof(wchar_t)*(nbe+1));
				hr = cI->GetFileExtensions(nbe, cW, &nbe);
				exts.push_back(cW);
				
				cW = new (std::nothrow) wchar_t[nbn+1];
				if(cW) {
					memset(cW, 0, sizeof(wchar_t)*(nbn+1));
					hr = cI->GetFriendlyName(nbn, cW, &nbn);
					names.push_back(cW);
				} else {
					names.push_back(cW);
				}
				GUID	guid = {0};
				hr = cI->GetContainerFormat(&guid);
				guids.push_back(guid);
			}
		}
		cI->Release();
		cU->Release();
		hr = S_OK;
	}
	if(fd != nullptr)
	if(names.size() == exts.size()) {
		COMDLG_FILTERSPEC	*filter;
		filter = new (std::nothrow) COMDLG_FILTERSPEC[names.size()+1];
		if(filter) {
			convert_to_filter();
			for(int i = 0; i < names.size(); i++) {
				filter[i].pszName = names[i];
				filter[i].pszSpec = exts[i];
			}
			fd->SetFileTypes(names.size(), filter);
			delete [] filter;
		}
	}
}

ZOfile::~ZOfile() {
	for(int e = 0; e < exts.size(); e++) {
		if(exts[e] != nullptr) delete [] exts[e];
	}
	for(int n = 0; n < names.size(); n++) {
		if(names[n] != nullptr) delete [] names[n];
	}
}
ZOfile::ZOfile(LPOPENFILENAME ofn) {
	ofn->lpstrTitle = L"Save Image...";
		
	ofn->lpstrFilter = L"TIFF Image\0*.tiff\0";
	ofn->nFilterIndex = 0;

	ofn->Flags = OFN_PATHMUSTEXIST	| OFN_HIDEREADONLY	| OFN_EXPLORER	
			| OFN_OVERWRITEPROMPT	| OFN_ENABLESIZING	| OFN_LONGNAMES;
	ofn->FlagsEx = OFN_EX_NOPLACESBAR;
}
	
ZOfile::ZOfile(IFileOpenDialog *fd) {
	HRESULT	hr;
	fd->SetTitle(L"Open Image...");

	if(gWICFac) {
		IEnumUnknown	*ieu = nullptr;
		hr = gWICFac->CreateComponentEnumerator(WICDecoder, 
		WICComponentEnumerateRefresh, &ieu);
		if(hr == S_OK) {
			fill_filter(ieu, fd);
		}
		if(ieu) ieu->Release();
	}

	DWORD	pfos;
	hr = fd->GetOptions(&pfos);
	if(SUCCEEDED(hr)) {
		hr = fd->SetOptions(pfos | FOS_FORCEFILESYSTEM);
	}
	
}
IFACEMETHODIMP ZOfile::OnFileOk(IFileDialog *pfd) {
		
	return S_OK;
}
IFACEMETHODIMP ZOfile::OnTypeChange(IFileDialog *pfd) {
	HRESULT	hr = S_OK;
	/*UINT	fT = 0;
	GUID	guid;
	hr = pfd->GetFileTypeIndex(&fT);
	if(SUCCEEDED(hr)) {
		if(fT > 0) {
			guid = guids[fT-1];

				
		}
	}

	IFileDialogCustomize	*fDC = nullptr;
	hr = pfd->QueryInterface<IFileDialogCustomize>(&fDC);
	if(SUCCEEDED(hr)) {
		if(IsEqualGUID(guid, GUID_ContainerFormatTiff)) {
			fDC->SetControlState(2001, CDCS_ENABLEDVISIBLE);
		} else {
			fDC->SetControlState(2001, CDCS_VISIBLE);
		}
	}
	if(fDC) fDC->Release();*/
	return hr;
}

ZOfile::ZOfile(IFileSaveDialog *fd) {
	HRESULT	hr;
	if(fd == nullptr) return;
	fd->SetTitle(L"Save Image...");
	if(gWICFac) {
		IEnumUnknown	*ieu = nullptr;
		hr = gWICFac->CreateComponentEnumerator(WICEncoder, 
		WICComponentEnumerateRefresh, &ieu);
		if(hr == S_OK) {
			fill_filter(ieu, fd);
		}
		if(ieu) ieu->Release();
	}
	
	fd->SetFileTypeIndex(get_filterIndex_from_extension(L"png"));
	fd->SetDefaultExtension(L"png");
	/*IFileDialogCustomize	*fDC = nullptr;
	hr = fd->QueryInterface<IFileDialogCustomize>(&fDC);
	if(SUCCEEDED(hr)) {
		hr = fDC->StartVisualGroup(2000, L"");
	}
	if(SUCCEEDED(hr)) {
		hr = fDC->AddCheckButton(2001, L"Compressed", true);
	}
	if(SUCCEEDED(hr)) {
		hr = fDC->EndVisualGroup();
	}
		
	if(fDC) fDC->Release();*/

	/*COMDLG_FILTERSPEC	Filter[] = {
		{ L"TIFF Image (*.tif; *.tiff)", L"*.tif;*.tiff" },
		{ L"JPEG Image (*.jpg; *.jpeg)", L"*.jpg;*.jpeg" },
	};
	fd->SetFileTypes(2, Filter);
	fd->SetFileTypeIndex(2);*/
	/*
	DWORD	pfos;
	hr = fd->GetOptions(&pfos);
	if(SUCCEEDED(hr)) {
		hr = fd->SetOptions(pfos | FOS_FORCEFILESYSTEM);
	}*/
}

void ZOfile::getbitmap(ESbitmap *src) {
	if(gWICFac) {
		IWICBitmapFrameDecode	*frame = nullptr;
		IWICBitmapDecoder		*decode = nullptr;
		HRESULT					hr = S_OK;

		GUID	guid;
		if(mT > 0) {
			guid = guids[mT-1];
		} else {
			hr = E_ABORT;
		}
		
		if(SUCCEEDED(hr)) {
			hr = gWICFac->CreateDecoderFromFilename(
			&mN[0], nullptr, GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decode);
		}

		if(SUCCEEDED(hr)) {
			hr = decode->GetFrame(0, &frame);
		}
		
		IWICFormatConverter		*conv = nullptr;
		if(SUCCEEDED(hr)) {
			hr = gWICFac->CreateFormatConverter(&conv);
		}
		if(SUCCEEDED(hr)) {
			hr = conv->Initialize(
				frame,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeCustom 
			);
		}
		src->initialize(conv);
		/*if(SUCCEEDED(hr)) {
			if(pawin) {
				//pawin->rB.setfromsource(conv);
				pawin->set_bT(conv);
			}
		}*/
		if(conv) conv->Release();
		if(frame) frame->Release();
		if(decode) decode->Release();
		
	}
}
void ZOfile::setbitmap(ESbitmap *src) {
	HRESULT	hr = S_OK;
	IWICStream				*pS = nullptr;
	IWICBitmapEncoder		*pE = nullptr;
	IWICBitmapFrameEncode	*pF = nullptr;
	IPropertyBag2			*pEO = nullptr;
	GUID	guid;
	if(mT > 0) {
		guid = guids[mT-1];
	} else {
		hr = E_ABORT;
	}
	if(src->gb() == nullptr) hr = E_ABORT;
	std::wstring	st;
	if(SUCCEEDED(hr)) {
		st.assign(&mN[0]);
		/*mT--;
		if(mT < exts.size()) {
			wchar_t	*ex = exts[mT];
			while(*ex != 0) {
				if(*ex != L'*') {
					if(*ex == L';') {
						break;
					}
					st += *ex;
				}
				ex++;
			}
		}*/
	}

	if(SUCCEEDED(hr)) {
		hr = gWICFac->CreateStream(&pS);
	}
	if(SUCCEEDED(hr)) {
		hr = pS->InitializeFromFilename(st.c_str(), GENERIC_WRITE);
	}
	if(SUCCEEDED(hr)) {
		hr = gWICFac->CreateEncoder(guid, NULL, &pE);
	}
	if(SUCCEEDED(hr)) {
		hr = pE->Initialize(pS, WICBitmapEncoderNoCache);
	}
	if(SUCCEEDED(hr)) {
		hr = pE->CreateNewFrame(&pF, &pEO);
	}
	if(SUCCEEDED(hr)) {
		hr = pF->Initialize(pEO);
	}
	UINT	fw, fh;
	if(SUCCEEDED(hr)) {
		hr = src->gb()->GetSize(&fw, &fh);
	}
	if(SUCCEEDED(hr)) {
		hr = pF->SetSize(fw, fh);
	}
	WICPixelFormatGUID	pxf;
	if(SUCCEEDED(hr)) {
		hr = src->gb()->GetPixelFormat(&pxf);
	}
	if(SUCCEEDED(hr)) {
		hr = pF->SetPixelFormat(&pxf);
	}
	IWICPalette		*pal = nullptr;
	if(SUCCEEDED(hr)) {
		hr = gWICFac->CreatePalette(&pal);
		//hr = IsEqualGUID(pxf, GUID_WICPixelFormat24bppBGR) ? S_OK: E_FAIL;
	}
	if(SUCCEEDED(hr)) {
		hr = pal->InitializePredefined(WICBitmapPaletteTypeFixedHalftone8, FALSE);
	}
	if(SUCCEEDED(hr)) {
		hr = pF->SetPalette(pal);
	}
	IWICFormatConverter		*conv = nullptr;
	if(SUCCEEDED(hr)) {
		hr = gWICFac->CreateFormatConverter(&conv);
	}
	if(SUCCEEDED(hr)) {
		hr = conv->Initialize(src->gb(), pxf, WICBitmapDitherTypeOrdered8x8,
			pal, 1.0, WICBitmapPaletteTypeFixedHalftone8);
	}
	if(SUCCEEDED(hr)) {
		BYTE	*srcdata;
		size_t	datasize;
		datasize = 8*fw;
		srcdata = new (std::nothrow) BYTE[datasize];
		if(srcdata) {
			WICRect	r;
			r.X = 0; r.Width = fw; r.Height = 1;
			for(int y = 0; y < fh; y++) {
				r.Y = y;
				hr = conv->CopyPixels(&r, datasize, datasize, srcdata);
				if(FAILED(hr)) break;
				hr = pF->WritePixels(1, datasize, datasize, srcdata);
				if(FAILED(hr)) break;
			}
		}
		delete [] srcdata;
	}
	///

	if(SUCCEEDED(hr)) {
		hr = pF->Commit();
	}
	if(SUCCEEDED(hr)) {
		hr = pE->Commit();
	}
	if(pal) pal->Release();
	if(conv) conv->Release();	
	if(pF) pF->Release();
	if(pE) pE->Release();
	if(pS) pS->Release();
}
