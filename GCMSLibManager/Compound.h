#pragma once
#include "StdAfx.h" 
#include <iostream>
#include <string>

class Peak {
public:
	Peak(void):_compoundID(0), _peakCount(0), _peakData(""), _matchDegree(-1) { }
	virtual ~Peak(void) { }

	int _compoundID;
	int _peakCount;
	std::string _peakData;
	int _matchDegree; //匹配度
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
	Compound(void):
			_compoundID(0),
			_compoundName(""),
			_formula(""),
			_massWeight(0),
			_casNo(""),
			_peakCount(0),
			_maxX(0),
			_peakData(""),
			_matchDegree(-1) { }
	~Compound(void) { }

	void print() {
		std::cout << "CompoundID:" << _compoundID << std::endl;
		std::cout << "Name:" << _compoundName << std::endl;
		std::cout << "Formula:" << _formula << std::endl;
		std::cout << "MassWeight:" << _massWeight << std::endl;
		std::cout << "CasNo:" << _casNo << std::endl;
		std::cout << "PeakCount:" << _peakCount << std::endl;
	}
	void printBrief() { std::cout << _compoundID << "\t" << _formula << std::endl; }

public:
	int _compoundID;
	std::string _compoundName;
	std::string _formula;
	int _massWeight;
	std::string _casNo;
	int _peakCount;
	int _maxX; //最大质量数
	std::string _peakData;
	int _matchDegree; //匹配度
};

class LibConfig {
public:
	LibConfig():_dbPath1st(""), _dbPath2nd(""), _dbPath3rd(""), 
		_minMass1st(0), _minMass2nd(0), _minMass3rd(0), 
		_matchLimitNumber(20), 
		_isUnique(false) { }
	~LibConfig() { }

	std::string _dbPath1st; 
	std::string _dbPath2nd; 
	std::string _dbPath3rd; 
	int _minMass1st;
	int _minMass2nd;
	int _minMass3rd;
	int _upperMass;
	int _lowerMass;
	int _matchLimitNumber;
	bool _isUnique;
};

class SearchPara {
public:
	SearchPara():_name(""), _cas(""), _formula(""), 
		_massLower(""), _massUpper(""), _idLower(""), _idUpper("") {}
	~SearchPara() { }
	bool isEmpty() const {
		if ((_name == "") && (_cas == "") && (_formula == "") && 
			(_massLower == "") && (_massUpper == "") && (_idLower == "") && (_idUpper == "") ) {
				return true;
		}
		return false;
	}
	std::string parseWhereQuery() const {
		if (isEmpty()) return "";

		/*
		SELECT * FROM Compound WHERE
								( CompoundID >= 31 AND CompoundID <= 34 ) AND
								( MassWeight >= 18 AND MassWeight <= 60 ) AND
								CompoundName = 'Water' AND
								CasNo = '7782-39-0' AND
								Formula = 'C20H21ClN2O5' AND
		*/

		std::string query = "WHERE";

		if (_idLower != "") { query += " ( CompoundID >= " + _idLower + " AND CompoundID <= " + _idUpper + " ) AND"; }
		if (_massLower != "") { query += " ( MassWeight >= " + _massLower + " AND MassWeight <= " + _massUpper + " ) AND"; }
		if (_name != "") { query += " CompoundName = '" + _name + "' AND"; }
		if (_cas != "") { query += " CasNo = '" + _cas + "' AND"; }
		if (_formula != "") { query += " Formula = '" + _formula + "' AND"; }
							   
		return query.substr(0, query.size() - 3);
		
	}

	std::string _name;
	std::string _cas;
	std::string _formula;
	std::string _massLower;
	std::string _massUpper;
	std::string _idLower;
	std::string _idUpper;
};