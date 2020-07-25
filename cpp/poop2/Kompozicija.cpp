#include <iomanip>
#include "Kompozicija.h"
#include "MidiFile.h"
#include <fstream>
#include <regex>
#include "MIDITable.h"
#include <typeinfo>
#include <iomanip>
using namespace smf;
using namespace std;

void Kompozicija::ucitaj(std::string s) {
	std::ifstream file(s);

	std::string textLine;
	std::regex rx("([^\\[]*){0,1}(\\[[^\\]]*\\]){0,1}");
	Measure* measure_left = new Measure();
	Measure* measure_right = new Measure();
	//int i = 0;
	while (getline(file, textLine)) {
		std::sregex_iterator begin(textLine.begin(), textLine.end(), rx);
		std::sregex_iterator end;
		
		while (begin != end) {
			std::smatch result = *begin;
			if (result.str(0) == "") { begin++; continue; }
			for (char c : result.str(1)) {
				MusicSymbol* ms = MIDI::getMIDIsymbol(c)->clone();

				if (typeid(*ms) == typeid(Pause)) { ///ovde ubacuje pauze i deli ih po potrebi
					if ((int)((ms->getDoubleDuration() + measure_left->TrenutnoTrajanje())*1000) >(int) (measure_left->MaxTrajanje()*1000)) {
						MusicSymbol* ms2 = ms->clone();
						ms2->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->getDuration().setDuration(Duration::DURATION::_1_8);
						measure_left->dodaj(ms->clone());
						measure_right->dodaj(ms);
						leva.dodaj(measure_left);
						desna.dodaj(measure_right);
						measure_right = new Measure(); measure_right->dodaj(ms2->clone());
						measure_left = new Measure(); measure_left->dodaj(ms2);

					}
					else {
						measure_left->dodaj(ms->clone());
						measure_right->dodaj(ms);
						if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje()) {
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);

							measure_left = new Measure();
							measure_right = new Measure();

						}

					}

				}//PAUZE
				else {

					if (ms->getDoubleDuration() + measure_left->TrenutnoTrajanje() > measure_left->MaxTrajanje()) {
						MusicSymbol* ms3 = new Pause(-1, Duration(Duration::DURATION::_1_8));
						MusicSymbol* ms4 = ms3->clone();
						MusicSymbol* ms2 = ms->clone();
						ms2->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->naspram = ms3;
						ms2->naspram = ms4;
						ms->drugi_deo = ms2;
						static_cast<Note*>(ms)->Podeljena() = true;
						static_cast<Note*>(ms)->glavna_pod() = true;
						static_cast<Note*>(ms2)->Podeljena() = true;
						if (static_cast<Note*>(ms2)->_oktava() > 3) {

							measure_left->dodaj(ms3);
							measure_right->dodaj(ms);
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);
							measure_right = new Measure(); measure_right->dodaj(ms2);
							measure_left = new Measure(); measure_left->dodaj(ms4);


						}
						else {

							measure_left->dodaj(ms);
							measure_right->dodaj(ms3);
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);
							measure_right = new Measure(); measure_right->dodaj(ms4);
							measure_left = new Measure(); measure_left->dodaj(ms2);
						}


					}
					else {
						MusicSymbol* ms3 = new Pause(-1, ms->getDuration());
						ms->naspram = ms3;
						
						if (static_cast<Note*>(ms)->_oktava() > 3) {

							measure_left->dodaj(ms3);
							measure_right->dodaj(ms);
							
						

						}
						else {

							measure_left->dodaj(ms);
							measure_right->dodaj(ms3);



						}

						if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje()) {
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);

							measure_left = new Measure();
							measure_right = new Measure();

						}

					}






				}


			}///// sve van zagrada

			bool zajedno = result.str(2).find(" ") == std::string::npos ? true : false;
			 
			if (result.str(2).length() < 3) { begin++; continue; }
			if (zajedno) {
				MusicSymbol** niz = new MusicSymbol*[result.str(2).length()];
				int i = 0;
				for (char c : result.str(2)) {
					if (c == '[' || c == ']') continue;
					niz[i] = MIDI::getMIDIsymbol(c)->clone();
					static_cast <Note*> (niz[i])->Zajedno() = true;
						i++;
				}
				bool dodat_levo = false;
				//bool ima_levo = false;
				bool dodat_desno = false;

				
				int length = result.str(2).length()-2;
				for (; length > 0; length--) {
					Note* n = static_cast <Note*> (niz[length-1]);//zbog ovoga dodavanja gore
					if (n->_oktava() > 3 ) {
						if (!dodat_desno) {
							n->setPoslednjaAkord(true);
							dodat_desno = true;
						}
					}
					else if(!dodat_levo){
						n->setPoslednjaAkord(true);
						dodat_levo = true;
					}
					
					if (dodat_desno&& dodat_levo) break;
				}
				
				dodat_desno = false;
				dodat_levo = false;
				//bool ima_desno= false;

				
				if (measure_left->MaxTrajanje() >= measure_left->TrenutnoTrajanje() + niz[0]->getDoubleDuration()) {
				
					for (int j = 0; j < i; j++) {
						if (static_cast<Note*>(niz[j])->_oktava() > 3) {
							if (!dodat_desno) {
								measure_right->dodaj(niz[j]);
								dodat_desno = true;
								//ima_desno = true;
							}
							else measure_right->dodaj(niz[j], true);
						
						}
						else {  
							if (!dodat_levo) {
								measure_left->dodaj(niz[j]);
								dodat_levo= true;
								//ima_desno = true;
							}
							else measure_left->dodaj(niz[j], true); 
						}
					
					
					}//for

					if (!dodat_desno) measure_right->dodaj(new Pause(-1,Duration(Duration::DURATION::_1_4)));
					if(!dodat_levo)measure_left->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_4)));

					if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje()) {
						leva.dodaj(measure_left);
						desna.dodaj(measure_right);
						measure_left = new Measure();
						measure_right = new Measure();
					
					}
				
				}  /// ako moze da stane u jedan takt
				else{
					Measure* new_right = new Measure();
					Measure* new_left = new Measure();
					for (int j = 0; j < i; j++) {
						MusicSymbol* novi = niz[j]->clone();
						static_cast<Note*>(niz[j])->getDuration().setDuration(Duration::DURATION::_1_8);
						static_cast<Note*>(novi)->getDuration().setDuration(Duration::DURATION::_1_8);
						static_cast<Note*>(niz[j])->Podeljena() = true;
						static_cast<Note*>(niz[j])->glavna_pod() = true;
						static_cast<Note*>(novi)->Podeljena() = true;
						if (static_cast<Note*>(niz[j])->_oktava() > 3) {
							if (!dodat_desno) {
								measure_right->dodaj(niz[j]);
								new_right->dodaj(novi);
								dodat_desno = true;
								//ima_desno = true;
							}
							else {
								measure_right->dodaj(niz[j], true);
							   new_right->dodaj(novi, true);
							}
						}
						else {
							if (!dodat_levo) {
								measure_left->dodaj(niz[j]);
								new_left->dodaj(novi);
								dodat_levo = true;
								//ima_desno = true;
							}
							else { measure_left->dodaj(niz[j], true); 
							         new_left->dodaj(novi, true);

							}
						}


					}//for
					
					if (!dodat_desno) { measure_right->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8))); 
					new_right->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8)));
					
					}
					if (!dodat_levo) { measure_left->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8)));
					
					new_left->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8)));
					}

					leva.dodaj(measure_left);
					desna.dodaj(measure_right);
					measure_left = new_left;
					measure_right = new_right;


				
				
				}

				delete[] niz;
			
			}

			else{
			bool dodat_jedan = false;
			bool dodat2 = false;
			for (char c : result.str(2)) {
				if (c == '[' || c == ' ')continue;
				if (c == ']') {

					if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje() && zajedno) {
						leva.dodaj(measure_left);
						desna.dodaj(measure_right);

						measure_left = new Measure();
						measure_right = new Measure();

					}
					continue;

				}

				MusicSymbol* ms = MIDI::getMIDIsymbol(c)->clone();
				if (!zajedno)ms->getDuration().setDuration(Duration::DURATION::_1_8);

				if (typeid(ms) == typeid(Pause)) { ///ovde ubacuje pauze i deli ih po potrebi
					if (ms->getDoubleDuration() + measure_left->TrenutnoTrajanje() > measure_left->MaxTrajanje()) {
						MusicSymbol* ms2 = ms->clone();
						ms2->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->getDuration().setDuration(Duration::DURATION::_1_8);
						measure_left->dodaj(ms->clone());
						measure_right->dodaj(ms);
						leva.dodaj(measure_left);
						desna.dodaj(measure_right);
						measure_right = new Measure(); measure_right->dodaj(ms2->clone());
						measure_left = new Measure(); measure_left->dodaj(ms2);

					}
					else {
						measure_left->dodaj(ms->clone());
						measure_right->dodaj(ms);
						if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje()) {
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);

							measure_left = new Measure();
							measure_right = new Measure();

						}

					}

				}//DODAVANJE PAUZE
				else {

					if (ms->getDoubleDuration() + measure_left->TrenutnoTrajanje() > measure_left->MaxTrajanje() && !dodat2&&zajedno || dodat_jedan && zajedno) {
						MusicSymbol* ms3 = new Pause(-1, Duration(Duration::DURATION::_1_8));
						MusicSymbol* ms4 = ms3->clone();
						MusicSymbol* ms2 = ms->clone();
						ms2->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->getDuration().setDuration(Duration::DURATION::_1_8);
						ms->naspram = ms3;
						ms2->naspram = ms4;
						ms->drugi_deo = ms2;
						
						
						static_cast<Note*>(ms)->Podeljena() = true;
						static_cast<Note*>(ms2)->Podeljena() = true;
						static_cast<Note*>(ms)->glavna_pod() = true;
						static_cast<Note*>(ms)->Zajedno() = zajedno;
						static_cast<Note*>(ms2)->Zajedno() = zajedno;
						if (static_cast<Note*>(ms2)->_oktava() > 3) {

							if (dodat_jedan && zajedno) {
								if (leva.lista.back()->MaxTrajanje() == leva.lista.back()->TrenutnoTrajanje() + ms->getDoubleDuration()) {

									desna.lista.back()->dodaj(ms, zajedno);
									measure_right->dodaj(ms2, zajedno);

								}



							}
							else {

								measure_left->dodaj(ms3);
								measure_right->dodaj(ms);
								leva.dodaj(measure_left);
								desna.dodaj(measure_right);
								measure_right = new Measure(); measure_right->dodaj(ms2);
								measure_left = new Measure(); measure_left->dodaj(ms4);
								dodat_jedan = true;
							}

						}
						else
							if (dodat_jedan && zajedno) {
								if ((float)(leva.lista.back()->MaxTrajanje()) == leva.lista.back()->TrenutnoTrajanje()) {

									leva.lista.back()->dodaj(ms, zajedno);
									measure_left->dodaj(ms2, zajedno);

								}


							}
							else {

								measure_left->dodaj(ms);
								measure_right->dodaj(ms3);
								leva.dodaj(measure_left);
								desna.dodaj(measure_right);
								measure_right = new Measure(); measure_right->dodaj(ms4);
								measure_left = new Measure(); measure_left->dodaj(ms2);
								dodat_jedan = true;
							}


					} //ako treba da se podeli nota je ovo iznad
					else {
						static_cast<Note*>(ms)->Zajedno() = zajedno;

						MusicSymbol* ms3 = new Pause(-1, ms->getDuration());
						ms->naspram = ms3;
						if (static_cast<Note*>(ms)->_oktava() > 3) {

							if (zajedno&&dodat2) {
								measure_right->dodaj(ms, zajedno);

							}
							else {
								measure_left->dodaj(ms3);
								measure_right->dodaj(ms);
							}


						}
						else {
							if (zajedno&&dodat2) {
								measure_left->dodaj(ms, zajedno);

							}
							else {
								measure_left->dodaj(ms);
								measure_right->dodaj(ms3);
							}
						}
						dodat2 = true;

						if (measure_left->MaxTrajanje() == measure_left->TrenutnoTrajanje() && !zajedno) {
							leva.dodaj(measure_left);
							desna.dodaj(measure_right);

							measure_left = new Measure();
							measure_right = new Measure();

						}

					}

				}
			}

					//std::cout << i << ": " << result.str(0) << std::endl;
					//std::cout << i << ": " << result.str(1) << std::endl;
					//std::cout << i << ": " << result.str(2) << std::endl;
					//i++; 
				}
				begin++;


			}


		}
	
		if (measure_left->TrenutnoTrajanje() == 0) { delete measure_left; delete measure_right; }
		else {
			while ((int)(measure_left->MaxTrajanje() * 1000) != (int)(measure_left->TrenutnoTrajanje() * 1000)) { //dopunjavanje do punog takta
				measure_left->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8)));
				measure_right->dodaj(new Pause(-1, Duration(Duration::DURATION::_1_8)));
			}

			leva.dodaj(measure_left);
			desna.dodaj(measure_right);
		}
	file.close();
	

}
std::ostream& operator<<(std::ostream& os, Kompozicija& k) {
	
	int br = 1;
	auto iter2 = k.desna_lista().begin();
	for (auto i = k.leva_lista().begin(); i != k.leva_lista().end();i++) {
		if(br==1)os << "Leva: ";
		os <<std::setw(20)<<std::right<< (*i)->toString()<<"|";
		br++;
		if (br % 5 == 0) {
			br = 2;
			os << "\n";
			os << "Desn: ";
			while (br % 6 != 0) {
				br++;
				os << std::setw(20)<<std::right<< (*iter2)->toString() << "|";
				iter2++;

			}
			os << "\n";
			br = 1;

		}
	}
	os << "\n";
	if (iter2 != k.desna_lista().end()) {
		os << "Desn: ";
		while (iter2 != k.desna_lista().end()) {



			
			os << std::setw(20) <<std::right<< (*iter2)->toString() << "|";
			iter2++;


		}
		os << "\n";
		}
	return os;

}


