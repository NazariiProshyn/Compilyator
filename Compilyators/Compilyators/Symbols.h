#pragma once
#include<iostream>
#include<vector>
class Symbols
{
public:
	Symbols() = default;
	~Symbols() = default;
private:
	std::vector<char> Letters;
	std::vector<char> Digit;

	std::vector<char> SpecSymbols;
	std::vector<std::string> WhiteSpace;
	std::vector<std::string> EndOfLine;

};

