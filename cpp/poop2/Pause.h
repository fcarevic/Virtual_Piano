#pragma once
#ifndef PAUSE_H
#define PAUSE_H
#include "MusicSymbol.h"
class Pause : public MusicSymbol {
	int midi_num;
public: 
	char vrsta() { return 'p'; }
	Pause * clone() { return new Pause(*this); }
	Pause(int m,Duration d) :MusicSymbol(d),midi_num(m) {}
	virtual std::ostream& ispis(std::ostream& os) {
		return os<<( getMidiDuration() == 2 ? '*' : '+');
	}
	virtual std::string ispis() {
		return getMidiDuration() == 2 ? "* " : "+ ";
	}

	virtual std::string operator ()(bool first_shared, int last_main_shared) {
	std:string s;
		s += "<note>\n<rest/>\n<duration>"+to_string(getMidiDuration())+"</duration>\n</note>\n";
		return s;
}



};
#endif
