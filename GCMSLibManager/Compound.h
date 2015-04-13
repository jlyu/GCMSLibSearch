#pragma once
#include "StdAfx.h" 
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#define INT2STR( x ) dynamic_cast < std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()


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

public:
	std::string _name;
	std::string _cas;
	std::string _formula;
	std::string _massLower;
	std::string _massUpper;
	std::string _idLower;
	std::string _idUpper;
};




// - 导出涉及的自定义类型
struct LibConfig {
	LibConfig(): _minMass1st(0), _minMass2nd(0), _minMass3rd(0), 
		_matchLimitNumber(20), _isUnique(false) { }

	CString _dbPath1st; 
	CString _dbPath2nd; 
	CString _dbPath3rd; 
	int _minMass1st;
	int _minMass2nd;
	int _minMass3rd;
	int _upperMass;
	int _lowerMass;
	int _matchLimitNumber;
	bool _isUnique;
};


class Compound {

public:
	Compound(void):
		_compoundID(0),_compoundName(""),_formula(""),_massWeight(0),
		_casNo(""),_peakCount(0),_maxX(0),_peakData(""),_matchDegree(-1) { }
	
	// 字符串数据转数组
	void str2ints() {
		std::string::size_type i = 0;
		std::string::size_type j = _peakData.find(';');

		while ( j != std::string::npos) {

			std::string strXY = _peakData.substr(i, j-i);
			std::string::size_type w = strXY.find(' ');
			std::string strX = strXY.substr(0, w);
			std::string strY = strXY.substr(w+1, strXY.length());

			_peakDatas.push_back(CPoint(atoi(strX.c_str()), atoi(strY.c_str())));

			i = ++j;
			j = _peakData.find(';', j);
		}
	}

	// 数组数据转字符串
	void ints2str() {
		_peakData = "";
		const int peakDatasSize = _peakDatas.size();
		for (int i = 0; i != peakDatasSize; i++) {
			_peakData += INT2STR(_peakDatas[i].x) + " " + INT2STR(_peakDatas[i].y) + ";";
		}
	}

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

	std::vector<CPoint> _peakDatas; //  _peakData 字符串的数组表示
};