std::list<Measure*>& Kompozicija::leva_lista() { return leva.lista; }
std::list<Measure*>& Kompozicija::desna_lista() { return desna.lista; }


void Kompozicija::MIDI_file(std::string ime) {


	MidiFile outputfile;
	outputfile.absoluteTicks();
	vector<uchar> midievent;
	midievent.resize(3);
	int tpq = 48;
	outputfile.setTicksPerQuarterNote(tpq);
	outputfile.addTrack(1);

	// data to write to MIDI file: (60 = middle C)
	// C5 C  G G A A G-  F F  E  E  D D C-
	/*
	int melody[50] = { 72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1 };
	int mrhythm[50] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,-1 };

	// C3 C4 E C F C E C D B3 C4 A3 F G C-
	int bass[50] = { 48,60,64,60,65,60,64,60,62,59,60,57,53,55,48,-1 };
	int brhythm[50] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,-1 };
	*/

	
	int actiontime = 0;
	int zaj_action = -1;
	midievent[2] = 64;
	for(auto iter:leva.lista) {

		for (auto it : iter->lista) {
			if (typeid(*it) == typeid(Pause)) {
				actiontime += tpq * (it->getMidiDuration());
				zaj_action = -1;
				continue;
			}
			if (((Note*)it)->Podeljena() && !((Note*)it)->glavna_pod()) continue;
			midievent[0] = 0x90;
			midievent[1] = ((Note*)it)->midi_n();

			if (((Note*)it)->Zajedno()) {
				if (zaj_action != -1)actiontime = zaj_action;
				else zaj_action = actiontime;
			}
			else zaj_action = -1;

			outputfile.addEvent(0, actiontime, midievent);
			if(!((Note*)it)->Podeljena())
			actiontime += tpq * (it->getMidiDuration());
			else actiontime += tpq * 2;

			/*if (((Note*)it)->Podeljena()) {
				if (((Note*)it)->glavna_pod()) {
					if (!((Note*)it)->Zajedno())actiontime += tpq * 2;//(it->getMidiDuration());
				}
				//else continue;
			}
				else if (!((Note*)it)->Zajedno())actiontime += tpq *(it->getMidiDuration());
			*/
			midievent[0] = 0x80;
			outputfile.addEvent(0, actiontime, midievent);
			if (((Note*)it)->isPoslednjaAkord())zaj_action = -1;
			
		}
	}

	actiontime = 0;
	zaj_action = -1;





	for (auto iter : desna.lista) {

		for (auto it : iter->lista) {
			if (typeid(*it) == typeid(Pause)) {
				actiontime += tpq * (it->getMidiDuration());
				zaj_action = -1;
				continue;
			}
			if (((Note*)it)->Podeljena() && !((Note*)it)->glavna_pod()) continue;
			midievent[0] = 0x90;
			midievent[1] = ((Note*)it)->midi_n();

			if (((Note*)it)->Zajedno()) {
				if (zaj_action != -1)actiontime = zaj_action;
				else zaj_action = actiontime;
			}
			else zaj_action = -1;



			outputfile.addEvent(1, actiontime, midievent);

			if (!((Note*)it)->Podeljena())
				actiontime += tpq * (it->getMidiDuration());
			else actiontime += tpq * 2;

			

			//if (!((Note*)it)->Zajedno())actiontime += tpq * (it->getMidiDuration());
			midievent[0] = 0x80;
			outputfile.addEvent(1, actiontime, midievent);
			if (((Note*)it)->isPoslednjaAkord())zaj_action = -1;
		}
	}














	/*for (auto iter : desna.lista) {

		for (auto it : iter->lista) {
			if (typeid(*it) == typeid(Pause)) {
				actiontime += tpq * (it->getMidiDuration());
				continue;
			}
			if (((Note*)it)->Podeljena() && !((Note*)it)->glavna_pod()) continue;
			midievent[0] = 0x90;
			midievent[1] = ((Note*)it)->midi_n();
			outputfile.addEvent(1, actiontime, midievent);
			
			if (((Note*)it)->Podeljena()) {
				if (((Note*)it)->glavna_pod()) {
					if (!((Note*)it)->Zajedno())actiontime += tpq * 2;//(it->getMidiDuration());
				}
			//	else continue;
			}
			else if (!((Note*)it)->Zajedno())actiontime += tpq * (it->getMidiDuration());
			
			//if (!((Note*)it)->Zajedno())actiontime += tpq * (it->getMidiDuration());
			midievent[0] = 0x80;
			outputfile.addEvent(1, actiontime, midievent);

		}
	}
	*/
	outputfile.sortTracks();
	outputfile.write(ime+".mid");










}

