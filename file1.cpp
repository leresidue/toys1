/* 2018 leresidue
	� 2018 Fr�d�rique Brisson-Lambert
*/


#include "stdafx.h"


bool Cfile::IFDgetname(IFileDialog *fD) {
	bool		must_delete = true;
	IShellItem	*sI = nullptr;
	HRESULT		hr;
	hr = fD->GetFileTypeIndex(&mT);
	hr = fD->GetResult(&sI);
	if(SUCCEEDED(hr)) {
		LPWSTR	pS;
		hr = sI->GetDisplayName(SIGDN_FILESYSPATH, &pS);
		if(SUCCEEDED(hr)) {
			wcsncpy_s(mN, (TCHAR*)pS, MAX_PATH);
			CoTaskMemFree(pS);

			must_delete = false;
		}
		sI->Release();
	}
	return must_delete;
}