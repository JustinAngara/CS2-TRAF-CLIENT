#pragma once


#include "../Parse/Parse.h"
#include <string>

class ConvertToJson
{
public:
	ConvertToJson(Parse parse) :
		m_parse(parse)
	{ };

	void printJson();
	void populateContent();


	Parse& getParse() { return m_parse; }

	void setParse(Parse parse) { m_parse = parse; }

private:
	// want some parse object
	Parse&		m_parse;
	std::string m_content;
	
};
