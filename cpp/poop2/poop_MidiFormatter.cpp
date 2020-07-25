#include "C:\Users\CAR\Desktop\klavir\poop\src\MidiFormatter.h"
#include "Kompozicija.h"
#include "MIDITable.h"
JNIEXPORT void JNICALL Java_poop_MidiFormatter_parsiraj(JNIEnv * env, jclass thisObj, jstring filePath) {
	Kompozicija c;
	MIDI::createInstance("map.csv");

	const char* putanja = env->GetStringUTFChars(filePath, NULL);
	Measure::setMaxTrajanje(0.5);
	c.ucitaj("your_composition.txt");
	c.MIDI_file(putanja);







}