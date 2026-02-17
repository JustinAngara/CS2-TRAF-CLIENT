#pragma once
#include <string>
#include <sstream>
#include <string>      
#include <vector>    
#include <mutex>       
#include <shared_mutex> 

struct LineString
{
	std::string data;

	template <typename T>
	LineString& operator+=(const T& value)
	{
		std::ostringstream ss;

		ss << value << "\n"; // adds teh append everytime you do builder += "data...."

		data += ss.str();
		return *this;
	}
	operator std::string() const
	{
		return data;
	}

};

namespace Logger
{
	// instance variables
	inline std::string g_folderLoc{ "" };
	inline std::string g_fileName{ "" }; // date + " " + randomLetters
	inline time_t	   g_date{};





	namespace Setup
	{
		void Init(std::string file);

		time_t GetDate();

		std::string GetFolderLoc();
		void		SetFolderLoc(std::string loc);


		std::string GetFileName();
		void		SetFileName(); // same here, just call it in init

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
