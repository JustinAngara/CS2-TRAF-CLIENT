#pragma once
#include <string>
#include "../../../ext/spdlog/logger.h"
#include "../../../ext/spdlog/spdlog.h"
/* 
TODO: Finish the logger
	  Write to file
*/


namespace Logger
{
	// instance variables
	inline std::string folderLoc{ "" };

	std::string_view GetFolderLoc();
	void SetFolderLoc(std::string_view loc);



	namespace Entity
	{
		void PrintLocalPlayer();
		void PrintEntity();
		void PrintViewAngles();
		void PrintXYZ();
	}

	namespace Memory
	{
		void PrintOffsets();
		void PrintMemScan();
		void PrintAddressFromOffset();
	}


}
