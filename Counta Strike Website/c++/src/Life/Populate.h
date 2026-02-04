#pragma once
#include "../Parse/Parse.h"
#include "../Parse/GrabContent.h"
#include "../JsonCovnersion/ConvertToJson.h"

namespace Populate
{

	void Run(std::string fileLoc)
	{
		Parse p1{ fileLoc };
		GrabContent::Run(p1);
		ConvertToJson ctj{ p1 };
		ctj.populateContent();
	}
}
