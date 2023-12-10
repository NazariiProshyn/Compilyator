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
	bool SyntaxAnalisator();
private:
	//Main
	Tables tables;

	//Lexical
	std::fstream infoFile;
	std::string nameOfFile{ "" };
	Symbols symbols;
	void identWord(std::string info, int line);
	bool identOperator(std::string info, int line);
	

	//Syntax
	size_t activeLineOfSymbTable{ 0 };
	bool checkTokenAndValue(std::string token, std::string value);
	bool checkToken(std::string token);
	bool checkTokenWithoutExc(std::string token);
	bool checkValueWithoutExc(std::string token, std::string value);
	void identParingList();
	void identRecParingList();
	void identExpresion();
	bool identBoolean();
	bool identArithm();
	void identIfExp();
	void identKeyWExp();
};

