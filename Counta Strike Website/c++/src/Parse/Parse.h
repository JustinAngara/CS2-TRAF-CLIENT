#pragma once
#include <string>
#include <vector>
#include "Item.h"
class Parse
{
public:
	Parse(std::string fileLoc) :
		m_fileLoc(fileLoc){}
	std::string getFileLoc() { return m_fileLoc; }
	std::vector<Item> getContent() { return m_items; }
	
private:
	std::string m_fileLoc;
	// ordered list
	std::vector<Item> m_items;
	
};
