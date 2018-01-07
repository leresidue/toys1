/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



#pragma once


class Cwindow {
private:
	static ATOM		CwA;
	//static wchar_t	*CnA;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND	oW;
protected:
	static const wchar_t	*CnA;

	Cwindow();

	virtual LRESULT wp(UINT message, WPARAM wParam, LPARAM lParam);

public:
	inline HWND	gw() { return oW; }
	
	virtual void finish();
	virtual ~Cwindow();

	static void registerclassex();

	template<typename T>
	static T* createwindowex(
		_In_     DWORD     dwExStyle,
		_In_opt_ LPCTSTR   lpWindowName,
		_In_     DWORD     dwStyle,
		_In_     int       x,
		_In_     int       y,
		_In_     int       nWidth,
		_In_     int       nHeight,
		_In_opt_ HWND      hWndParent,
		_In_opt_ HMENU     hMenu
	) {
	
	T			*rV;
	HWND		fW;

	rV = new T();
	fW = CreateWindowEx(
		dwExStyle,
		CnA,
		lpWindowName,
		dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu,
		gI,
		(LPVOID)rV
	);
	if(fW != NULL) {
		return rV;
	}
	delete rV;
	return nullptr;
}
};