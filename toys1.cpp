/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/



#include "stdafx.h"




toy::toy() {
	next = 0;
	santa = L"";
	hasSanta = false;
	hasClaus = false;
	claus.integer = 0;
}
toy::~toy() {
		
}
void toy::testnose() {
	if(hasSanta) {
		bool	gdec = false;
		bool	ldec = false;
		if(santa.size() >= 3) {
			switch(santa[0]) {
				case L'<': if(santa[santa.size()-1] == L'>') gdec = true; break;
				case L'[': if(santa[santa.size()-1] == L']') ldec = true; break;
			}
		}
		if(gdec || ldec) {
			claus.rednose = 0;
			hasClaus = true;
			sled = rednose;
			return;
		}
	}
	givegift();
}
void toy::givegift() {
	int mode = 0;
	hasClaus = false;
	int		boules = 0;
	int64_t	reinder = 0;
	int32_t digitcount = 0;
	for(int i = 0; i < santa.size(); i++) {
		bool	notdigit = iswdigit(santa[i]) == 0;
		if(notdigit) {
			int gold = boules;
			switch(gold) {
				case 0:	if(santa[i] == L'-') boules|=1;
				case 1:	if(santa[i] == L'.') boules|=2;
				default:
					if(gold == boules) {
						if(iswspace(santa[i]))
							break;
						return;
					}
			}
			if(gold == boules)
				break;
			if(boules & 2) {
				claus.integer = reinder;
				sled = integer;
				digitcount = 0;
			}
		} else {
			digitcount++;
			reinder *= 10;
			reinder += int(santa[i]-L'0');
		}
	}
	if(boules & 2) {
		double	builder;
		builder = reinder;
		if(digitcount > 0) {
			builder /= double(digitcount)*10.0;
		} else {
			builder = 0.0;
		}
		builder += double(claus.integer);
		claus.decimal = builder;
		sled = decimal;
	} else {
		claus.integer = reinder;
		sled = integer;
	}
	if(boules & 1) {
		switch(sled) {
			case integer: claus.integer = -claus.integer; break;
			case decimal: claus.decimal = -claus.decimal; break;
		}
	}
	hasClaus = true;
}

bool toy::canNumber() {
	if(hasClaus) {
		if(sled == integer || sled == decimal) return true;
	}
	return false;
}

int toy::getInteger() {
	int	ret = 0;
	if(hasClaus) {
		switch(sled) {
			case integer: ret = claus.integer; break;
			case decimal: ret = claus.decimal; break;
		}
	}
	return ret;
}

int toy::init(wchar_t const *str, size_t sz) {
	size_t	i;
	int	mode = 0;
	bool	notwhite = false;
	for(i = 0; i < sz; i++) {
		notwhite = iswspace(str[i]) == 0;
		if(str[i] == L',') mode = 3;
		switch(mode) {
		case 0:
			if(notwhite) {
				mode = 1;
				hasSanta = true;
				santa = str[i];
			}
			break;
		case 1:
			if(notwhite) {
				santa += str[i];
			} else {
				mode = 2;
			}
			break;
		case 2:
			if(notwhite) {
				testnose();
				return i-1;
			}
			break;
		case 3:
			if(hasSanta) testnose();
			return i;
			break;
		}
	}
	testnose();
	return i;
}
////
/*
class littlehelper {
public:
	enum lhcmd {
		nothing,

	};
	bag		*curbag;
	size_t	advance(const wchar_t *src, size_t len, lhcmd *cmd) {
		return 0;
	}
};

ContextCommand::~ContextCommand() {


}

void ContextCommand::receive(const wchar_t *src, size_t len) {
	enum {
		asanta = (1 << 0),
		alabel = (1 << 1),

	} fexpectation;
	std::vector<littlehelper>	stack;
	littlehelper			plh;
	littlehelper::lhcmd	cmd;
	size_t	i = 0, nlen;
	while(len) {
		nlen = plh.advance(&src[i], len, &cmd);
		switch(cmd) {
			case littlehelper::lhcmd::nothing:
				break;

		}
		if(nlen >= len) {
			len = 0;
		} else if(nlen == 0) {
			len = 0;
		} else {
			len -= nlen;
		}
	}
}*/


////

void VerbCommand::clearlist() {
	toy	*item = toylist;
	while(item) {
		item->santa = L"";
		item->hasSanta = false;
		item->hasClaus = false;
		item->claus.integer = 0;
		item = item->next;
	}
}

toy *VerbCommand::isVerb(wchar_t const *pv) {
	if(pv == nullptr) return nullptr;
	if(toylist == nullptr) return nullptr;
	if(toylist->hasSanta == false) return nullptr;
	if(toylist->santa == pv) return toylist;
	return nullptr;
}

toy *VerbCommand::getToy(wchar_t const *pv) {
	if(pv == nullptr) return nullptr;
	
	toy	*item = toylist;
	while(item) {
		if(item->hasSanta == false) return nullptr;
		if(item->santa == pv) return item;
		item = item->next;
	}

	return nullptr;
}

VerbCommand::~VerbCommand() {
	toy	*buf;
	while(toylist) {
		buf = toylist->next;
		delete toylist;
		toylist = buf;
	}
}

void VerbCommand::receive(std::wstring const *cmd) {
	toy	*target = toylist, *ptarget = nullptr;
	wchar_t	const *str;
	str = cmd->c_str();

	if(target == nullptr) {
		target = new toy();
		toylist = target;
	}
	clearlist();
	
	bool param;

	int offset = 0;
	size_t	sz = cmd->size();
	while(offset < sz) {
		offset += target->init(&str[offset], sz-offset);
		if(target->next == 0) {
			target->next = new toy();
		}
		param = true;
		if(offset < sz) if(str[offset] == L',') {
			param = false;
			offset++;
		}
		if(ptarget && param) {
			if(ptarget->hasClaus == false) {
				if(target->hasClaus == true) {
					target->hasClaus = false;
					ptarget->hasClaus = true;
					ptarget->sled = target->sled;
					switch(target->sled) {
						case toy::integer: ptarget->claus.integer = target->claus.integer; break;
						case toy::decimal: ptarget->claus.decimal = target->claus.decimal; break;
						case toy::rednose: ptarget->claus.rednose = target->claus.rednose; break;
					}
				}
			}
		}
		ptarget = target;
		target = target->next;
	}
	this->toysanta();
}