void Kompozicija::operator()(std::string ime) {

	std::ofstream file(ime+".musicxml");
	std::string s = "<?xml version=\"1.0\" ";
		s += "encoding=\"UTF-8\" standalone=\"no\"?>\n";
	
	s += "<!DOCTYPE score-partwise ";
		s+="PUBLIC\n";
		s += "\"-//Recordare//DTD ";
		s += "MusicXML 3.1 Partwise//EN\"\n";

		s += "\"http://www.musicxml.org/dtds/partwise.dtd\">\n";
		s += "<score-partwise version=\"3.1\">\n";
		s += "<part-list>\n";
		s += "<score-part id=\"Right\"></score-part>\n";
		s += "<score-part id=\"Left\"></score-part>\n";
		s+="</part-list>\n";
		file << s;
		file << desna(false);
		file << leva(true);
		file << "</score-partwise>";



		file.close();



}
void Kompozicija::BMP(std::string s) {

	std::cout << "Uneti sirinu slike u pikselima\n";
	int sirina;
	std::cin >> sirina;


	struct boje {
		int r, g, b;

	};
	std::ifstream fajl("mapa_BMP.txt");
	std::string textLine;
	std::map<std::string, boje> mapa;
	std::regex rx("([^,]*),([^,]*),([^,]*),([^,]*)");
	while (getline(fajl, textLine)) {
		std::smatch result;
		if (regex_match(textLine, result, rx)) {
			mapa[result.str(1)] = { stoi(result.str(2)),stoi(result.str(3)),stoi(result.str(4)) };
			std::cout << mapa[result.str(1)].r << " " << mapa[result.str(1)].g << " " << mapa[result.str(1)].b << "\n";
		
		
		}
	
	
	
	}
	
	//sirina :3*sirina + sirina%8 =   n: ?

	std::ofstream file(s+".bmp",std::ofstream::binary);

	file <<(char) 0x42 << (char)  0x4D;
	int broj_piksela = (int)(leva.lista.size()* (Measure::getMaxTrajanje() / 0.125));
	//int size = (int)(leva.lista.size()* (Measure::getMaxTrajanje() / 0.125)) * 3 + (int)(leva.lista.size()* (Measure::getMaxTrajanje() / 0.125)) / sirina *2;
	int size =  (broj_piksela/sirina+(broj_piksela%sirina!=0))* (3 * sirina + (3 * sirina % 4==0? 0: 4- 3*sirina % 4)) ;
	int sirina_reda = (3 * sirina + (3 * sirina % 4 == 0 ? 0 : 4 - 3 * sirina % 4));
	int offs16 = (size+54) & 0xFF;
	int offs32 = ((size+54) >> 8) & 0xFF;
	file << (char)offs16;
	file << (char)offs32;
	file << (char)((offs32>>8)& 0xFF);
	file << (char)((offs32 >> 16) & 0xFF);
	
	file << (char)0x00;
	file << (char)0x00;
	file << (char)0x00;
	file << (char)0x00;


	file << (char)54;
	file << (char)0x00;
	file << (char)0x00;
	file << (char)0x00;


	file << (char)0x28;
	file << (char)0x00;



	file << (char)0x00;
	file << (char)0x00;// privh 16B

	//sirina slike

	offs16 = sirina & 0xFF;
	offs32 = (sirina >> 8) & 0xFF;


	file << (char)offs16;
	file << (char)offs32;
	file << (char)(offs32>>8);
	file << (char)(offs32 >> 16);

//visina slike
	int visina = broj_piksela / sirina + (broj_piksela%sirina != 0);
	offs16 = (int)(leva.lista.size()*Measure::getMaxTrajanje() / 0.125) / sirina + ((int)(leva.lista.size()*Measure::getMaxTrajanje() / 0.125) % sirina!= 0);       //(size/sirina +(size%sirina == 0 ? 0 : 1)) & 0xFF;
	// offs32 =( (size/sirina +( size%sirina==0? 0:1 )) >> 8) & 0xFF;

	 file << (char)offs16 ;
	 file << (char)(offs16 >> 8);//offs32 ;
	 file << (char)(offs16 >> 16);//0x00 ;
	 file << (char)(offs16 >> 24);//0x00;




	 file << (char)0x01;
	 file << (char)0x00;
	 file << (char)0x18;
	 file << (char)0x00;
	 file << (char)0x00;
	 file << (char)0x00 ;// drugih 16B



	 file << (char)0x00 ;
	 file << (char)0x00;


	 offs16 = size & 0xFF;
	 offs32 = (size >> 8) & 0xFF;


	 file << (char)offs16 ;
	 file << (char)offs32 ;

	 file << (char)(offs32 >> 8) ;
	 file << (char)(offs32 >> 16);

	 file << (char)0x13 ;
	 file << (char)0x0B ;
	 file << (char)0x00;
	 file << (char)0x00 ;

	 file << (char)0x13 ;
	 file << (char)0x0B ;
	 file << (char)0x00 ;
	 file << (char)0x00 ;

	 file << (char)0x00 ;
	 file << (char)0x00 ; //trecih 16B


	 file << (char)0x00 ;
	 file << (char)0x00 ;


	 file << (char)0x00;
	 file << (char)0x00;
	 file << (char)0x00;
	 file << (char)0x00;  //zauzeto 6B



	 char** mat = new char*[visina];
	 for (int i = 0; i < visina; i++) mat[i] = new char[sirina_reda];
	 

	 int red = 0;
	 
  



	 int broj_u_redu = 0;
	 bool cetvrtina = false;
	 bool zajedno = false;
	auto iter_leva = leva.lista.begin();
	auto iter_desna = desna.lista.begin();
	while (iter_leva != leva.lista.end()) {
		auto iter_leva_takt = (*iter_leva)->lista.begin();
		auto iter_desna_takt = (*iter_desna)->lista.begin();

		while (1) {
			int x=255, y=255, z=255;
			int num = 1;
			if (iter_leva_takt != (*iter_leva)->lista.end()) {
				if (typeid(*(*iter_leva_takt)) == typeid(Note)) {

					cetvrtina = static_cast<Note*>(*iter_leva_takt)->getMidiDuration() == 2;
				//izvlacenje iz mapre, racunanje i ubacivanje
					textLine = std::string(1, static_cast<Note*>(*iter_leva_takt)->Oznaka()) +( static_cast<Note*>(*iter_leva_takt)->Povisena() ? "#" : "");
					boje b = mapa[textLine];
					switch (static_cast<Note*>(*iter_leva_takt)->_oktava()) {
					case 2:	b.r -= b.r / 8 * 6;
						b.b -= b.b / 8 * 6;
						b.g -= b.g / 8 * 6;
						break;
					
					case 3:	b.r -= b.r / 8 * 3;
						b.b -= b.b / 8 * 3;
						b.g -= b.g / 8 * 3;
						break;
					
					case 5:	b.r += (255- b.r) / 8 *3;
						b.b += (255-b.b) / 8 * 3;
						b.g += (255-b.g )/ 8 * 3;
						break;
					
					case 6:	b.r -= (255 - b.r) / 8 * 6;
						b.b -= (255 - b.b) / 8 * 6;
						b.g -= (255 - b.g) / 8 * 6;
						break;
					
					
					}
					x = b.r;
					y = b.g;
					z = b.b;
					num = 1;
					zajedno = false;

					if (static_cast<Note*>(*iter_leva_takt)->Zajedno()) {  //ako se svira vise nota odjednom

						iter_leva_takt++;
						zajedno = true;
						

						while (iter_leva_takt != (*iter_leva)->lista.end()) {
						

							if (typeid(*(*iter_leva_takt)) == typeid(Note)) {
								if (!static_cast<Note*>(*iter_leva_takt)->Zajedno()) break;
								//izvlacenje iz mapre, racunanje i ubacivanje
								textLine = std::string(1, static_cast<Note*>(*iter_leva_takt)->Oznaka()) + (static_cast<Note*>(*iter_leva_takt)->Povisena() ? "#" : "");
								 b = mapa[textLine];
								switch (static_cast<Note*>(*iter_leva_takt)->_oktava()) {
								case 2:	b.r -= b.r / 8 * 6;
									b.b -= b.b / 8 * 6;
									b.g -= b.g / 8 * 6;
									break;

								case 3:	b.r -= b.r / 8 * 3;
									b.b -= b.b / 8 * 3;
									b.g -= b.g / 8 * 3;
									break;

								case 5:	b.r += (255 - b.r) / 8 * 3;
									b.b += (255 - b.b) / 8 * 3;
									b.g += (255 - b.g) / 8 * 3;
									break;

								case 6:	b.r -= (255 - b.r) / 8 * 6;
									b.b -= (255 - b.b) / 8 * 6;
									b.g -= (255 - b.g) / 8 * 6;
									break;


								}
								x += b.r; y += b.g; z += b.b; num++;
								iter_leva_takt++;

							}
							else break;
						
						
						
						
						
						
						
						}
					
					
					
					}
					
				}
				else { zajedno = false; num = 1; }
				if(!zajedno)
				iter_leva_takt++;
			
			}
		
			if (iter_desna_takt != (*iter_desna)->lista.end()) {
				if (typeid(*(*iter_desna_takt)) == typeid(Note)) {
					//izvlacenje iz mapre, racunanje i ubacivanje

					cetvrtina = static_cast<Note*>(*iter_desna_takt)->getMidiDuration() == 2;

					////////////////////////////////////////////////////////////////////////////////////////////////////


					textLine = std::string(1, static_cast<Note*>(*iter_desna_takt)->Oznaka()) + (static_cast<Note*>(*iter_desna_takt)->Povisena() ? "#" : "");
					boje b = mapa[textLine];
					switch (static_cast<Note*>(*iter_desna_takt)->_oktava()) {
					case 2:	b.r -= b.r / 8 * 6;
						b.b -= b.b / 8 * 6;
						b.g -= b.g / 8 * 6;
						break;

					case 3:	b.r -= b.r / 8 * 3;
						b.b -= b.b / 8 * 3;
						b.g -= b.g / 8 * 3;
						break;

					case 5:	b.r += (255 - b.r) / 8 * 3;
						b.b += (255 - b.b) / 8 * 3;
						b.g += (255 - b.g) / 8 * 3;
						break;

					case 6:	b.r -= (255 - b.r) / 8 * 6;
						b.b -= (255 - b.b) / 8 * 6;
						b.g -= (255 - b.g) / 8 * 6;
						break;


					}
					x = b.r;
					y = b.g;
					z = b.b;
					
					if (!zajedno || !static_cast<Note*>(*iter_desna_takt)->Zajedno()) {
						
						if (zajedno) {
						
						//ispis u fajl 
						}
						
						x = b.r;
						y = b.g;
						z = b.b;

						
						num = 1;
						zajedno = false;
					}
					else {
						x += b.r;
						y += b.g;
						z += b.b;
						num++;
					
					}

					if (static_cast<Note*>(*iter_desna_takt)->Zajedno()) {  //ako se svira vise nota odjednom

						iter_desna_takt++;
						zajedno = true;


						while (iter_desna_takt != (*iter_desna)->lista.end()) {


							if (typeid(*(*iter_desna_takt)) == typeid(Note)) {
								if (!static_cast<Note*>(*iter_desna_takt)->Zajedno()) break;
								//izvlacenje iz mapre, racunanje i ubacivanje
								textLine = std::string(1, static_cast<Note*>(*iter_desna_takt)->Oznaka()) + (static_cast<Note*>(*iter_desna_takt)->Povisena() ? "#" : "");
								b = mapa[textLine];
								switch (static_cast<Note*>(*iter_desna_takt)->_oktava()) {
								case 2:	b.r -= b.r / 8 * 6;
									b.b -= b.b / 8 * 6;
									b.g -= b.g / 8 * 6;
									break;

								case 3:	b.r -= b.r / 8 * 3;
									b.b -= b.b / 8 * 3;
									b.g -= b.g / 8 * 3;
									break;

								case 5:	b.r += (255 - b.r) / 8 * 3;
									b.b += (255 - b.b) / 8 * 3;
									b.g += (255 - b.g) / 8 * 3;
									break;

								case 6:	b.r -= (255 - b.r) / 8 * 6;
									b.b -= (255 - b.b) / 8 * 6;
									b.g -= (255 - b.g) / 8 * 6;
									break;


								}
								x += b.r; y += b.g; z += b.b; num++;
								iter_desna_takt++;

							}
							else break;







						}



					}

					/////////////////////////////////////////////////////////////





















				}
				else { zajedno = false;  }
				if(!zajedno )iter_desna_takt++;

			}

			//ispis x,y,x u faljl!!!!
			mat[red][broj_u_redu++]= (char)((z & 0xFF) / num);
			mat[red][broj_u_redu++] = (char)((y & 0xFF) / num);
			mat[red][broj_u_redu++] = (char)((x & 0xFF) / num);

			if (broj_u_redu / 3 == sirina) {
				while (broj_u_redu % 4 != 0) {
					mat[red][broj_u_redu++] =(char)0x00;
				}
				broj_u_redu = 0;
				red++;
				}


			if (cetvrtina) {
				mat[red][broj_u_redu++] = (char)((z & 0xFF) / num);
				mat[red][broj_u_redu++] = (char)((y & 0xFF) / num);
				mat[red][broj_u_redu++] = (char)((x & 0xFF) / num);

				if (broj_u_redu / 3 == sirina) {
					while (broj_u_redu % 4 != 0) {
						mat[red][broj_u_redu++] = (char)0x00;
					}
					broj_u_redu = 0;
					red++;
				}

			}

			
			
			/*
			broj_u_redu+=3;
			file << (char)((x & 0xFF) / num);
			file << (char)((y & 0xFF) / num);
			file << (char)((z & 0xFF) / num);

			if (broj_u_redu / 3 == sirina) {

				while (broj_u_redu % 4 != 0) {
					file << (char)0x00;
					broj_u_redu++;
				}
				//file << (char)0x00;
				broj_u_redu = 0;

			}




			if (cetvrtina) {
				broj_u_redu+=3;
				file << (char)((x & 0xFF) / num) ;
				file << (char)((y & 0xFF) / num) ;
				file << (char)((z & 0xFF) / num) ;
			}
			if (broj_u_redu /3 == sirina) {
				
				while (broj_u_redu % 4 != 0) {
					file << (char)0x00;
					broj_u_redu++;
				}
				//file << (char)0x00;
				broj_u_redu = 0;
			
			}
			*/
			cetvrtina = false;
			if (iter_desna_takt == (*iter_desna)->lista.end() && iter_leva_takt == (*iter_leva)->lista.end()) break;
		  
		}
	
		iter_leva++;
		iter_desna++;
	
	}
	if(broj_u_redu)
	 {
		while (broj_u_redu / 3 != sirina) {
			mat[red][broj_u_redu]= (char)0xFF;
			//file << (char)0xFF;
			broj_u_redu++;
		}
		
		//broj_u_redu++;
		while (broj_u_redu % 4 != 0) //umesto 5 je bilo !=
		{
			mat[red][broj_u_redu] = (char)0x00;
			//file << (char)0x00;
			broj_u_redu++;
		}
	
	}
	else red--;


	for (; red >= 0; red--)
		for (int j = 0; j < sirina_reda; j++)
			file << mat[red][j];
		
		
		
	
	
	

	file.close();
	fajl.close();




}
void Kompozicija::promeni_oktavu() {
	std::cout << "Uneti pomeraj\n";
	int pom;
	std::cin >> pom;
	
	for (auto iter : leva.lista) {
	  for (auto iter2 :iter->lista)
		  if (typeid((*iter2)) == typeid(Note)) 
		  
			  static_cast<Note*>(iter2)->promeni_oktavu(pom);
	}

	for (auto iter : desna.lista) {
		for (auto iter2 : iter->lista)
			if (typeid((*iter2)) == typeid(Note))

				static_cast<Note*>(iter2)->promeni_oktavu(pom);
	}



}


