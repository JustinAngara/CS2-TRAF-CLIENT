#include "Logger.h"
#include <string>
#include "../../sdk/entity/EntityManager.h"
#include <ctime>
//////////////////////////
// logger setup & mutators
//////////////////////////
void Logger::Setup::Init(std::string folder)
{
	
	SetFolderLoc(folder);

	// no params cuz i said so
	SetDate();     // gets the current date automatically
	SetFileName(); // sets the name automatically
}

// folder
void Logger::Setup::SetFolderLoc(std::string loc) { g_folderLoc = loc; }
std::string Logger::Setup::GetFolderLoc()         { return g_folderLoc; }

// date
void Logger::Setup::SetDate() {}
time_t Logger::Setup::GetDate(){}

// name
std::string Logger::Setup::GetFileName() {}
void Logger::Setup::SetFileName() {}


//////////////////////////
// logger entity
//////////////////////////
void Logger::Entity::PrintLocalPlayer()
{
}

void Logger::Entity::PrintEntity()
{
}

void Logger::Entity::PrintViewAngles()
{
}

void Logger::Entity::PrintXYZ()
{
}


void Logger::Entity::PrintAllEntities()
{
	const auto& list = EntityManager::Get().GetEntities();
	for (const auto& e : list)
	{
				
	}
}

//////////////////////////
// logger memory
//////////////////////////
void Logger::Memory::PrintOffsets()
{
}

void Logger::Memory::PrintMemScan()
{
}

void Logger::Memory::PrintAddressFromOffset()
{
}



