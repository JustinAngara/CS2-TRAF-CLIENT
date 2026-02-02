#pragma once
#include "Block.h"
#include <string>
#include <vector>
class Parse
{
public:
	Parse(std::string fileLoc) :
		m_fileLoc(fileLoc){}
	std::string getFileLoc() { return m_fileLoc; }
	std::vector<Block> getContent() { return m_items; }
	
private:
	std::string m_fileLoc;
	// ordered list
	std::vector<Block> m_items;
	
};
