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

class Dtext {
public:
	typedef uint64_t Dtexti;
protected:
	uint64_t	gmin = 1;
	std::unordered_map< Dtexti, IDWriteTextFormat*>	dwtf;
public:
	
	

	IDWriteTextFormat *gettf(Dtexti i);
	Dtexti createformat(const wchar_t *fn, float size, bool italic, bool bold);

	Dtext();
	~Dtext();
};

class Dcontext {
protected:
	ID2D1RenderTarget	*rT = nullptr;
	
	std::unordered_map< uint64_t, ID2D1SolidColorBrush*>	bli;
	ID2D1SolidColorBrush *getbrush(short r, short g, short b, short a = 255);

	virtual void paint();
	virtual void paint(D2D1_SIZE_F dim);
public:
	Dcontext();
	Dcontext(ID2D1RenderTarget *pr);
	~Dcontext();
};

class Dwindow : public Cwindow, public xScroll, public virtual Dcontext {
private:
	LRESULT wp(UINT msg, WPARAM wParam, LPARAM lParam);
	void destroy_rT();
	ID2D1DCRenderTarget	*drt = nullptr;
protected:
	void create_rendertarget();
public:
	void finish();
	Dwindow();
	~Dwindow();

};