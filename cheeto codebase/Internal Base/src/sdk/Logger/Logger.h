#pragma once
#include <string>
#include <sstream>
#include <string>      
#include <vector>    
#include <mutex>       
#include <iomanip>       
#include <shared_mutex>
#include "../entity/EntityManager.h"
#include <utility> 




/** Usage
* LogSys::Run<LogSys::Channel::Entity>(Logger::Entity::PrintLocalPlayer);
* LogSys::Run<LogSys::Channel::Memory>(Logger::Memory::PrintMemScan, rawPointer, 128);
* LogSys::Run<LogSys::Channel::Entity>(Logger::Entity::PrintEntity, myEntity);
*/
namespace LogSys
{
	enum class Channel
	{
		Master,
		Entity,
		Memory
	};

	template <Channel C>
	constexpr bool IsEnabled()
	{
		if constexpr (C == Channel::Master) return true;  //////////master switch
		if constexpr (C == Channel::Entity) return true; 
		if constexpr (C == Channel::Memory) return true;  
		return false;
	}

	template <Channel C, typename Func, typename... Args>
	inline void Run(Func&& func, Args&&... args)
	{
		if constexpr (IsEnabled<Channel::Master>() && IsEnabled<C>())
		{
			std::forward<Func>(func)(std::forward<Args>(args)...);
		}
	}
}



struct LineString;
namespace Logger
{
	// instance variables
	inline std::string g_folderLoc{ "" };
	inline std::string g_fileName{ "" }; // date + " " + randomLetters
	inline std::string g_date{};


	void WriteToFile(LineString ls);

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
		void PrintEntity(Entity_t& entity);
		void PrintViewAnglesEntity(Entity_t& entity);
		void PrintXYZEntity(Entity_t& entity);
		void PrintLocalPlayer();
		void PrintAllEntities();
	
	}

	// TODO: Figure out what i want to do with memory statements. 
	namespace Memory
	{
		void PrintAllOffsetsCheck();
		void PrintOffsetsCheck(const uintptr_t offset);
		void PrintMemScan(uintptr_t rawPointer, int bufferSize); 
		void PrintAddressFromOffset();
	}

}




//////////////////////////
// i want to abstract this away,
// im scared i will accidentally
// use logger some fuck ass loc
//////////////////////////
inline std::string GetDate()
{
	time_t	rawTime = time(NULL);
	std::tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	std::ostringstream ss;
	ss << std::put_time(&timeInfo, " [%H:%M:%S]");
	Logger::g_date = ss.str();
	return Logger::g_date;
}

struct LineString
{
	std::string data;

	LineString(std::string s) :
		data(s){}



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
		std::string temp = GetDate();
		return temp + data + "-=-=-=-=END-=-=-=-=";
	}

};

