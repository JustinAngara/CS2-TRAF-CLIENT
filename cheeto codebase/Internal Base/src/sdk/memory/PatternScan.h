#pragma once
#include <cstdint>

namespace Memory
{
    uintptr_t GetModuleBase(const char* module);
    uintptr_t PatternScan(const char* module, const char* signature);
}
