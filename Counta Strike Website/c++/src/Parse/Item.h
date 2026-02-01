#pragma once
#include <string>

class Item
{
public:
	Item(bool isEnumClass = false, std::string name = "", std::string type = "", std::string offset = "") :
	m_isEnumClass(isEnumClass), m_name(name), m_offset(offset)
{}

private:
	bool m_isEnumClass; // if it is u need to append uint32_t to compensate for safe data type

	std::string m_name;
	std::string m_type;
	std::string m_offset;

};
