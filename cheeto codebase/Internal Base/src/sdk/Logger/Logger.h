#pragma once
#include <string>
#include <sstream>
#include <string>      
#include <vector>    
#include <mutex>       
#include <iomanip>       
#include <shared_mutex> 


namespace Logger
{
	// instance variables
	inline std::string g_folderLoc{ "" };
	inline std::string g_fileName{ "" }; // date + " " + randomLetters
	inline time_t	   g_date{};


	// TODO: Change to string, i dont fuck with this time_t shit
	time_t GetDate(); // this gets called a lot thats why it isn't setup 

	namespace Setup
	{
		void Init(std::string file);

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
		time_t rawTime = Logger::GetDate();

		std::tm timeInfo;
		localtime_s(&timeInfo, &rawTime);

		std::ostringstream ss;
		ss << '\n';
		ss << std::put_time(&timeInfo, " [%H:%M:%S]");
		return ss.str() + data + "-=-=-=-=END-=-=-=-=";
	}

};
