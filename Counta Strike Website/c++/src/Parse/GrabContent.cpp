#include "GrabContent.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
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


};
