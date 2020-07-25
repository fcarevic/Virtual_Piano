#pragma once
#ifndef NOTE_H_
#define NOTE_H_
#include "MusicSymbol.h"
#include <locale>
class Note :public MusicSymbol {

public:
	char vrsta(){
		return 'n';
}
	Note(int _oktava, bool _povisena, char _oznaka, int _midi_num) :oktava(_oktava), povisena(_povisena), oznaka(_oznaka), midi_num(_midi_num) {}
	Note* clone() { return new Note(*this); }
	char Oznaka() { return oznaka; }
	bool& Podeljena() { return podeljena; }
	bool& Zajedno() { return zajedno; }
	bool& glavna_pod() { return glavna_podeljena; }
	void setPoslednjaAkord(bool t) { poslednja_u_akordu = t; }
	bool isPoslednjaAkord() { return poslednja_u_akordu; }
	int _oktava() { return oktava; }
	void promeni_oktavu(int n) {
		if (oktava + n > 6 || oktava + n < 2)return;
		oktava += n;
	
	
	}

	virtual std::string ispis() {
		std::string s = "";
		if (getMidiDuration() == 1) s+= (char)tolower(oznaka);
		else s+= (char)toupper(oznaka);
		if (povisena)s+="#";
		s+= to_string(oktava);
		if (!zajedno || poslednja_u_akordu )s += " ";
		return s;

	}




	std::ostream& ispis(std::ostream& os) {
		if (getMidiDuration() == 1) os << (char)tolower(oznaka);
		else os << (char)toupper(oznaka);
		if (povisena)os << '#';
			os << oktava;
			return os;

	}
	bool& Povisena(){
		return povisena;
	}

	int midi_n() { return midi_num; }

	virtual std::string operator ()(bool first_shared,int last_main_shared) {
		std::string s;
	
		s += "<note>\n";
		if (zajedno && !first_shared) s += "<chord/>\n";
		s += "<pitch>\n";
		s += "<step>"+std::string(1,oznaka)+"</step>\n";
		s += "<octave>" + to_string(oktava) + "</octave>\n";
		if (povisena)s += "<alter>1</alter>\n";
		s += "</pitch>\n";
		s += "<duration>" +to_string(getMidiDuration())+ "</duration>\n";
		//if (last_main_shared==1 && glavna_podeljena)
		if(glavna_podeljena)
		s += "<tie type=\"start\"/>\n";
		//if (last_main_shared == 0 && podeljena)
		if(podeljena && !glavna_podeljena)
		s += "<tie type=\"end\"/>\n";
		s += "</note>\n";
		return s;
	
	
	}



private:
	bool glavna_podeljena=false;
	int oktava;
	bool povisena;
	char oznaka;
	int midi_num;
	bool podeljena=false;
	bool zajedno = false;
	bool poslednja_u_akordu = false;


};

#endif