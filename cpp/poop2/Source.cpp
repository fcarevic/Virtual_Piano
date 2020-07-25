
#include "MIDITable.h"
#include "Kompozicija.h"




int main() {
	MIDI::createInstance("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\map.csv");
	Kompozicija* k = new Kompozicija();
	Measure::setMaxTrajanje(0.375);
  std:string putanja;
	bool izmenjeno = false;
	while (1) {
		int option;
		std::cout << "1)Ucitaj kompoziciju\n2)Ispisi kompoziciju\n4)Promeni oktavu cele kompozicije\n5)Iteriraj kroz kompoziciju\n6)Promeni takt kompozicije\n7)Eksportuj\n";
		std::cin >> option;
		int iter_opt = 0;
		char ruka;
		switch (option) {
		case 1:
			izmenjeno = false;
			delete k;
			std::cout << "Uneti ime fajla bez  ekstenzije .txt\n";
			std::cin >> putanja;
			k = new Kompozicija();
			k->ucitaj("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\input\\"+putanja+".txt");//MORA LI DA SE ESKEJPUJU U STRINGU????
			break;

		case 2: std::cout << *k; break;

		case 4: k->promeni_oktavu();
			izmenjeno = true;
			break;
		case 5:
			k->pocni_iteraciju();
			while (iter_opt != -1) {
				
				std::cout << "\t1)Prethodni takt\n\t2)Sledeci takt\n\t3)Prethodna nota\n\t4)Sledeca nota\n\t5)Postavi povisilicu noti\n\t6)Postavi oktavu noti\n\t-1)Nazad";
				std::cin >> iter_opt;
				
				switch (iter_opt)
				{
				case 1:
					k->prethodni_takt();
					break;

				case 2:
					k->sledeci_takt();
					break;

				case 3:
					
					std::cout << "\nKoja ruka? l/d\n";
					std::cin >> ruka;
					if (ruka == 'l') k->prethodna_nota_levi_part();
					else k->prethodna_nota_desni_part();
				    break;
				case 4:
					
					std::cout << "\nKoja ruka? l/d\n";
					std::cin >> ruka;
					if (ruka == 'd') k->sledeca_nota_desni_part();
					else k->sledeca_nota_levi_part();

					break;

				case 5:
					izmenjeno = true;
					k->postavi_povisilicu_noti();
					break;

				case 6:
					izmenjeno = true;
					//k->promeni_oktavu_noti();
					k->promenaOktaveSaPromenomRuke();
					break;

					
				}

			
			}
			iter_opt = 0;


			break;

		case 6:
			izmenjeno = true;
			std::cout << "Uneti takt u broju osmina\n";
			//double takt;
			int br;
			std::cin >> br;
			Measure::setMaxTrajanje(br*0.125);
			delete k;
			k = new Kompozicija();
			k->ucitaj("C:\\Users\\CAR\\Desktop\\13S112POOP_PZ1_1819_prilog\\input\\"+putanja + ".txt");
			break;

		case 7:
			k->BMP(putanja);
			k->operator()(putanja);
			k->MIDI_file(putanja);
			break;



				case -1: 
					if (izmenjeno)
						std::cout << "Sacuvati izmene?\nd/n\n";
					char c = 'a';
					std::cin >> c;
					if (c == 'd') {
						k->BMP(putanja);
						k->operator()(putanja);
						k->MIDI_file(putanja);

					
					
					}
					
					return 0;
		
		}
	
	
	
	
	
	
	
	
	
	
	}



	



	/*MIDI::createInstance("C:\\Users\\Car\\Desktop\\poop_projekat\\13S112POOP_PZ1_1819_prilog\\map.csv");
	Kompozicija k ;
	Measure::setMaxTrajanje(0.5);
	k.ucitaj("C:\\Users\\Car\\Desktop\\poop_projekat\\13S112POOP_PZ1_1819_prilog\\input\\ode_to_joy.txt");
	std::cout <<  k<<std::endl;
	k.MIDI_file();
	k();
	k.BMP("primer.bmp");

	/*std::string s = "C:\\Users\\Administrator\\Desktop\\a.txt";
	std::ifstream file(s);
	std::string textLine;
	std::regex rx("([^\\[]*){0,1}(\\[[^\\]]*\\]){0,1}");
	int i = 0;
	while (getline(file, textLine)) {
		std::sregex_iterator begin(textLine.begin(), textLine.end(), rx);
		std::sregex_iterator end;
		while (begin != end) {
			std::smatch result = *begin;
			if (result.str(0) == "") { begin++; continue; }
			std::cout << i << ": " << result.str(0) << std::endl;
			std::cout << i << ": " << result.str(1) << std::endl;
			std::cout << i << ": " << result.str(2) << std::endl;
			i++; begin++;
		
		}



		
	
	
	
	
	
	
	
	}


	file.close();

	*/

	return 0;
}