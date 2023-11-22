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

	std::cout << "Text in File:\n";
	while (std::getline(infoFile, dataString)) {
		++numOfLine;
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
