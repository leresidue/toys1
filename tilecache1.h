/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



#pragma once

class EACBbitmap {
public:
	EACBbitmap	*next = nullptr;
	ID2D1Bitmap	*bmp = nullptr;
	bool		ptu[16];
	bool		ptc[16];
	EACBbitmap(ID2D1RenderTarget *rT);
	~EACBbitmap();

	void CopyScale(int scale, POINT orig, UINT pw, UINT ph, BYTE *cache, BYTE *src, UINT32 stride, int item);
	void CopyFrom(POINT orig, UINT pw, UINT ph, BYTE *cache, IWICBitmapSource *src, int item);
	void CopyBits(POINT orig, UINT pw, UINT ph, BYTE *src, UINT32 stride, int item);
};

class EACBrect {
public:
	RECT		rect;
	EACBbitmap	*bitmap = nullptr;
	int			item = 16;
	EACBrect(const EACBrect &r);
	EACBrect(RECT r, EACBbitmap *b, int i);
	bool operator== (const RECT &b);
	bool operator> (const RECT &b);
};
class ESbitmap;
class EAcachebmp {
public:
	ID2D1RenderTarget		*rT = nullptr;
	EACBbitmap				*list = nullptr;
	uint32_t				*cache = nullptr;
	//IWICBitmapSource		*scaler = nullptr;
	bool					usescaler = false;
	int						scaling = 0;
	std::vector<EACBrect>	zones;

	EAcachebmp();
	~EAcachebmp();

	int32_t	cdim = 256;

	bool	esisvalid = false;
	int		eswidth = 0, esheight = 0;

	int	width = 0, height = 0;
	int	left = 0, top = 0;
	bool	in_update_function_set_wnd_to_NULL = false;

	void DrawBitmap(ESbitmap *eB, D2D1_RECT_F const &rrr, float pf);

	void clear();
	void flush();
	void add(RECT r);
	void update_view(int px, int py, int pw, int ph);
	void event_rendertarget_changed(ID2D1RenderTarget *ptr);
	void test_window_redimensioned_and_retrieve_bitmap_size(HWND pwnd, ESbitmap *pLimit);
	void event_size_changed(uint32_t pwidth, uint32_t pheight);
	void event_position_changed(int px, int py);
	void step_event_zoom_changed(int pz);
};