std::list<Measure*>::iterator takt_iter_leva;
std::list<Measure*>::iterator takt_iter_desna;
std::list<MusicSymbol*>::iterator nota_iter_leva;
std::list<MusicSymbol*>::iterator nota_iter_desna;
void ispisi_iteratore() {

	std::cout<<"Desni takt:\t" << *(*takt_iter_desna) << std::endl;
	std::cout <<"Levi takt:\t"<< *(*takt_iter_leva) << std::endl;
	std::cout <<"Desna nota:\t"<< *(*nota_iter_desna) << std::endl;
	std::cout <<"Leva nota:\t"<< *(*nota_iter_leva) << std::endl;

}

void Kompozicija::pocni_iteraciju() {
	takt_iter_leva = leva.lista.begin();
	takt_iter_desna = desna.lista.begin();
	nota_iter_desna = (*takt_iter_desna)->lista.begin();
	nota_iter_leva = (*takt_iter_leva)->lista.begin();
	ispisi_iteratore();
	
}

void Kompozicija::sledeci_takt() {
	if (takt_iter_desna == desna.lista.end()) { std::cout << "Nema vise  taktova\n"; return; }
	takt_iter_desna++;
	takt_iter_leva++;
	
	if (takt_iter_desna == desna.lista.end()) { std::cout << "Nema vise  taktova\n"; return; }
	
	nota_iter_desna = (*takt_iter_desna)->lista.begin();
	nota_iter_leva = (*takt_iter_leva)->lista.begin();
	ispisi_iteratore();


}

