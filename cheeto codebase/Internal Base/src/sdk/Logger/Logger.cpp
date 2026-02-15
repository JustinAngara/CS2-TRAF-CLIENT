#include "Logger.h"
#include <string_view>
#include <string>
#include <iostream>

//////////////////////////
// logger setup
//////////////////////////
void Logger::Setup::Init()
{  
	
}

//////////////////////////
// logger iv
//////////////////////////

std::string_view Logger::GetFolderLoc()
{
	std::cout << "okay i am in here of logger\n";
}

void Logger::SetFolderLoc(std::string_view loc)
{
}

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



