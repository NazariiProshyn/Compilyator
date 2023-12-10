#include "Compiler.h"
#include <sstream>
namespace NMLexicalErrors{
	std::string fileOpenError{ "ERROR CANNOT OPEN FILE: " };
	std::string unkownSymbol{ "Unknown symbol: " };
	std::string unkownOperator{ "Unknown operator: " };
	std::string onLine{ " on line: " };
}

Compiler::Compiler(std::string filename)
{
	infoFile.open(filename);
	nameOfFile = filename;
}

Compiler::~Compiler()
{
	infoFile.close();
}

bool Compiler::LexicalAnalisator()
{
	if(!infoFile.is_open()){
		std::cout << std::endl << NMLexicalErrors::fileOpenError
			<< nameOfFile << std::endl;
		return false;
	}

	std::string dataString{""};
	std::string activeString{ "" };
	std::string typeOfSymbol{ "" };
	std::string activeChar{ "" };
	int lexicalState{0};
	bool checkedChar = false;
	int numOfLine = 0;

	while (std::getline(infoFile, dataString)) {
		++numOfLine;
		lexicalState = 0;
		//std::cout << dataString << std::endl;
		for (size_t i = 0; i < dataString.size(); ++i)
		{
			checkedChar = false;

			if (!checkedChar && lexicalState == 0)
			{
				
				activeChar = dataString[i];
				typeOfSymbol = symbols.typeOfSymbol(activeChar);

				if (typeOfSymbol == "Letter")
				{
					lexicalState = 1;
					activeString = dataString[i];
					checkedChar = true;
				}

				if (typeOfSymbol == "Digit")
				{
					lexicalState = 4;
					activeString = dataString[i];
					checkedChar = true;
				}

				if (typeOfSymbol == "SpecSymbols")
				{
					lexicalState = 11;
					activeString = dataString[i];
					checkedChar = true;
				}

				if (typeOfSymbol == "WhiteSpace")
				{
					checkedChar = true;
				}

				if (typeOfSymbol == "Dot")
				{
					lexicalState = 5;
					activeString = dataString[i];
					checkedChar = true;
				}

				if (typeOfSymbol == "Comment")
				{
					checkedChar = true;
					break;
				}

				if (typeOfSymbol == "Error")
				{
					std::cout << std::endl << NMLexicalErrors::unkownSymbol
						<< dataString[i] << NMLexicalErrors::onLine << numOfLine << std::endl;
					return false;
				}


			}

			if (!checkedChar && lexicalState == 1) {

				activeChar = dataString[i];
				typeOfSymbol = symbols.typeOfSymbol(activeChar);

				if (typeOfSymbol == "Letter")
				{
					activeString += dataString[i];
					checkedChar = true;
				}

				else if (typeOfSymbol == "Digit")
				{
					activeString += dataString[i];
					checkedChar = true;
				}

				else if (typeOfSymbol == "Error")
				{
					std::cout << std::endl << NMLexicalErrors::unkownSymbol
						<< dataString[i] << std::endl;
					return false;
				}

				else
				{
					identWord(activeString, numOfLine);
					lexicalState = 0;
					--i;
				}

			}

			if (!checkedChar && lexicalState == 4) {

				activeChar = dataString[i];
				typeOfSymbol = symbols.typeOfSymbol(activeChar);

				if (typeOfSymbol == "Digit")
				{
					activeString += dataString[i];
					checkedChar = true;
				}
				else if (typeOfSymbol == "Dot")
				{
					lexicalState = 5;
					activeString += dataString[i];
					checkedChar = true;
				}
				else if (typeOfSymbol == "Error")
				{
					std::cout << std::endl << NMLexicalErrors::unkownSymbol
						<< dataString[i] << std::endl;
					return false;
				}
				else
				{
					tables.addTableOfConst(activeString, numOfLine, "int");
					lexicalState = 0;
					--i;
				}
			}

			if (!checkedChar && lexicalState == 5) {

				activeChar = dataString[i];
				typeOfSymbol = symbols.typeOfSymbol(activeChar);

				if (typeOfSymbol == "Digit")
				{
					activeString += dataString[i];
					checkedChar = true;
				}
				else if (typeOfSymbol == "Error")
				{
					std::cout << std::endl << NMLexicalErrors::unkownSymbol
						<< dataString[i] << std::endl;
					return false;
				}
				else
				{
					tables.addTableOfConst(activeString, numOfLine, "float");
					lexicalState = 0;
					--i;
				}
			}

			if (!checkedChar && lexicalState == 11) {

				activeChar = dataString[i];
				typeOfSymbol = symbols.typeOfSymbol(activeChar);

				if (typeOfSymbol == "SpecSymbols")
				{
					activeString += dataString[i];
					checkedChar = true;
				}
				else if (typeOfSymbol == "Error")
				{
					std::cout << std::endl << NMLexicalErrors::unkownSymbol
						<< dataString[i] << std::endl;
					return false;
				}
				else
				{
					if (identOperator(activeString, numOfLine))
					{
						lexicalState = 0;
						--i;
					}
					else
					{
						std::cout << std::endl << NMLexicalErrors::unkownSymbol
							<< activeString << NMLexicalErrors::onLine << numOfLine << std::endl;
						return false;
					}
					

				}
			}


			if (i == dataString.size() - 1) {
				if (lexicalState == 1) {
					identWord(activeString, numOfLine);
				}

				if (lexicalState == 4) {
					tables.addTableOfConst(activeString, numOfLine, "int");
				}

				if (lexicalState == 5) {
					tables.addTableOfConst(activeString, numOfLine, "float");
				}

				if (lexicalState == 11) {
					if (identOperator(activeString, numOfLine))
					{

					}
					else
					{
						std::cout << std::endl << NMLexicalErrors::unkownSymbol
							<< activeString << NMLexicalErrors::onLine << numOfLine << std::endl;
						return false;
					}
				}
				activeString = "";
			}
		}
	}

	return true;

}