void Kompozicija::prethodni_takt() {

	if (takt_iter_desna == desna.lista.begin()) { std::cout << "U prvom ste taktu\n"; return; }
	takt_iter_desna--;
	takt_iter_leva--;
	nota_iter_desna = (*takt_iter_desna)->lista.begin();
	nota_iter_leva = (*takt_iter_leva)->lista.begin();
	ispisi_iteratore();


}

void Kompozicija::sledeca_nota_desni_part() {


	if (nota_iter_desna == (*takt_iter_desna)->lista.end()) { std::cout << "Nema vise nota u desnom part-u\n"; return; }
	nota_iter_desna++;
	if (nota_iter_desna == (*takt_iter_desna)->lista.end()) { std::cout << "Nema vise nota u desnom part-u\n"; return; }
	else ispisi_iteratore();
}

void Kompozicija::sledeca_nota_levi_part() {


	if (nota_iter_leva == (*takt_iter_leva)->lista.end()) { std::cout << "Nema vise nota u levom part-u\n"; return; }
	nota_iter_leva++;
	if (nota_iter_leva == (*takt_iter_leva)->lista.end()) { std::cout << "Nema vise nota u levom part-u\n"; return; }
	else ispisi_iteratore();
}

void Kompozicija::prethodna_nota_levi_part() {


	if (nota_iter_leva == (*takt_iter_leva)->lista.begin()) { std::cout << "Prva nota u levom part-u\n"; return; }
	nota_iter_leva--;
	
	ispisi_iteratore();
}


