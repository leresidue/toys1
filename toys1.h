/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



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

