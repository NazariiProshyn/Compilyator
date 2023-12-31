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
	bool checkTokenAndValueWithoutExp(std::string token, std::string value);

	void identParingList();
	void identRecParingList();
	void identExpresion();
	void identExp();
	void assExp();
	void opScope();
	void clScope();
	void relExp();
	void identIfExp();
	void identKeyWExp();
	void identFor();
	int ops{ 0 };
	std::vector<std::string> activeScope;
	std::vector<std::string> afterScope;
	std::vector<std::string> activeIdents;
	std::string activeIdent;


	bool checkElementInVector(bool info, std::string elem);
	bool checkIdentsInVector(std::string elem);
	bool isPoriv = false;
};