void Kompozicija::prethodna_nota_desni_part() {


	if (nota_iter_desna == (*takt_iter_desna)->lista.begin()) { std::cout << "Prva nota u desnom part-u\n"; return; }
	nota_iter_desna--;

	ispisi_iteratore();
}

void Kompozicija::promeni_oktavu_noti() {
	std::cout << "Pomeraj?\n";
	int pom;
std:cin >> pom;
	std::cout << "Levi/desni iterator?  Unesite l/d\n";
	char c;
	std::cin >> c;
	if (c == 'l') {
	
		if((*nota_iter_leva)->vrsta() == 'n') static_cast<Note*>(*nota_iter_leva)->promeni_oktavu(pom);
	   
	
	} else if ((*nota_iter_desna)->vrsta() == 'n') static_cast<Note*>(*nota_iter_desna)->promeni_oktavu(pom);



}


void Kompozicija::postavi_povisilicu_noti() {
	std::cout << "Povisena d/n?\n";
	char op;
	std::cin >> op;
	std::cout << "Levi/desni iterator?  Unesite l/d\n";
	char c;
	std::cin >> c;
	if (c == 'l') {

		if ((*nota_iter_leva)->vrsta() == 'n') static_cast<Note*>(*nota_iter_leva)->Povisena()=op=='d';


	}
	else if ((*nota_iter_desna)->vrsta() == 'n') static_cast<Note*>(*nota_iter_desna)->Povisena() = op == 'd';


}

