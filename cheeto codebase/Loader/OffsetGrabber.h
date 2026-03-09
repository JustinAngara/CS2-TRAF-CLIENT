#pragma once
#include <string_view>

/*
update: switch to loader instead.

this will be the setup to grab everything at init
i am very sick of having to manually find offsets and then manually have to type it out.
check out: https://justinangara.github.io/CS2-Offsets-Website/
ferda
okay this is a test sorry for spamming 

*/
namespace OffsetGrabber
{
	// directed towards the file loc of the a2x dumper
	constexpr std::string_view s_client_dll { "" };
	constexpr std::string_view s_offsets_dll{ "" };


	void Setup();
	void Run();

	int	 IterateOffsets();
	bool IsValid();



}
