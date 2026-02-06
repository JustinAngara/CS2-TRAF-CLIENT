#pragma once
#include "Item.h"
#include <string>
#include <vector>

class Block
{
public:
	Block(bool isEnumClass = false, std::string tuName = "", std::string blockContent = "") :
		m_isEnumClass(isEnumClass), m_tuName(tuName), m_blockContent(blockContent)
	{ }

	bool getIsEnumClass() const { return m_isEnumClass; }
	std::string getName() const { return m_tuName; }
	std::string getBlockContent() const { return m_blockContent; }
	std::vector<Item> getContent() const { return m_content; }

	void addItem(const Item& item) { m_content.push_back(item); }

private:
	bool m_isEnumClass;
	std::string m_tuName;
	std::string m_blockContent;
	std::vector<Item> m_content;
};
