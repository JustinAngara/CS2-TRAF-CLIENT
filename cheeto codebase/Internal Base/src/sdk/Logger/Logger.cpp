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

	SetFileName(); // sets the name automatically
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
std::string Logger::Setup::GetFileName(){ return ""; }

void Logger::Setup::SetFileName()
{
	g_fileName = GetDate() + " - Log";
}


//////////////////////////
// logger entity
//////////////////////////
void Logger::Entity::PrintLocalPlayer()
{

	C_CSPlayerPawn* local = EntityManager::Get().GetLocalPawn();
	std::string	temp{};
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



