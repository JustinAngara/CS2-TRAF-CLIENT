#include "GrabContent.h"
#include "Item.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


// edit the parse attributes now
void GrabContent::Run(Parse& parse)
{
	std::string fileLoc{ parse.getFileLoc() };

	std::string content{ };

	std::ifstream in(fileLoc, std::ios::binary);
	if (!in) throw std::runtime_error("Failed to open: " + fileLoc);

	std::ostringstream ss;
	ss << in.rdbuf();

	content = ss.str();

	std::cout << content << '\n';

	// now format the content into the vectors of parse items


};


