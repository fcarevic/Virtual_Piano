#pragma once
#ifndef PART_H_
#define PART_H_
#include "Measure.h"
#include <list>
class Part{
	friend class Kompozicija;
	//////////////
	friend class XML;
	
	std::list<Measure*> lista;

	

public:
	void dodaj(Measure* m);
	auto iterator();
	std::string operator()(bool);
	~Part();



};

#endif