#include "Part.h"


void Part::dodaj(Measure* m) {
	lista.push_back(m);
}
auto Part::iterator() {
	return lista.begin();
}

std::string Part::operator()(bool leva) {
	std::string s;
	if (!leva)s = "<part id=\"Right\">\n<measure>\n<attributes>\n<divisions>2</divisions>\n<time>\n<beats>"+to_string(Measure::brojilac())+"</beats>\n<beat-type>"+ to_string(Measure::imenilac())+"</beat-type>\n</time>\n<clef>\n<sign>G</sign>\n<line>2</line>\n</clef>\n</attributes>\n";
	else s = "<part id=\"Left\">\n<measure>\n<attributes>\n<divisions>2</divisions>\n<time>\n<beats>"+ to_string(Measure::brojilac())+"</beats>\n<beat-type>"+ to_string(Measure::imenilac()) +"</beat-type>\n</time>\n<clef>\n<sign>F</sign>\n<line>4</line>\n</clef>\n</attributes>\n";
		
	bool prvi = true;
		for (auto iter : lista) {
			s += iter->operator()(prvi);
			prvi = false;
		
		}
		s += "</part>\n";
		return s;


}

Part::~Part() {

	for (auto iter = lista.begin(); iter != lista.end(); iter++) {
		delete *iter;
	}


}