void Compiler::prinTables()
{
	tables.printTableOfConst();
	tables.printTableOfId();
	tables.printTableOfSymb();
}

bool Compiler::SyntaxAnalisator()
{
	
	try
	{
		identParingList();
	}
	catch (const std::exception& e)
	{
		std::cerr << "\nEXCEPTION IN SYNTAX ANALISATOR: " << e.what() << '\n';
		return false;
	}
	return true;
}

void Compiler::identWord(std::string info, int line)
{
	std::string tempString = symbols.typeOfIdentificator(info);
	if(tempString == "None") {
		tables.addTableOfId(info, line, "ident");
	}
	else
	{
		tables.addTableOfSymb(info, line, tempString);
	}
	
}

bool Compiler::identOperator(std::string info, int line)
{
	std::string tempString = symbols.typeOfIdentificator(info);
	if (tempString == "None") {
		return false;
	}
	else
	{
		tables.addTableOfSymb(info, line, tempString);
		return true;
	}
}

bool Compiler::checkTokenAndValue(std::string token, std::string value)
{
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		std::string error = " Unexpected end of programm: Out of range table of Symbols\n Expected: (";
		error += token + ", " + value + ")";
		throw std::runtime_error(error);
		return false;
	}
	if (token != tables.tableOfSymb[activeLineOfSymbTable].token ||
		value!= tables.tableOfSymb[activeLineOfSymbTable].value) {
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nExpected: (" + token + ", " + value + ")";
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
		return false;


	}
	std::cout << "Check Token. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	return true;
}

bool Compiler::checkToken(std::string token)
{
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		std::string error = " Unexpected end of programm: Out of range table of Symbols\n Expected: ";
		error += token;
		throw std::runtime_error(error);
		return false;
	}
	if (token != tables.tableOfSymb[activeLineOfSymbTable].token) {
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nExpected: (" + token + ")";
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
		return false;

		
	}
	std::cout << "Check Token. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	return true;
}

bool Compiler::checkTokenWithoutExc(std::string token)
{
	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		std::string error = " Unexpected end of programm: Out of range table of Symbols\n Expected: ";
		error += token;
		throw std::runtime_error(error);
		return false;
	}
	if (token != tables.tableOfSymb[activeLineOfSymbTable].token) {
		return false;
	}
	std::cout << "Check Token. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	return true;
}

bool Compiler::checkValueWithoutExc(std::string token, std::string value)
{
	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		std::string error = " Unexpected end of programm: Out of range table of Symbols\n Expected: ";
		error += token;
		throw std::runtime_error(error);
		return false;
	}
	if (value != tables.tableOfSymb[activeLineOfSymbTable].value) {
		return false;
	}
	std::cout << "Check Token. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	return true;
}

void Compiler::identParingList()
{
	while (activeLineOfSymbTable < tables.tableOfSymb.size() - 1)
	{
		if (tables.tableOfSymb[activeLineOfSymbTable].token == "ident")
		{
			std::cout << "Parsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			checkTokenAndValue("assign_op", "<-");

			identExpresion();
		}
		else if (tables.tableOfSymb[activeLineOfSymbTable].token == "if_op")
		{
			std::cout << "\nParsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			identIfExp();
			
		}
		else if (tables.tableOfSymb[activeLineOfSymbTable].token == "keyword")
		{
			std::cout << "\nParsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			identKeyWExp();

		}
		else
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nUnexpected in Parsing List: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
			throw std::runtime_error(error);
		}

		
		//keyword
	}
}

