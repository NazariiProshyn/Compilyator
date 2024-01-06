#pragma once
#include <vector>
#include <string>


	class TableOfId
	{
	public:
		TableOfId() = default;
		~TableOfId() = default;

		std::string value{};
	};

	class TableOfConst
	{
	public:
		TableOfConst() = default;
		~TableOfConst() = default;

		std::string token{};
		std::string value{};
	};

	class TableOfSymb
	{
	public:
		TableOfSymb() = default;
		~TableOfSymb() = default;

		int textLine{};
		std::string value{};
		std::string token{};
		std::string idxIdConst{};
	};


class Tables
{
public:
	Tables() = default;
	~Tables() = default;
	void addTableOfId(std::string val, int line, std::string tok);
	void addTableOfConst(std::string val, int line, std::string tok);
	void addTableOfSymb(std::string val, int line, std::string tok);

	void printTableOfId();
	void printTableOfConst();
	void printTableOfSymb();

	std::vector<TableOfSymb> tableOfSymb;
private:
	std::vector<TableOfId> tableOfId;
	std::vector<TableOfConst> tableOfConst;
	

	TableOfId idT;
	TableOfConst constT;
	TableOfSymb symbT;
};

