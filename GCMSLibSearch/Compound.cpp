#include "Compound.h"
#include <string>

Compound::Compound(void):
	_compoundID(0),
    _compoundName(""),
    _formula(""),
    _massWeight(0),
    _casNo(""),
    _peakCount(0),
    _peakData("")
{
}


Compound::~Compound(void)
{
}

void Compound::print() {
	std::cout << "CompoundID:" << _compoundID << std::endl;
	std::cout << "Name:" << _compoundName << std::endl;
	std::cout << "Formula:" << _formula << std::endl;
	std::cout << "MassWeight:" << _massWeight << std::endl;
	std::cout << "CasNo:" << _casNo << std::endl;
	std::cout << "PeakCount:" << _peakCount << std::endl;
}

void Compound::printBrief() {
	std::cout << _compoundID << "\t" << _formula << std::endl;
}