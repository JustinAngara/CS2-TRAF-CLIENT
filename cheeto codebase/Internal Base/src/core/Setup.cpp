#include "Setup.h"
#include "../feature/visuals/esp/Esp.h"
#include "../sdk/Logger/Logger.h"

#include <iostream> // TEMP TEMP TEMP
// perform logger handler
void Setup::Run()
{

	std::cout << "hit iostream\n"; // TEMP TEMP TEMP

	//////// logger
	//std::string logFolder{ "./logs" };
	//Logger::Setup::Init( logFolder ); // this is setup init for logger

	// i dont want annoying spam
	//Logger::Memory::PrintAllOffsetsCheck(); // okay this should only be ran once pleaes figureo ut a way to see how to ensure this


	//////// esp
	ESP::setup(); 
	

}
