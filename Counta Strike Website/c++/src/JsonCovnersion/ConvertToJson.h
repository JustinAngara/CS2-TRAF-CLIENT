#pragma once


#include "../../libs/json.hpp"
#include "../Parse/Parse.h"
#include <string>

using json = nlohmann::json;

class ConvertToJson
{
public:
	ConvertToJson(Parse& parse) :
		m_parse(parse)
	{ };

	void printJson();
	void populateContent();
	void populateList(std::vector<Item>& items, json& j);


	const Parse& getParse() const { return m_parse; }
	const std::string& getContent() const { return m_content; }
	void setParse(Parse parse) { m_parse = parse; }

private:
	// want some parse object
	Parse& m_parse;
	std::string  m_content;
	
};
