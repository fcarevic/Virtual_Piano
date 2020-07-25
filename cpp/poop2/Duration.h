#pragma once
#ifndef DURATION_H
#define DURATION_H
#include <string>
class Duration {
	
	//double duration;

public:
	enum DURATION { _1_4, _1_8};
	Duration(DURATION d):duration(d) {}
	DURATION getDuration() const { return duration; }
	void setDuration(DURATION d) { duration = d; }
	
private:
	DURATION duration;
};



#endif
