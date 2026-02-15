#pragma once
#include <string>
#include <string_view>
/* 
TODO: Finish the logger
	  Write to file
*/


namespace Logger
{
	// instance variables
	inline std::string folderLoc{ "" };
	std::string GetFolderLoc();
	void SetFolderLoc(std::string loc);

	
	namespace Setup
	{
		void Init();
	}



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
