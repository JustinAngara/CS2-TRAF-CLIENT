#pragma once
#include "../../../sdk/entity/EntityManager.h"
#include "../../../sdk/entity/Classes.h"
class ESP
{
public:
	static void render();
	static void setup();
	static void renderEntity(C_CSPlayerPawn* pawn, C_CSPlayerPawn* localPawn);

private:
};
