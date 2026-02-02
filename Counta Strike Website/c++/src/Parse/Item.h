#pragma once
#include <string>

class Item
{
public:
	Item(std::string name = "", std::string type = "", std::string offset = "") :
	m_name(name), m_type(type), m_offset(offset)
{}

private:
	
	std::string m_name;
	std::string m_type;
	std::string m_offset;

};
