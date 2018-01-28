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



class toy {
public:
	toy				*next;
	std::wstring	santa;
	union {
		int64_t		integer;
		double		decimal;
		uint64_t	rednose;
	} claus;
	bool	hasSanta = false;
	bool	hasClaus = false;
	enum {
		integer,
		decimal,
		rednose,
	} sled;
	toy();
	~toy();
	void testnose();
	void givegift();

	bool canNumber();
	int	getInteger();

	int init(wchar_t const *str, size_t sz);
};

class box {
public:
	box		*next;
	toy		*list;

};

class bag : public box {
public:
	std::string	name;
	uint32_t	good;

};
/*
class ContextCommand {
protected:

public:
	virtual ~ContextCommand();
	void receive(const wchar_t *src, size_t len);
};*/

class VerbCommand {
protected:
	toy		*toylist = nullptr;
	void clearlist();
	toy *isVerb(wchar_t const *pv);
	toy	*getToy(wchar_t const *pv);
public:
	virtual ~VerbCommand();
	void receive(std::wstring const *cmd);
	virtual void toysanta() = 0;
};

