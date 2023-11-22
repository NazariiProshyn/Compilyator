#include "Tables.h"
#include <iostream>
#include <string>
void Tables::addTableOfId(std::string val, int line, std::string tok)
{
	idT.value = val;

	bool check = false;
	int tempId = 0;

	for (size_t i = 0; i < tableOfId.size(); ++i) {
		if (val == tableOfId[i].value)
		{
			check = true;
			tempId = i + 1;
		}
	}
	if (check) {
		symbT.value = val;
		symbT.textLine = line;
		symbT.token = tok;
		symbT.idxIdConst = std::to_string(tempId);
		tableOfSymb.push_back(symbT);
	}
	else
	{
		tableOfId.push_back(idT);

		symbT.value = val;
		symbT.textLine = line;
		symbT.token = tok;
		symbT.idxIdConst = std::to_string(tableOfId.size());

		tableOfSymb.push_back(symbT);
	}



}

void Tables::addTableOfConst(std::string val, int line, std::string tok)
{
	constT.value = val;
	constT.token = tok;

	tableOfConst.push_back(constT);

	symbT.value = val;
	symbT.textLine = line;
	symbT.token = tok;
	symbT.idxIdConst = std::to_string(tableOfConst.size());

	tableOfSymb.push_back(symbT);
}

void Tables::addTableOfSymb(std::string val, int line, std::string tok)
{
	symbT.value = val;
	symbT.textLine = line;
	symbT.token = tok;
	symbT.idxIdConst = "NoId";

	tableOfSymb.push_back(symbT);
}

void Tables::printTableOfId()
{
	std::cout << "\n\tTableOfId\nId : idxId\n";
	for (size_t i = 0; i < tableOfId.size(); ++i) {
		std::cout << tableOfId[i].value << " : " << i + 1 << std::endl;
	}
}

void Tables::printTableOfConst()
{
	std::cout << "\n\tTableOfnConst\nConst : (token,idxConst}\n";
	for (size_t i = 0; i < tableOfConst.size(); ++i) {
		std::cout << tableOfConst[i].value << " : {" << tableOfConst[i].token << ", " << i + 1 << "}" << std::endl;
	}
}

void Tables::printTableOfSymb()
{
	std::cout << "\n\tTableOfSymb\nn_rec : (num_line, lexeme, token, idxIdConst)\n";
	for (size_t i = 0; i < tableOfSymb.size(); ++i) {

		std::cout << i + 1 << " : (" << tableOfSymb[i].textLine << ", "
			<< tableOfSymb[i].value << ", " << tableOfSymb[i].token << ", "
			<< tableOfSymb[i].idxIdConst << ")" << std::endl;
	}
}
