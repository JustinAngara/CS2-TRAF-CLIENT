#pragma once
#include <string>
class Parse
{
public:
	Parse(std::string fileLoc) :
		m_fileLoc(fileLoc){}
	std::string getFileLoc() { return m_fileLoc; }
	std::string getContent() { return m_content; }
	
private:
	std::string m_fileLoc;
	// ordered list
	
};
