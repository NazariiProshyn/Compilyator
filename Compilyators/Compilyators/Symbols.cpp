#include "Symbols.h"

namespace NMSymbols {
	std::vector<std::string> Letters{
		"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
		"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D",
		"E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S",
		"T", "U", "V", "W", "X", "Y", "Z"
	};

	std::vector<std::string> Digit{
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
	};

	std::vector<std::string> SpecSymbols{
		 ",", "(", ")", "{", "}", ":", "%", "=", "+", "-", "*", "/", "<", ">",
		"^", "_"
	};

	std::string dot{ "." };
	std::string comment{ "#" };

	std::vector<std::string> WhiteSpace{
		" ", "/t"
	};

	std::vector<std::string> EndOfLine{
		"/n", "/r"
	};

	std::vector<std::pair<std::string, std::string>> lexemToken{
		{"keyword","readline"},
		{"keyword","cat"},
		{"keyword","for"},
		{"keyword","next"},
		{"keyword","break"},
		{"boolval","TRUE"},
		{"boolval","FALSE"},
		{"assign_op","<-"},
		{"add_op","+"},
		{"add_op","-"},
		{"mult_op","*"},
		{"mult_op","/"},
		{"mult_op","%"},
		{"exp_op","^"},
		{"rel_op","<"},
		{"rel_op",">"},
		{"rel_op","<="},
		{"rel_op",">="},
		{"rel_op","=="},
		{"rel_op","!="},
		{"brackets_op","("},
		{"brackets_op",")"},
		{"punct","."},
		{"punct",","},
		{"punct",":"},
		{"ws","/t"},
		{"eol","/n"},
		{"block_op","{"},
		{"block_op","}"},
		{"if_op","if"},
		{"if_op","elseif"},
		{"if_op","else"}
	};

	std::vector<std::pair<int, std::string>> numTokens{
		{2, "ident"},
		{9, "intnum"},
		{6, "realnum"}
	};
}

std::string Symbols::typeOfSymbol(std::string info)
{
	for (size_t i = 0; i < NMSymbols::Letters.size(); ++i)
	{
		if (info == NMSymbols::Letters[i])
		{
			return "Letter";
		}
	}

	for (size_t i = 0; i < NMSymbols::Digit.size(); ++i)
	{
		if (info == NMSymbols::Digit[i])
		{
			return "Digit";
		}
	}

	for (size_t i = 0; i < NMSymbols::SpecSymbols.size(); ++i)
	{
		if (info == NMSymbols::SpecSymbols[i])
		{
			return "SpecSymbols";
		}
	}

	if (info == NMSymbols::dot)
	{
		return "Dot";
	}

	if (info == NMSymbols::comment)
	{
		return "Comment";
	}

	for (size_t i = 0; i < NMSymbols::WhiteSpace.size(); ++i)
	{
		if (info == NMSymbols::WhiteSpace[i])
		{
			return "WhiteSpace";
		}
	}

	for (size_t i = 0; i < NMSymbols::EndOfLine.size(); ++i)
	{
		if (info == NMSymbols::EndOfLine[i])
		{
			return "EndOfLine";
		}
	}

	return std::string{"Error"};
}

std::string Symbols::typeOfIdentificator(std::string info)
{
	for (size_t i = 0; i < NMSymbols::lexemToken.size(); ++i)
	{
		if (info == NMSymbols::lexemToken[i].second)
		{
			return NMSymbols::lexemToken[i].first;
		}
	}
	return std::string{"None"};
}
