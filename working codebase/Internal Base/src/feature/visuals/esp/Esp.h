#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/entity/Classes.h"
#include "../../../../ext/imgui/imgui.h" 
class ESP
{
public:
	static void setup();
	static void renderEntity(Entity_t& ent, C_CSPlayerPawn* localPawn);

private:
    static ImDrawList* m_dl;
    static float m_sw;
    static float m_sh;
    static ImU32 m_boxCol1;
    static ImU32 m_boxCol2;
    static ImU32 m_skelCol;
    static ImU32 m_nameCol;
};
