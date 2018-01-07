/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



#pragma once

#include "rwhole1.h"

extern HINSTANCE			gI;
extern IWICImagingFactory	*gWICFac;
extern ID2D1Factory			*gD2D1Fac;



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