#pragma once
#include "../Life/InjectorContext.h"
#include <windows.h>
#include <string>

namespace Process
{

	DWORD GetPIDByName(InjectorContext& ctx, const std::wstring& name);

}
