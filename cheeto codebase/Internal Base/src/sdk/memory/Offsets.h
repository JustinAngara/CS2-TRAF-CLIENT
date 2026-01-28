#pragma once
#include <cstddef>

/*
* NOTE: Hopefully I RE correctly ;-;
	offsets are going to be rolled back to the 1/19/26 version
	i will figure out the changes soon
	use old cs2 version dating 1/19/26
	constexpr uintptr_t		 dwEntityList			 = 0x21C3AF8;
	constexpr uintptr_t		 dwLocalPlayerPawn		 = 0x2061850;
	constexpr uintptr_t		 dwLocalPlayerController = 0x22EBA28;
	constexpr uintptr_t		 dwViewMatrix			 = 0x2307850;
	constexpr uintptr_t		 dwViewAngles			 = 0x2311A68;
	constexpr uintptr_t		 dwGlobalVars			 = 0x20562A8;
	constexpr std::ptrdiff_t dwSensitivity			 = 0x2303128;

	constexpr uintptr_t		 dwEntityList			 = 0x1D13CE8;
	constexpr uintptr_t		 dwLocalPlayerPawn		 = 0x1BEEF28;
	constexpr uintptr_t		 dwLocalPlayerController = 0x1E1DC18;
	constexpr uintptr_t		 dwViewMatrix			 = 0x1E323D0;
	constexpr uintptr_t		 dwViewAngles			 = 0x1E3C800;
	constexpr uintptr_t		 dwGlobalVars			 = 0x1BE41C0;
	constexpr std::ptrdiff_t dwSensitivity			 = 0x1E2ED08;
*/

namespace Offsets
{
	// -> offsets.h on a2x dumper
	constexpr uintptr_t		 dwEntityList			 = 0x24A7B48; // 
	constexpr uintptr_t		 dwLocalPlayerPawn		 = 0x2062850; //
	constexpr uintptr_t		 dwLocalPlayerController = 0x22ECA18; //
	constexpr uintptr_t		 dwViewMatrix			 = 0x2308850; //
	constexpr uintptr_t		 dwViewAngles			 = 0x2312A68; //
	constexpr uintptr_t		 dwGlobalVars			 = 0x20572A8; //
	constexpr std::ptrdiff_t dwSensitivity			 = 0x2304128; //

	// needs updates
	//constexpr std::ptrdiff_t dwSensitivity = 0x2303128;


	// -> client.dll.hpp on a2x dumper -> C_BaseEntity
	constexpr uintptr_t m_iHealth		 = 0x644;
	constexpr uintptr_t m_iTeamNum		 = 0x3F3;
	constexpr uintptr_t m_vOldOrigin	 = 0x1588;
	constexpr uintptr_t m_pGameSceneNode = 0x338;

	// -> CbaseModelEntity
	constexpr uintptr_t m_vecViewOffset = 0xD58;

	// -> C_CSPlayerPawn
	constexpr uintptr_t m_iShotsFired	= 0x270C;
	constexpr uintptr_t m_aimPunchAngle = 0x16CC;
	constexpr uintptr_t m_fFlags		= 0x400;

	// -> C_CSPlayerController
	constexpr uintptr_t m_szTeamname   = 0x63C; // idk the offset
	constexpr uintptr_t m_hPlayerPawn  = 0x90C;
	constexpr uintptr_t m_bPawnIsAlive = 0x914;

	// -> CBodyComponentSkeletonInstance
	constexpr uintptr_t m_modelState = 0x190;

	// -> CObserverServices
	constexpr std::ptrdiff_t m_pObserverServices  = 0x1408;
	constexpr std::ptrdiff_t m_hObserverTarget	  = 0x44;
	constexpr std::ptrdiff_t m_iObserverMode	  = 0x48;
	constexpr std::ptrdiff_t m_entitySpottedState = 0x1F98;
	constexpr std::ptrdiff_t m_aimPunchAngleVel	  = 0x16D8;

	// -> Buttons
	constexpr std::ptrdiff_t jump			 = 0x205BAF0;
	constexpr std::ptrdiff_t m_aimPunchCache = 0x1008;

}
