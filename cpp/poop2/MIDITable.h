#pragma once
#ifndef MIDI_TABLE
#define MIDI_TABLE
#include <string>
#include "Note.h"
#include "Pause.h"
#include <map>
class MIDI {
	static MIDI* midi;
	std::map<char, MusicSymbol*> mapa;
	
	void ucitaj(std::string);
public:
	static MIDI* getInstance();
	static MIDI* createInstance(std::string);
	//static int getMIDInum(char c);
	static MusicSymbol* getMIDIsymbol(char c);
	//destruktor ?????
};
#endif