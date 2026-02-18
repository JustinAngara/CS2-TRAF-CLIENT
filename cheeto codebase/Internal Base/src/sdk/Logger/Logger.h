#pragma once
#include <string>
#include <sstream>
#include <string>      
#include <vector>    
#include <mutex>       
#include <iomanip>       
#include <shared_mutex>

struct LineString;

namespace Logger
{
	// instance variables
	inline std::string g_folderLoc{ "" };
	inline std::string g_fileName{ "" }; // date + " " + randomLetters
	inline std::string g_date{};


	// TODO: Change to string, i dont fuck with this time_t shit
	std::string GetDate(); // this gets called a lot thats why it isn't setup 
	void		WriteToFile(LineString ls);

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
		std::string temp = Logger::GetDate();
		return temp + data + "-=-=-=-=END-=-=-=-=";
	}

};


// toggle this line to enable/disable logger
#define ENABLE_LOGGING 

#ifdef ENABLE_LOGGING
    #define LOG_INIT(file)        Logger::Setup::Init(file)
    #define LOG(ls)               Logger::WriteToFile(ls)

    // entity
    #define LOG_PLAYER()          Logger::Entity::PrintLocalPlayer()
    #define LOG_ENTITY()          Logger::Entity::PrintEntity()
    #define LOG_ANGLES()          Logger::Entity::PrintViewAngles()
    #define LOG_XYZ()             Logger::Entity::PrintXYZ()
    #define LOG_ALL_ENTITIES()    Logger::Entity::PrintAllEntities()

    // memory
    #define LOG_OFFSETS()         Logger::Memory::PrintOffsets()
    #define LOG_MEMSCAN()         Logger::Memory::PrintMemScan()
    #define LOG_ADDR_OFFSET()     Logger::Memory::PrintAddressFromOffset()
#else
    // optimize away
    #define LOG_INIT(file)        do { (void)(file); } while(0)
    #define LOG(ls)               do { (void)(ls); } while(0)

    #define LOG_PLAYER()          do {} while(0)
    #define LOG_ENTITY()          do {} while(0)
    #define LOG_ANGLES()          do {} while(0)
    #define LOG_XYZ()             do {} while(0)
    #define LOG_ALL_ENTITIES()    do {} while(0)

    #define LOG_OFFSETS()         do {} while(0)
    #define LOG_MEMSCAN()         do {} while(0)
    #define LOG_ADDR_OFFSET()     do {} while(0)
#endif
