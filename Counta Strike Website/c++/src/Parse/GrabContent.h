#pragma once
#include "Parse.h"
#include "Item.h"

namespace GrabContent
{
	void Run(Parse& parse);
	Item GetItem(std::string content, int index); // pass in reference to content and maybe end?
}
