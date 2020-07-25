#pragma once
#ifndef MUEASURE_H_
#define MUEASURE_H_
#include "MusicSymbol.h"
#include <list>
class Measure {
	friend class XML;
	/////////////////////////
	
	static double trajanje;
	double trenutno_trajanje = 0;
	std::list<MusicSymbol*> lista;
	bool nadovezuje_se=false;
	friend class Kompozicija;

public:
	std::string toString();
	static int brojilac();
	static int imenilac();
	std::string operator()(bool);
	friend std::ostream& operator << (std::ostream&, const Measure&);
	static void setMaxTrajanje(double );
	static double MaxTrajanje();
	double TrenutnoTrajanje();
	void setNadovezuje(bool b);
	void dodaj(MusicSymbol* m);
	void dodaj(MusicSymbol* , bool);
	static double getMaxTrajanje();
	~Measure();




};


#endif