void Kompozicija::promenaOktaveSaPromenomRuke() {
	std::cout << "Pomeraj?\n";
	int pom;
std:cin >> pom;
	std::cout << "Levi/desni iterator?  Unesite l/d\n";
	char c;
	std::cin >> c;
	if (c == 'l') {

		if ((*nota_iter_leva)->vrsta() == 'n') {
			Note* n = static_cast<Note*>(*nota_iter_leva);n ->promeni_oktavu(pom);
			if(n->_oktava()>3)
			promenaRuke(nota_iter_leva,desna.lista,false);
		}

	}
	else if ((*nota_iter_desna)->vrsta() == 'n') { 
		Note* n = static_cast<Note*>(*nota_iter_desna);n ->promeni_oktavu(pom);
		if (n->_oktava() <= 3)
		promenaRuke(nota_iter_desna,leva.lista,true);
	}
	ispisi_iteratore();
  

}

void Kompozicija::promenaRuke(std::list<MusicSymbol*>::iterator& iter, std::list<Measure*>& suprotanPart, bool ruka) {
	
	 
	
	std::list<MusicSymbol*>::iterator temp;
	std::list<Measure*>::iterator takt;
	for (auto i = suprotanPart.begin(); i != suprotanPart.end();i++) {
		bool flag = false;
		temp = (*i)->lista.begin(); //iterator liste u taktu
		while (temp != (*i)->lista.end()) {
			if (*temp == (*iter)->naspram) { takt = i; flag = true; break; }
			temp++;
		}
		if (flag)break;
		}

	if (!static_cast<Note*>(*iter)->Zajedno()) {
		if (static_cast<Note*>(*iter)->Podeljena()) {
			Note* n = static_cast<Note*>((*iter)->drugi_deo);
			n->promeni_oktavu( (static_cast<Note*>(*iter))->_oktava()- n->_oktava() );
			auto takt_temp = takt;
			auto temp2 = takt;
			if (!ruka) { temp2 = takt_iter_leva; }
			else temp2 = takt_iter_desna;
			temp2++;
			takt_temp++;
			MusicSymbol* t = *((*temp2)->lista.begin());
			*((*temp2)->lista.begin()) = *((*takt_temp)->lista.begin());
			*((*takt_temp)->lista.begin()) = t;


		}

		MusicSymbol* t = *iter;
		*iter = *temp;
		*temp = t;

	}//AKO NIEJ AKROD JE SVE OVO GORE


}