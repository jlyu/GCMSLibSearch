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
	PeakPoint(int id):_compoundID(id), _x(0), _y(0) { }

    int _compoundID;
	int _x;
	int _y;
};

class FilterPoint {
public:
	FilterPoint(): _peakPoint(), _yrx(0), _rank(0) { }
	FilterPoint(int id): _peakPoint(id), _yrx(0), _rank(0) { }

	PeakPoint _peakPoint;
	int _yrx;
	int _rank;

	void printXY() { std::cout <<"("<< _peakPoint._x << "," << _peakPoint._y <<")"<< std::endl; }
	void printY() { std::cout << _peakPoint._y << std::endl; }
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
	int _maxX; //最大质量数
	std::string _peakData;
};

