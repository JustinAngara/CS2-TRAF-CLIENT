#include "Logger.h"
#include <string>
#include <iostream>

//////////////////////////
// logger setup
//////////////////////////
void Logger::Setup::Init()
{
	std::string wasd{ "" };
	SetFolderLoc(wasd);
	
}

//////////////////////////
// logger iv
//////////////////////////

std::string Logger::GetFolderLoc()
{
	std::cout << "okay i am in here of logger\n";
	return "";
}

void Logger::SetFolderLoc(std::string loc)
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



