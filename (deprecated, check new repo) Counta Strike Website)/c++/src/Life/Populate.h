#pragma once
#include "../Parse/Parse.h"
#include "../Parse/GrabContent.h"
#include "../JsonCovnersion/ConvertToJson.h"
#include "../JsonCovnersion/File.h"

namespace Populate
{

	std::string BaseNameFromPath(const std::string& p)
	{
		std::filesystem::path path(p);
		return path.stem().string(); // "client_dll" from ".../client_dll.hpp"
	}

	// R"(src\Data\server_dll.hpp)"
	void Run(std::string fileLoc)
	{
		// get the name of the offset/data files
		auto base = BaseNameFromPath(fileLoc);

		Parse p1{ base, fileLoc };
		GrabContent::Run(p1);
		ConvertToJson ctj{ p1 };
		ctj.populateContent();

		// file stuff
		std::string fileName = base + ".json";
		std::filesystem::path path = std::filesystem::path("src") / "output" / fileName;
		File::PutInTo(path.string(), ctj.getContent());


	}
}
