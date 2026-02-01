#pragma once
#include <string>

class Item
{
private:
	std::string m_isEnumClass; // if it is u need to append uint32_t to compensate for safe data type

	std::string m_name;
	std::string m_type;
	std::string m_offset;

};
