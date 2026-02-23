#include "Logger.h"
#include <string>
#include "../../sdk/entity/EntityManager.h"
#include <ctime>
#include <format>
#include <fstream>
#include <iostream>

//////////////////////////
// logger setup & mutators
//////////////////////////
void Logger::Setup::Init(std::string folder)
{
	
	SetFolderLoc(folder);
	SetFileName();


}

void Logger::WriteToFile(LineString ls)
{
	// 'static' ensures the file stays open across calls for better performance
	static std::ofstream logFile(g_folderLoc + "\\" + g_fileName + ".txt", std::ios::app);
	std::string data = ls; 

	if (!logFile.is_open())
	{
		std::cerr << "can't open file\n";
		return;
	} 
	logFile << data << '\n'; // safe operation to ensure to write before crash
	

}


std::string Logger::GetDate()
{
	time_t rawTime = time(NULL);
	std::tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	std::ostringstream ss;
	ss << std::put_time(&timeInfo, " [%H:%M:%S]");
	g_date = ss.str();

	return g_date;
}

// folder
void Logger::Setup::SetFolderLoc(std::string loc) { g_folderLoc = loc; }
std::string Logger::Setup::GetFolderLoc()         { return g_folderLoc; }

// name
std::string Logger::Setup::GetFileName(){ return g_fileName; }

void Logger::Setup::SetFileName()
{
	g_fileName = "[" + GetDate() + "] Log";
}


//////////////////////////
// logger entity
//////////////////////////

void Logger::Entity::PrintEntity(Entity_t& entity)
{
	// e.pawn, e.controller figure out what iw ant to do

}

void Logger::Entity::PrintViewAnglesEntity(Entity_t& entity)
{
}

void Logger::Entity::PrintXYZEntity(Entity_t& entity)
{
	LineString ls{ "Entity's PrintXYZ" }; // constructor

	Vector localPos = entity.pawn->m_vOldOrigin() + entity.pawn->m_vecViewOffset();

	ls += std::format("(X, Y, Z) -> ({}, {}, {})", localPos.x, localPos.y, localPos.z);

	WriteToFile(ls);

}

void Logger::Entity::PrintLocalPlayer()
{

	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	std::string	temp{};
	
}

void Logger::Entity::PrintAllEntities()
{
	const auto& list = EntityManager::Get().GetEntities();
	for (auto e : list)
	{
		PrintEntity(e);
		
	}
}

//////////////////////////
// logger memory
//////////////////////////
void Logger::Memory::PrintOffsetsCheck()
{
}

void Logger::Memory::PrintMemScan(uintptr_t rawPointer, int bufferSize)
{
}

void Logger::Memory::PrintAddressFromOffset()
{
}



