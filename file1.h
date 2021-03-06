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



#pragma once

class Cifiledialogevents : 
	public IFileDialogEvents,
	public IFileDialogControlEvents {
public:
	IFACEMETHODIMP  QueryInterface(REFIID riid, void** ppv) {
		static const QITAB qit[] = {
			QITABENT(Cifiledialogevents, IFileDialogEvents),
			QITABENT(Cifiledialogevents, IFileDialogControlEvents),
			{ 0 },
		};
		return QISearch(this, qit, riid, ppv);
	}
	IFACEMETHODIMP_(ULONG) AddRef() {
		return InterlockedIncrement(&_cRef);
	}
	IFACEMETHODIMP_(ULONG) Release() {
		long cRef = InterlockedDecrement(&_cRef);
		if (!cRef)
			delete this;
		return cRef;
	}

	IFACEMETHODIMP  OnFileOk(IFileDialog *pfd)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnFolderChange(IFileDialog *pfd)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnFolderChanging(IFileDialog *pfd, IShellItem *psi)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnOverwrite(IFileDialog *pfd, IShellItem *psi, FDE_OVERWRITE_RESPONSE *pR)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnSelectionChange(IFileDialog *pfd)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnShareViolation(IFileDialog *pfd, IShellItem *psi, FDE_SHAREVIOLATION_RESPONSE *pR)
		{ return E_NOTIMPL; }
	IFACEMETHODIMP  OnTypeChange(IFileDialog *pfd)
		{ return E_NOTIMPL; }

	IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize *pfdc, DWORD dwIDCtl)
		{ return S_OK; }
	IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize *pfdc, DWORD dwIDCtl, BOOL bCkd)
		{ return S_OK; }
	IFACEMETHODIMP OnControlActivating(IFileDialogCustomize *pfdc, DWORD dwIDCtl)
		{ return S_OK; }
	IFACEMETHODIMP OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem)
		{ return S_OK; }

	Cifiledialogevents() : _cRef(1) { }
protected:
	~Cifiledialogevents() { }
private:
	long	_cRef;
};


class Cfile : public Cifiledialogevents {
protected:
	TCHAR	mN[MAX_PATH];
	UINT	mT = 0;
private:
	bool	IFDgetname(IFileDialog *fD);
public:
	Cfile() {
		ZeroMemory(&mN[0], sizeof(mN));
	}

	template <typename T>
	static T* getsavefilename(
		std::wstring	*pN,
		HWND			pO = NULL
	) {
		OPENFILENAME	ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = pO;
		ofn.hInstance = gI;

		T	*rV = nullptr;

		rV = new (std::nothrow) T(&ofn);
		if(rV) {
			if(pN && rV->mN[0] == 0) {
				 wcsncpy(&rV->mN[0], pN->c_str(), MAX_PATH);
			}
			ofn.lpstrFile = &rV->mN[0];
			ofn.nMaxFile = sizeof(rV->mN)/sizeof(TCHAR);

			if(GetSaveFileName(&ofn)==TRUE) {
			
			} else {
				delete rV;
				rV = nullptr;
			}
		}
		return rV;
	}

	template <typename T>
	static T* filesavedialog(
		std::wstring	*pN,
		HWND			pO = NULL
	) {
		IFileSaveDialog		*fD = nullptr;

		T	*rV = nullptr;
		
		HRESULT			hr;
		bool	must_delete = true;
		

		hr = CoCreateInstance(
			CLSID_FileSaveDialog, NULL, 
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fD));
		if(SUCCEEDED(hr)) {
			DWORD	cookie;
			bool	must_unadvise = false;

			rV = new (std::nothrow) T(fD);
			if(rV) {
				if(rV->mN[0] == 0) {
					if(pN) fD->SetFileName(pN->c_str());
				} else {
					fD->SetFileName(&rV->mN[0]);
				}

				hr = fD->Advise(rV, &cookie);
				if(SUCCEEDED(hr)) {
					must_unadvise = true;
				}

				hr = fD->Show(NULL);
				if(SUCCEEDED(hr)) {
					must_delete = rV->IFDgetname(fD);
				}

				if(must_unadvise) {
					fD->Unadvise(cookie);
				}
			}

			fD->Release();
		}
		if(must_delete) {
			delete rV;
			rV = nullptr;
		}

		return rV;
	}

	template <typename T>
	static T* fileopendialog(
		std::wstring	*pN,
		HWND			pO = NULL
	) {
		IFileOpenDialog		*fD = nullptr;

		T	*rV = nullptr;
		
		HRESULT			hr;
		bool	must_delete = true;
		

		hr = CoCreateInstance(
			CLSID_FileOpenDialog, NULL, 
			CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fD));
		if(SUCCEEDED(hr)) {
			DWORD	cookie;
			bool	must_unadvise = false;

			rV = new (std::nothrow) T(fD);
			if(rV) {
				if(rV->mN[0] == 0) {
					if(pN) fD->SetFileName(pN->c_str());
				} else {
					fD->SetFileName(&rV->mN[0]);
				}

				hr = fD->Advise(rV, &cookie);
				if(SUCCEEDED(hr)) {
					must_unadvise = true;
				}

				hr = fD->Show(pO);
				if(SUCCEEDED(hr)) {
					must_delete = rV->IFDgetname(fD);
				}

				if(must_unadvise) {
					fD->Unadvise(cookie);
				}
			}

			fD->Release();
		}
		if(must_delete) {
			delete rV;
			rV = nullptr;
		}

		return rV;
	}
};
