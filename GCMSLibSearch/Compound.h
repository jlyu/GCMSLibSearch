#pragma once

//#include "StdAfx.h" //

#include <iostream>

class Compound {

public:

	Compound(void);
	virtual ~Compound(void);

	void print();
	void printBrief();


public:
	int _compoundID;
	std::string _compoundName;
	std::string _formula;
	int _massWeight;
	std::string _casNo;
	int _peakCount;
	std::string _peakData;
};

