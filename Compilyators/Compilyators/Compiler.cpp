#include "Compiler.h"
#include <sstream>

namespace NMLexicalErrors {
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
	if (!infoFile.is_open()) {
		std::cout << std::endl << NMLexicalErrors::fileOpenError
			<< nameOfFile << std::endl;
		return false;
	}

	std::string dataString{ "" };
	std::string activeString{ "" };
	std::string typeOfSymbol{ "" };
	std::string activeChar{ "" };
	int lexicalState{ 0 };
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
	if (tempString == "None") {
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
		value != tables.tableOfSymb[activeLineOfSymbTable].value) {
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nExpected: (" + token + ", " + value + ")";
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
		return false;


	}
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
	return true;
}

bool Compiler::checkValueWithoutExc(std::string token, std::string value)
{
	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		
	}
	else if (value != tables.tableOfSymb[activeLineOfSymbTable].value) {
		return false;
	}
	return true;
}

bool Compiler::checkTokenAndValueWithoutExp(std::string token, std::string value)
{

	if (activeLineOfSymbTable >= tables.tableOfSymb.size()) {
		std::string error = " Unexpected end of programm: Out of range table of Symbols\n Expected: (";
		error += token + ", " + value + ")";
		throw std::runtime_error(error);
		return false;
	}
	if (token != tables.tableOfSymb[activeLineOfSymbTable].token ||
		value != tables.tableOfSymb[activeLineOfSymbTable].value) {
		return false;
	}

	return true;
}

void Compiler::identParingList()
{
	while (activeLineOfSymbTable < tables.tableOfSymb.size()-1)
	{
		if (tables.tableOfSymb[activeLineOfSymbTable].token == "ident")
		{
			std::cout << "\nParsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			checkTokenAndValue("assign_op", "<-");
			std::cout << "\nParsing list. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
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
	}
}

void Compiler::identRecParingList()
{
	while (activeLineOfSymbTable < tables.tableOfSymb.size())
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
	std::cout << "\nStart identifacation expression\n";
	++activeLineOfSymbTable;
	if (checkTokenWithoutExc("add_op")) {
		assExp();
	}
	else if (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float") || checkTokenWithoutExc("boolval")) {
		identExp();
	}
	else if(checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		++ops;
		opScope();
	}
	else if (checkTokenAndValueWithoutExp("keyword", "readline")) {
		std::cout << "\nCheck keyword: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
		++activeLineOfSymbTable;
	}
	else
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected identificator expression: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}

}



void Compiler::identExp()
{
	std::cout << "\nCheck ident or value: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size())
	{
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
		std::cout << "\nSuccess: identArithm\n\n";
	}
	else if (checkTokenWithoutExc("add_op") || checkTokenWithoutExc("mult_op") || checkTokenWithoutExc("exp_op") || checkTokenWithoutExc("boolval")) {
		assExp();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		++ops;
		opScope();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", ")"))
	{
		if (ops > 0) {
			--ops;
			clScope();
		}
	}
	else if (checkTokenWithoutExc("rel_op"))
	{
		relExp();
	}
	else {
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
	}
}

void Compiler::assExp()
{
	std::cout << "\nCheck operators: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size())
	{
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
		std::cout << "\nSuccess: identArithm\n\n";
	}
	else if (checkTokenWithoutExc("add_op") || checkTokenWithoutExc("mult_op") || checkTokenWithoutExc("exp_op") || checkTokenWithoutExc("boolval")) {
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else if (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float")) {
		identExp();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		++ops;
		opScope();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", ")"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else if (checkTokenWithoutExc("rel_op"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else {
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
	}
}

void Compiler::opScope()
{
	std::cout << "\nCheck brackets_op: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size())
	{
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
		std::cout << "\nSuccess: identArithm\n\n";
	}
	else if (checkTokenWithoutExc("add_op")) {
		assExp();
	}
	else if (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float") || checkTokenWithoutExc("boolval")) {
		identExp();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		++ops;
		opScope();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", ")"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else if (checkTokenWithoutExc("rel_op"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else {
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
	}
}

