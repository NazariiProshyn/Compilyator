#include <iostream>
#include "Compiler.h"
int main()
{
	std::string nameOfFile{"LR.txt"};
	//std::cout << "Filename to compile: ";
	//std::cin >> nameOfFile;
	Compiler compiler(nameOfFile);
	if (compiler.LexicalAnalisator())
	{
		compiler.prinTables();
	}
	return 0;
}

