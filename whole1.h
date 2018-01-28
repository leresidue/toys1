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

#include "rwhole1.h"

extern HINSTANCE				gI;
extern IWICImagingFactory		*gWICFac;
extern ID2D1Factory1			*gD2D1Fac;
extern IDWriteFactory			*gDWriteFac;

extern ID3D11Device			*d3dev;
extern ID3D11DeviceContext	*d3dc;
extern IDXGIDevice			*dxdev;



class EAwindow : public Cwindow, public xScroll {
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	friend class ZOfile;
	
	ID2D1DCRenderTarget	*rT = nullptr;
	void create_rendertarget();

public:
	EAcachebmp		cB;
	ESbitmap		*eB = nullptr;
	void set_bT(ESbitmap *src);
	bool	d2mode = true;

	void finish();
	EAwindow();
	~EAwindow();

	void update(int nx, int ny, HWND wnd);
};


class ZoomWidget : public Cwindow, public VerbCommand {
private:
	bool setzoom(int z);
	void sendzoom_to_target(bool k = false, POINT pt = {0,0});
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam);
	int	zPos = 32;
	int	realZ = 1;
public:
	VerbCommand	*target = nullptr;

	void toysanta();
};


class ZOfile : public Cfile {
protected:
	std::vector<wchar_t*>	names;
	std::vector<wchar_t*>	exts;
	std::vector<GUID>		guids;
	void convert_to_filter();
	bool if_extension_is_present(const wchar_t *plist, const wchar_t *pext);
	UINT get_filterIndex_from_extension(const wchar_t *pext);
	void fill_filter(IEnumUnknown *ieu, IFileDialog *fd);
public:
	bool	compressed = true;

	~ZOfile();
	ZOfile(LPOPENFILENAME ofn);
	
	ZOfile(IFileOpenDialog *fd);
	IFACEMETHODIMP OnFileOk(IFileDialog *pfd);
	IFACEMETHODIMP OnTypeChange(IFileDialog *pfd);

	ZOfile(IFileSaveDialog *fd);

	void getbitmap(ESbitmap *src);
	void setbitmap(ESbitmap *src);
};