#pragma once

//#include "StdAfx.h" //

#include <iostream>

class Peak {
public:
	Peak(void):_peakCount(0), _peakData("") { }
	virtual ~Peak(void) { }

	int _peakCount;
	std::string _peakData;
};

class PeakPoint {
public:
	PeakPoint():_compoundID(0), _x(0), _y(0) { }

    int _compoundID;
	int _x;
	int _y;
};

class PeakXY {
public:
	PeakXY(int capcity = 800):_compoundID(0) {
			_x = new unsigned int[capcity];
			_y = new float[capcity];
	}
	~PeakXY() {
		if (_x != NULL && _y != NULL) {
			delete [] _x; _x = NULL;
			delete [] _y; _y = NULL;
		}
	}

	int _compoundID;
	unsigned int *_x;
	float *_y;
};


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

