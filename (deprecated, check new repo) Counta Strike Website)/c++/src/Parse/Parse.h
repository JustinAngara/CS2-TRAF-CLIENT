#pragma once
#include "Block.h"
#include <string>
#include <vector>
class Parse
{
public:
	Parse(std::string name = "", std::string fileLoc = "") :
		m_name(name), m_fileLoc(fileLoc){}
	std::string getFileLoc() { return m_fileLoc; }
	std::vector<Block> getContent() { return m_blocks; }

	void add(Block b)
	{
		m_blocks.push_back(b);
	};

private:
	std::string m_fileLoc;
	std::string m_name;
	// ordered list
	std::vector<Block> m_blocks;
	
};
