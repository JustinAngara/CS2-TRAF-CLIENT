#pragma once
#include <cstddef>
#include <string>
/*
	going to switch up how we do the offsets
	utilzie the loader to check for updates		
*/

// migration to new way to handle offsets
// TODO: FINISH THIS
// THIS IS NOT FOR PROD YET, IDGAF ABOUT MACROS JUST DONT USE YET

class PseudoMigration
{
public:
	
	static uintptr_t GrabOffsets(std::string var, std::string dllPath); // foo( dwEntityList, {client.dll, offsets.dll} )
	static void setFolderLoc(std::string fl) { s_folderLoc = fl; };

private:
	static std::string s_folderLoc;
};

namespace Offsets
{

	// -> offsets.h on a2x dumper
	constexpr std::ptrdiff_t dwCSGOInput						   = 0x231E330;
	constexpr std::ptrdiff_t dwEntityList						   = 0x24B3268;
	constexpr std::ptrdiff_t dwGameEntitySystem					   = 0x24B3268;
	constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x20A0;
	constexpr std::ptrdiff_t dwGameRules						   = 0x2311ED0;
	constexpr std::ptrdiff_t dwGlobalVars						   = 0x2062540;
	constexpr std::ptrdiff_t dwGlowManager						   = 0x230ECD8;
	constexpr std::ptrdiff_t dwLocalPlayerController			   = 0x22F8028;
	constexpr std::ptrdiff_t dwLocalPlayerPawn					   = 0x206D9E0;
	constexpr std::ptrdiff_t dwPlantedC4						   = 0x231BAB0;
	constexpr std::ptrdiff_t dwPrediction						   = 0x206D8F0;
	constexpr std::ptrdiff_t dwSensitivity						   = 0x230F7E8;
	constexpr std::ptrdiff_t dwSensitivity_sensitivity			   = 0x58;
	constexpr std::ptrdiff_t dwViewAngles						   = 0x231E9B8;
	constexpr std::ptrdiff_t dwViewMatrix						   = 0x2313F10;
	constexpr std::ptrdiff_t dwViewRender						   = 0x2314328;
	constexpr std::ptrdiff_t dwWeaponC4							   = 0x229D2B0;

	////////////// -> client.dll.hpp on a2x dumper and on
	constexpr uintptr_t m_iHealth		 = 0x354; 
	constexpr uintptr_t m_iTeamNum		 = 0x3F3; 
	constexpr uintptr_t m_vOldOrigin	 = 0x1588; 
	constexpr uintptr_t m_pGameSceneNode = 0x338; 

	// -> CbaseModelEntity
	constexpr uintptr_t m_vecViewOffset = 0xD58;

	// -> C_CSPlayerPawn
	constexpr uintptr_t m_iShotsFired	= 0x270C;
	constexpr uintptr_t m_aimPunchAngle = 0x16CC;
	constexpr uintptr_t m_fFlags		= 0x63; 

	constexpr uintptr_t m_szTeamname   = 0x63C; 
	constexpr uintptr_t m_hPlayerPawn  = 0x90C;
	constexpr uintptr_t m_bPawnIsAlive = 0x914;

	constexpr uintptr_t m_modelState = 0x160;

	constexpr std::ptrdiff_t m_pObserverServices  = 0x13F0;
	constexpr std::ptrdiff_t m_hObserverTarget	  = 0x4C;
	constexpr std::ptrdiff_t m_iObserverMode	  = 0x48;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x26E0; 
	constexpr std::ptrdiff_t m_aimPunchAngleVel	  = 0x16D8;

	//ashfiasiuofhusa
	constexpr std::ptrdiff_t m_aimPunchCache = 0x16F0;

	constexpr std::ptrdiff_t jump = 0x205BAF0;



}


