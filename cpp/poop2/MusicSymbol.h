#pragma once
#ifndef MUSIC_SIMBOL_H
#define MUSIC_SIMBOL_H
#include "Duration.h"
#include <iostream>
using namespace std;

class MusicSymbol {
public:
	virtual char vrsta() = 0;
	MusicSymbol(Duration d=Duration::DURATION::_1_4) :_duration(d) {}
	Duration& getDuration() { return _duration; }
	double getDoubleDuration() { return _duration.getDuration() == Duration::DURATION::_1_4 ? 0.25 : 0.125; }
	int getMidiDuration() { return _duration.getDuration() == Duration::DURATION::_1_4 ? 2:1; }
	virtual MusicSymbol* clone()=0;
	virtual std::ostream& ispis(std::ostream& os) = 0;
	virtual std::string ispis() = 0;
	virtual std::string operator ()(bool first_shared, int last_main_shared) = 0;


private:
	friend std::ostream& operator<<(std::ostream& os,  MusicSymbol& m) { return m.ispis(os); }
	friend class Kompozicija;
	friend class Measure;
	Duration _duration;
	MusicSymbol* naspram = nullptr;
	MusicSymbol* drugi_deo = nullptr;

};

#endif