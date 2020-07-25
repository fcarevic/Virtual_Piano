#include "Measure.h"
#include "Note.h"
double Measure::trajanje = 0;

int Measure::brojilac() {
	return (int)(trajanje / 0.125) % 2 == 0 ? (trajanje / 0.125) / 2 : (trajanje / 0.125);
}

int Measure::imenilac() {
	return (int)(trajanje / 0.125) % 2 == 0 ? 4 : 8;

}
void Measure::dodaj(MusicSymbol* m) {
	lista.push_back(m);
	trenutno_trajanje += m->getDuration().getDuration() == Duration::DURATION::_1_4 ? 0.25 : 0.125;
}

void Measure::dodaj(MusicSymbol* m,bool zajedno) {
	lista.push_back(m);
	if(!zajedno)
	trenutno_trajanje += m->getDuration().getDuration() == Duration::DURATION::_1_4 ? 1.0 / 4 : 1.0 / 8;
}


void Measure::setNadovezuje(bool b) {
	nadovezuje_se = b;
}

void Measure::setMaxTrajanje(double d) {
	trajanje = d;
}

double Measure::MaxTrajanje() {
	return trajanje;
}
double Measure::TrenutnoTrajanje() {
	return trenutno_trajanje;
}



Measure::~Measure() {
	for (auto iter = lista.begin(); iter != lista.end();iter++) {
		delete *iter;
		
	}//>>>>>>>>>>>>>>>>???????????????????????????

}

std::string Measure::toString () {
	std::string os = "";
	for (auto i : lista) {

		os += i->ispis();
	}
	return os;
}


std::ostream& operator << (std::ostream& os, const Measure& m) {
   
	for (auto i : m.lista) {
		
			os << *i;
	}
	return os;
}




std::string Measure::operator()(bool prvic) {
	std::string s;
	if (!prvic)s += "<measure>\n";
	bool prvi = false;
	int poslednji;
	MusicSymbol* ms = nullptr;
	for (auto iter2 : lista) {

		if (typeid(*iter2) == typeid(Note))
			if (static_cast<Note*>(iter2)->Podeljena())
				ms = iter2;
	}
	int num;

	for (auto iter : lista) {

		if (typeid(*iter) == typeid(Note)) {
			if (static_cast<Note*>(iter)->Zajedno()) {



				num = iter == lista.back() ? 1 : iter == ms ? 0 : 2;
				if (!prvi) {
					s += iter->operator()(true, num);
					prvi = true;
				}
				else s += iter->operator()(false, num);
				if (static_cast<Note*>(iter)->isPoslednjaAkord())prvi = false;
			}
			else {
				prvi = false;
				num = iter == lista.back() ? 1 : iter == ms ? 0 : 2;
				s += iter->operator()(true, num);
			}



		}
		else {
			s += iter->operator()(true, 0);
			prvi = false;
		}

	}
	s += "</measure>\n";

	return s;

}


	double Measure::getMaxTrajanje() {
		return trajanje;
	}

