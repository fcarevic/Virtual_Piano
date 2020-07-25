#include "MIDITable.h"
#include <fstream>
#include <regex>
#include <iostream>
MIDI* MIDI::midi = nullptr;

MIDI* MIDI::getInstance() {
	return midi;
}

void MIDI::ucitaj(std::string s) {
	std::ifstream file(s);
	std::string textLine;
	std::regex rx("(.),(.*),(.*)");
	while (getline(file, textLine)) {
		std::smatch result;
		if (regex_match(textLine, result, rx)) {
			char znak = result.str(1) [0];
			std::string simbol = result.str(2);
			int midi_num = stoi(result.str(3));
			MusicSymbol* M_simbol;
			/*if (simbol == " ") 
				M_simbol = new Pause(midi_num,(Duration::DURATION::_1_8));//POSTOJE LI PAUZE U ONOM FAJLU???	
			else if (simbol == "|") M_simbol = new Pause(midi_num,(Duration::DURATION::_1_4));
			else {*/
				
				if (simbol[1] == '#') {
					M_simbol = new Note(simbol[2]-'0',true,simbol[0],midi_num);

				}
				else {
					M_simbol = new Note(simbol[1]-'0',false, simbol[0], midi_num);
				
				
				}
			
			
			
			mapa[znak] = M_simbol;  // ili mapa.insert(std::pair<char,MusicSymbol> (znak,M_simbol));
		}
	}
	mapa[' ']= new Pause(-1, (Duration::DURATION::_1_8));
	mapa['|']= new Pause(-1, (Duration::DURATION::_1_4));
	file.close();

}

MIDI* MIDI::createInstance(std::string s) {
	if (midi != nullptr) return midi;
	 midi = new MIDI();
	 midi->ucitaj(s);
	 return midi;
}



MusicSymbol* MIDI::getMIDIsymbol(char c) {
	if (midi == nullptr) {
		std::cout << "\nNe postoji MIDI\n";
		return nullptr; 
	}
	return midi->mapa[c];
	
}