#pragma once
#include <cstdint>
#include "../memory/Offsets.h"
#include "../utils/Vector.h"
#include "../utils/Memory.h"        
#include "../memory/PatternScan.h"  
#define SCHEMA(type, name, offset) \
    type name() const { \
        return *reinterpret_cast<const type*>(reinterpret_cast<uintptr_t>(this) + offset); \
    }

class CGameSceneNode
{
public:
    SCHEMA(uintptr_t, m_modelState, Offsets::m_modelState);
};

class C_Player_ObserverServices {
public:
    SCHEMA(uint32_t, m_hObserverTarget, Offsets::m_hObserverTarget);
};

class C_BaseEntity
{
public:
    SCHEMA(int, m_iHealth, Offsets::m_iHealth);
    SCHEMA(int, m_iTeamNum, Offsets::m_iTeamNum);
    SCHEMA(Vector, m_vOldOrigin, Offsets::m_vOldOrigin);
    SCHEMA(uintptr_t, m_pGameSceneNode, Offsets::m_pGameSceneNode);

    bool IsAlive() const { return m_iHealth() > 0; }
};

class C_CSPlayerPawn : public C_BaseEntity
{
public:

	struct EntitySpottedState_t
	{
		uint64_t m_bSpottedByMask[2];
	};

	struct C_UTL_VECTOR
	{
		uintptr_t data;
		int		  count;
		int		  size;
	};

    SCHEMA(Vector, m_vecViewOffset, Offsets::m_vecViewOffset);
    SCHEMA(int, m_iShotsFired, Offsets::m_iShotsFired);
    SCHEMA(Vector, m_aimPunchAngle, Offsets::m_aimPunchAngle);
	SCHEMA(Vector, m_aimPunchAngleVel, Offsets::m_aimPunchAngleVel);
    SCHEMA(uintptr_t, m_pObserverServices, Offsets::m_pObserverServices);
	SCHEMA(C_UTL_VECTOR, m_aimPunchCache, Offsets::m_aimPunchCache);

    // this is for the fFlag for bhop
    SCHEMA(int, m_fFlags, Offsets::m_fFlags);
    bool IsOnGround() const { return (m_fFlags() & 1) != 0; }

	// for the entity spotted state
	SCHEMA(EntitySpottedState_t, m_entitySpottedState, Offsets::m_entitySpottedState);


	bool IsSpotted() const
	{
		return *(bool*)((uintptr_t)this + Offsets::m_entitySpottedState + 0x8);
	}

};

class C_CSPlayerController : public C_BaseEntity
{
public:
    SCHEMA(uint32_t, m_hPlayerPawn, Offsets::m_hPlayerPawn);
	SCHEMA(const char*, m_szTeamname, Offsets::m_szTeamname);
    SCHEMA(bool, m_bPawnIsAlive, Offsets::m_bPawnIsAlive);


	static C_CSPlayerController* GetLocalPlayer()
	{
		static uintptr_t clientBase = 0;
		if (!clientBase)
			clientBase = Memory::GetModuleBase("client.dll");

		if (!clientBase) return nullptr;

		uintptr_t localController = *(uintptr_t*)(clientBase + Offsets::dwLocalPlayerController);

		if (!localController) return nullptr;

		return reinterpret_cast<C_CSPlayerController*>(localController);
	}

};

