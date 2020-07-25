#pragma once
#ifndef KOMP_H_
#define KOMP_H_
#include "Part.h"

class Kompozicija {
	Part leva;
	Part desna;
	friend std::ostream& operator<<(std::ostream&, Kompozicija&);

	std::list<Measure*>& leva_lista();
	std::list<Measure*>& desna_lista();

public:
	void operator()(std::string );
	void ucitaj(std::string);
	void MIDI_file(std::string);
	void BMP(std::string);
	void promeni_oktavu();
    void pocni_iteraciju();
	void sledeci_takt();
	void prethodni_takt();
	void sledeca_nota_levi_part();
	void sledeca_nota_desni_part();
	void prethodna_nota_levi_part();
	void prethodna_nota_desni_part();
	void promeni_oktavu_noti();
	void postavi_povisilicu_noti();
	
	
	void promenaOktaveSaPromenomRuke();
private: void promenaRuke(std::list<MusicSymbol*>::iterator&, std::list<Measure*>&,bool);
	


};

#endif
