#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/entity/Classes.h"
#include "../../../../ext/imgui/imgui.h" 
class ESP
{
public:
	static void setup();
	static void renderEntity(C_CSPlayerPawn* pawn, C_CSPlayerPawn* localPawn);

private:
    static ImDrawList* dl;
    static float sw;
    static float sh;
    static ImU32 boxCol1;
    static ImU32 boxCol2;
    static ImU32 skelCol;
    static ImU32 nameCol;
};
