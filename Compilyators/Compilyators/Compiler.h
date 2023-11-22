#pragma once
#include <fstream>
#include "Symbols.h"
#include "Tables.h"
class Compiler
{
public:
	Compiler(std::string filename);
	~Compiler();
	//Lexical
	bool LexicalAnalisator();
	void prinTables();
private:
	//Lexical
	std::fstream infoFile;
	std::string nameOfFile{ "" };
	Symbols symbols;
	void identWord(std::string info, int line);
	bool identOperator(std::string info, int line);
	Tables tables;
};

