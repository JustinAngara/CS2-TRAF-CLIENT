#pragma once
#include "Item.h"
#include <string>
#include <vector>

class Block
{
public:
	Block(bool isEnumClass = false, std::string tuName) :
		  m_isEnumClass(isEnumClass), m_tuName(tuName)
	{ }

private:
	bool m_isEnumClass;
	std::string m_tuName;
	std::vector<Item> m_content;
};
