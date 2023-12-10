#include <iostream>
#include "Compiler.h"
int main()
{
	std::string nameOfFile{"LR.txt"};
	//std::cout << "Filename to compile: ";
	//std::cin >> nameOfFile;
	Compiler compiler(nameOfFile);
	std::cout << "Start analising: Lexical Analisator\n";
	if (compiler.LexicalAnalisator())
	{
		std::cout << "\nSuccess: Lexical Analisator\n";
		compiler.prinTables();
		std::cout << "\nContinue analising: Syntax Analisator\n";
		if (compiler.SyntaxAnalisator())
		{
			std::cout << "\nSuccess: Syntax Analisator\n";
		}

	}
	return 0;
}

