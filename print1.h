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

class Cprint {
protected:
	PAGESETUPDLG	psd;
	PRINTDLGEX		pdx;

	void clearsetupglobals();

	virtual bool init(DEVNAMES *dn, DEVMODE *dm, PCWSTR pn, const wchar_t *title) = 0;
	virtual bool reset();
public:
	float					m_pageHeight, m_pageWidth;
	float					m_dpih, m_dpiw;
	RECT					m_margins;

	Cprint();
	virtual ~Cprint();

	bool Setup(HWND w32win, const RECT *pmargin = 0, bool landscape = true);
	bool Print();
	bool Start(const wchar_t *title = nullptr);
	bool Stop();
};

class Dprint : public Cprint {
protected:
	ID2D1Device							*device					 = nullptr;

	bool init(DEVNAMES *dn, DEVMODE *dm, PCWSTR pn, const wchar_t *title);
	bool reset();

	IStream* printTicketStream;

	IPrintDocumentPackageTargetFactory	*documentTargetFactory	 = nullptr;
	IWICImagingFactory2					*wicFactory				 = nullptr;

	IPrintDocumentPackageTarget			*documentTarget			 = nullptr;
	ID2D1DeviceContext					*deviceContext			 = nullptr;
	
	ID2D1PrintControl					*printControl			 = nullptr;
public:
	ID2D1DeviceContext		*dev = nullptr;
	
	Dprint(ID2D1Device*);
	~Dprint();

	bool AddPage(ID2D1CommandList *pcmd);
	
};