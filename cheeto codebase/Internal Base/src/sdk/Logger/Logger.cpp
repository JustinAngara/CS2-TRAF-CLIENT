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

// folder
void Logger::Setup::SetFolderLoc(std::string loc) { g_folderLoc = loc; }
std::string Logger::Setup::GetFolderLoc()         { return g_folderLoc; }
// name
std::string Logger::Setup::GetFileName(){ return g_fileName; }
void Logger::Setup::SetFileName() { g_fileName = "[" + GetDate() + "] Log Test"; }


void Logger::Setup::Init(std::string folder)
{
	// do a bunch of this shit here. i don't want to test right now so someone fix this 	
	SetFolderLoc(folder);
	SetFileName();
	LineString ls{ "irjgioerjiogje" };
	WriteToFile(ls);
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

//////////////////////////
// logger entity
//////////////////////////

void Logger::Entity::PrintEntity(Entity_t& entity)
{
	LineString ls{ "" }; 
	// e.pawn, e.controller figure out what iw ant to do
	WriteToFile(ls);
}

void Logger::Entity::PrintViewAnglesEntity(Entity_t& entity)
{
	LineString ls{ "Entity's ViewAngles" }; // constructor

	//Vector	   localPos = entity.pawn->m_vOldOrigin() + entity.pawn->m_vecViewOffset();
	//ls += std::format("(X, Y, Z) -> ({}, {}, {})", localPos.x, localPos.y, localPos.z);
	WriteToFile(ls);
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
	LineString ls{ "" };

	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	std::string	temp{};
	
	WriteToFile(ls);
}

void Logger::Entity::PrintAllEntities()
{
	LineString ls{ "Start of Print ALL Entities ...{" };

	const auto& list = EntityManager::Get().GetEntities();
	for (auto e : list)
	{
		PrintEntity(e);
	}
	ls += "\n...}";
	WriteToFile(ls);
}

//////////////////////////
// logger memory
//////////////////////////

/// @brief For fuck sake only run this on init/setup
///		   You need to run the fucking damn shit and see if it matches
void Logger::Memory::PrintAllOffsetsCheck()
{
	LineString ls{ "" };
	
	WriteToFile(ls);
}
void Logger::Memory::PrintOffsetsCheck(const uintptr_t offset)
{
	LineString ls{ "" };
	
	WriteToFile(ls);
}

void Logger::Memory::PrintMemScan(uintptr_t rawPointer, int bufferSize)
{
	LineString ls{ "" };

	WriteToFile(ls);
}

void Logger::Memory::PrintAddressFromOffset()
{
	LineString ls{ "" };

	WriteToFile(ls);
}
