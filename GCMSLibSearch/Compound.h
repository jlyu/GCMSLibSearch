#pragma once
#include <iostream>

class Compound {

public:
	Compound(void);
	virtual ~Compound(void);

	int _compoundID;
	std::string _compoundName;
	std::string _formula;
	int _massWeight;
	std::string _casNo;
	int _peakCount;
	std::string _peakData;
};

