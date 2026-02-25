#include "Setup.h"
#include "../feature/visuals/esp/Esp.h"
#include "../sdk/Logger/Logger.h"
#include <iostream>
void Setup::Run()
{
	std::string file{ "" };
	Logger::Setup::Init(file); // this is setup init for logger

	// i dont want annoying spam
	Logger::Memory::PrintAllOffsetsCheck(); // okay this should only be ran once pleaes figureo ut a way to see how to ensure this

}