void Compiler::clScope()
{
	std::cout << "\nCheck brackets_op: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size())
	{
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
		std::cout << "\nSuccess: identArithm\n\n";
	}
	else if (checkTokenWithoutExc("add_op") || checkTokenWithoutExc("mult_op") || checkTokenWithoutExc("exp_op")) {
		assExp();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", ")"))
	{
		if (ops > 0) {
			--ops;
			clScope();
		}
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else if (checkTokenWithoutExc("rel_op"))
	{
		relExp();
	}
	else {
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
	}
}

void Compiler::relExp()
{
	std::cout << "\nCheck brackets_op: (" << tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value << ")";
	++activeLineOfSymbTable;
	if (activeLineOfSymbTable >= tables.tableOfSymb.size())
	{
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
		std::cout << "\nSuccess: identArithm\n\n";
	}
	else if (checkTokenWithoutExc("add_op")) {
		assExp();
	}
	else if (checkTokenWithoutExc("ident") || checkTokenWithoutExc("int") || checkTokenWithoutExc("float") || checkTokenWithoutExc("boolval")) {
		identExp();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", "("))
	{
		++ops;
		opScope();
	}
	else if (checkTokenAndValueWithoutExp("brackets_op", ")"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else if (checkTokenWithoutExc("rel_op"))
	{
		std::string error = "In line: ";
		error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
		error += "\nUnexpected: (" + tables.tableOfSymb[activeLineOfSymbTable].token + ", " + tables.tableOfSymb[activeLineOfSymbTable].value + ")";
		throw std::runtime_error(error);
	}
	else {
		if (ops != 0)
		{
			std::string error = "In line: ";
			error += std::to_string(tables.tableOfSymb[activeLineOfSymbTable].textLine);
			error += "\nDiffernt number of ( and ): " + std::to_string(ops);
			throw std::runtime_error(error);
		}
	}
}


void Compiler::identIfExp()
{
	
	if (checkValueWithoutExc("if_op", "if") || checkValueWithoutExc("if_op", "elseif")) {
		std::cout << "\nStart If expression\n";

		checkTokenAndValue("brackets_op", "(");
		std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		identExpresion();
		if (checkTokenWithoutExc("rel_op"))
		{
			++activeLineOfSymbTable;
			identExpresion();

		}
		--activeLineOfSymbTable;
		checkTokenAndValue("brackets_op", ")");
		std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		++activeLineOfSymbTable;
	}
	if (checkValueWithoutExc("if_op", "else"))
	{
		std::cout << "\nStart else expression\n";
		++activeLineOfSymbTable;
	}
	if (checkValueWithoutExc("block_op", "{")) {
		std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		++activeLineOfSymbTable;
		identRecParingList();

		--activeLineOfSymbTable;
		checkTokenAndValue("block_op", "}");
		std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		++activeLineOfSymbTable;
	}
}

void Compiler::identKeyWExp()
{
	if (checkValueWithoutExc("keyword", "break")) {
		std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		++activeLineOfSymbTable;
	}
	else if (checkValueWithoutExc("keyword", "next")) {
		std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		++activeLineOfSymbTable;
	}
	else if (checkValueWithoutExc("keyword", "cat")) {
		
		bool temp = true;
		std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
		checkTokenAndValue("brackets_op", "(");

		while (temp)
		{

			
			checkToken("ident");
			std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			++activeLineOfSymbTable;
			temp = checkTokenAndValueWithoutExp("punct", ",");
			if (temp) {
				std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
					<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			}

		}

		--activeLineOfSymbTable;
		
		checkTokenAndValue("brackets_op", ")");
		std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
			<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";

		++activeLineOfSymbTable;
	}
	else if (checkValueWithoutExc("keyword", "for")) {
		identFor();
	}
}

void Compiler::identFor()
{
	
	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	checkTokenAndValue("brackets_op", "(");



	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	checkToken("ident");

	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";

	checkTokenAndValue("keyword", "in");
	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";

	identExpresion();

	--activeLineOfSymbTable;
	checkTokenAndValue("punct", ":");
	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";

	
	identExpresion();

	--activeLineOfSymbTable;

	checkTokenAndValue("brackets_op", ")");
	std::cout << "\nKeyword expression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
		<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
	
	++activeLineOfSymbTable;

		if (checkValueWithoutExc("block_op", "{")) {
			std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			++activeLineOfSymbTable;
			identRecParingList();

			--activeLineOfSymbTable;
			checkTokenAndValue("block_op", "}");
			std::cout << "\nIfExpression. Line: " << tables.tableOfSymb[activeLineOfSymbTable].textLine << "("
				<< tables.tableOfSymb[activeLineOfSymbTable].token << ", " << tables.tableOfSymb[activeLineOfSymbTable].value << ")\n";
			++activeLineOfSymbTable;
		}
	


}
