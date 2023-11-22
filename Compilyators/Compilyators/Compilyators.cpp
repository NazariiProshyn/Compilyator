#include <iostream>
#include "Compiler.h"
int main()
{
	std::string nameOfFile{"LR1.txt"};
	//std::cout << "Filename to compile: ";
	//std::cin >> nameOfFile;
	Compiler compiler(nameOfFile);
	if (compiler.LexicalAnalisator())
	{
		compiler.prinTables();
	}
	return 0;
}