void Compiler::identRecParingList()
{
	while (activeLineOfSymbTable < tables.tableOfSymb.size() - 1)
	{
		if (tables.tableOfSymb[activeLineOfSymbTable].token == "ident")
		{
			std::cout << "Recursiv Parsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			checkTokenAndValue("assign_op", "<-");

			identExpresion();
		}
		else if (tables.tableOfSymb[activeLineOfSymbTable].token == "if_op")
		{
			std::cout << "\nRecursiv Parsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			identIfExp();

		}
		else if (tables.tableOfSymb[activeLineOfSymbTable].token == "keyword")
		{
			std::cout << "\nRecursiv Parsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			identKeyWExp();

		}
		else if (checkValueWithoutExc("block_op", "}"))
		{
			break;

		}
		else
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nUnexpected in recursive Parsing List: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
			throw std::runtime_error(error);
		}

		
		//keyword
	}
}

void Compiler::identExpresion()
{
	
	++activeLineOfSymbTable;
	if (checkTokenWithoutExc("add_op"))
	{
		++activeLineOfSymbTable;
	}
	if (!identBoolean()) {//
		if (!identArithm())
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nUnexpected identificator expression: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
			throw std::runtime_error(error);
		}
	}

}

bool Compiler::identBoolean()
{
	std::cout << "\nStart: identBoolean\n";
	int temp = activeLineOfSymbTable;
	if (checkTokenWithoutExc("boolval")) {
		++activeLineOfSymbTable;
		std::cout << "\n\Success: identBoolean\n\n";
		return true;
	}
	//x <- 5+x>=y
	while (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float")) {
		if (activeLineOfSymbTable >= tables.tableOfSymb.size() - 1)
		{
			std::cout << "False: identBoolean\n\n";
			activeLineOfSymbTable = temp;
			return false;
		}
		++activeLineOfSymbTable;
		if (!checkTokenWithoutExc("add_op") && !checkTokenWithoutExc("mult_op") && !checkTokenWithoutExc("exp_op")) {

			if (!checkTokenWithoutExc("rel_op")) {
				std::cout << "False: identBoolean\n\n";
				activeLineOfSymbTable = temp;
				return false;
			}
			else
			{
				++activeLineOfSymbTable;
				break;
			}
		}
		++activeLineOfSymbTable;
	}



	while (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float")) {
		++activeLineOfSymbTable;
		if (activeLineOfSymbTable >= tables.tableOfSymb.size())
		{
			std::cout << "Success: identBoolean\n\n";
			return true;
		}
		if (!checkTokenWithoutExc("add_op") && !checkTokenWithoutExc("mult_op") && !checkTokenWithoutExc("exp_op")) {
			std::cout << "Success: identBoolean\n\n";
			return true;
		}
		++activeLineOfSymbTable;
	}

	std::cout << "False: identBoolean\n\n";
	return false;
}

bool Compiler::identArithm()
{
	std::cout << "\nStart: identArithm\n";
	while (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float")) {
		++activeLineOfSymbTable;
		if (activeLineOfSymbTable >= tables.tableOfSymb.size())
		{
			std::cout << "Success: identArithm\n\n";
			return true;
		}
		if (!checkTokenWithoutExc("add_op") && !checkTokenWithoutExc("mult_op") && !checkTokenWithoutExc("exp_op")) {
			std::cout << "Success: identArithm\n\n";
			return true;
		}
		++activeLineOfSymbTable;
	}
		
	std::cout << "\nFalse: identArithm\n\n";
	return false;
}

void Compiler::identIfExp()
{
	if (checkValueWithoutExc("if_op", "if") || checkValueWithoutExc("if_op", "elseif")) {
		checkTokenAndValue("brackets_op", "(");
		++activeLineOfSymbTable;
		identBoolean();
		if (checkTokenWithoutExc("rel_op"))
		{
			++activeLineOfSymbTable;
			identBoolean();
			
		}
		--activeLineOfSymbTable;
		checkTokenAndValue("brackets_op", ")");
		++activeLineOfSymbTable;
	}
	if (checkValueWithoutExc("if_op", "else"))
	{
		++activeLineOfSymbTable;
	}
	if (checkValueWithoutExc("block_op", "{")) {
		++activeLineOfSymbTable;
		identRecParingList();
		--activeLineOfSymbTable;
		checkTokenAndValue("block_op", "}");
		++activeLineOfSymbTable;
	}
}

void Compiler::identKeyWExp()
{
	/*
		{"keyword","readline"},
		{"keyword","cat"},
		{"keyword","for"},
		{"keyword","in"},
		{"keyword","next"},
		{"keyword","break"},
	*/
}
