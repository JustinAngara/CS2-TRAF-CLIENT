#pragma once
#include <string>
#include <string_view>
#include <ctime>
/* 
TODO: Finish the logger
	  Write to file
*/


namespace Logger
{
	// instance variables
	inline std::string g_folderLoc{ "" };
	inline std::string g_fileName{ "" }; // date + " " + randomLetters
	inline time_t	   g_date{};

	
	namespace Setup
	{
		void Init(std::string file);

		// getters
		std::string GetFolderLoc();
		time_t		GetDate();
		std::string GetFileName();

		// setters
		void SetFolderLoc(std::string loc);
		void SetDate();     // just do it manually here, no setters
		void SetFileName(); // same here, just call it in init

	}



	namespace Entity
	{
		void PrintLocalPlayer();
		void PrintEntity();
		void PrintViewAngles();
		void PrintXYZ();
		void PrintAllEntities();
	}

	namespace Memory
	{
		void PrintOffsets();
		void PrintMemScan();
		void PrintAddressFromOffset();
	}


}
