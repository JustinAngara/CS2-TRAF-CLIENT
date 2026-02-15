#include "Setup.h"
#include "../feature/visuals/esp/Esp.h"
#include "../sdk/Logger/Logger.h"
#include <iostream>
void Setup::Run()
{
	Logger::Setup::Init(); // this is setup init for logger

}
