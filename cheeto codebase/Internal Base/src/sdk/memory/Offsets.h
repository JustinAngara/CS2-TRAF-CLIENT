#pragma once
#include <cstddef>

/*
	going to switch up how we do the offsets
	utilzie the loader to check for updates		
*/

namespace Offsets
{
	// -> offsets.h on a2x dumper
	constexpr uintptr_t		 dwEntityList			 = 0x24AE268;  
	constexpr uintptr_t		 dwLocalPlayerPawn		 = 0x2069B50; 
	constexpr uintptr_t		 dwLocalPlayerController = 0x22F4188;  
	constexpr uintptr_t		 dwViewMatrix			 = 0x230FF20;  
	constexpr uintptr_t		 dwViewAngles			 = 0x231A648;  
	constexpr uintptr_t		 dwGlobalVars			 = 0x205E5C0;    
	constexpr std::ptrdiff_t dwSensitivity			 = 0x230B7F8;  

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
	constexpr uintptr_t m_fFlags		= 0x400; // 

	constexpr uintptr_t m_szTeamname   = 0x63C; 
	constexpr uintptr_t m_hPlayerPawn  = 0x90C;
	constexpr uintptr_t m_bPawnIsAlive = 0x914;

	constexpr uintptr_t m_modelState = 0x160;

	constexpr std::ptrdiff_t m_pObserverServices  = 0x13F0;
	constexpr std::ptrdiff_t m_hObserverTarget	  = 0x4C;
	constexpr std::ptrdiff_t m_iObserverMode	  = 0x48;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x1180; 
	constexpr std::ptrdiff_t m_aimPunchAngleVel	  = 0x16D8;
	constexpr std::ptrdiff_t m_aimPunchCache	  = 0x16F0;

	constexpr std::ptrdiff_t jump = 0x205BAF0;

}
