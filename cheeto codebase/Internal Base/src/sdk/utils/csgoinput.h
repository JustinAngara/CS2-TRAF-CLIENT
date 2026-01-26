#include "../../sdk/common.h"
#include "../memory/PatternScan.h"
#include "../utils/Memory.h"
#include "usermode.h"

#define MULTIPLAYER_BACKUP 150

class CUserCmdArray
{
public:
	inline uint32_t& m_nSequenceNumber()
	{
		return *(uint32_t*)((uintptr_t)this + 0x59A8);
	}
};

///////////////////////////
/// @brief ADD NEW OFFSETS
///////////////////////////
class CCSGOInput
{
public:
	void* pVtable;
	MEM_PAD(12344);
	CUserCmd arrCommands[150];
	int8_t N0000065C; //0x8E00
	bool bInThirdPerson;
	MEM_PAD(6);
	QAngle_t m_vecThridpersonAngles;
	MEM_PAD(16);
	int32_t nSequenceNumber;

	int32_t iOldSequence; //0x8E28
	uint32_t pad; //0x8E2C
	double m_Unknown; //0x8E30 look's like a timer
	uint64_t m_iButtonState1; //0x8E38
	MEM_PAD(16);
	uint64_t m_iButtonState2; //0x8E50
	MEM_PAD(32);
	int32_t N0000066A; //0x8E78 padding
	MEM_PAD(12);
	uint32_t N0000066C; //0x8E88 padding
	MEM_PAD(164);
	QAngle_t m_vecViewangles; //0x8F30
	MEM_PAD(232);
	uint32_t N0000069F; //0x9024 padding
	uint32_t m_buttons; //0x9028
	uint32_t N000006A0; //0x902C padding
	MEM_PAD(20);
	int32_t N000006A3; //0x9044 padding
	int32_t N0000178A; //0x9048 padding
	float m_flUnknownTimer; //0x904C
	MEM_PAD(28);

    CUserCmd** getFirstCUserCmdArray()
	{
		static uintptr_t pattern = Memory::PatternScan("client.dll", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B F8");
		if (!pattern)
			return nullptr;

		// Resolve relative address
		int32_t offset = *(int32_t*)(pattern + 3);
		uintptr_t addr = pattern + 7 + offset;
		return *(CUserCmd***)addr;
	}

	void getCUserCmdTick(void* controller, int32_t* outputTick)
	{
		using Fn = void(__fastcall*)(void*, int32_t*);
		static Fn fn = (Fn)Memory::PatternScan("client.dll", "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA");
		if (fn)
			fn(controller, outputTick);
	}

	CUserCmdArray* getCUserCmdArray(CUserCmd** array, int tick)
	{
		using Fn = CUserCmdArray*(__fastcall*)(CUserCmd**, int);
		static Fn fn = (Fn)Memory::PatternScan("client.dll", "48 89 4C 24 ? 41 56 41 57");
		if (fn)
			return fn(array, tick);
		return nullptr;
	}

	CUserCmd* getCUserCmdBySequenceNumber(void* controller, uint32_t sequenceNumber)
	{
		using Fn = CUserCmd*(__fastcall*)(void*, uint32_t);
		static Fn fn = (Fn)Memory::PatternScan("client.dll", "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0");
		if (fn)
			return fn(controller, sequenceNumber);
		return nullptr;
	}

	uint32_t getSequenceNumber(void* localPlayer)
	{
		int32_t outputTick = 0;
		getCUserCmdTick(localPlayer, &outputTick);

		int32_t tick = outputTick - 1;
		if (outputTick == -1)
			tick = -1;

		auto* userCmdArray = getCUserCmdArray(getFirstCUserCmdArray(), tick);
		if (userCmdArray)
			return userCmdArray->m_nSequenceNumber();

		return 0;
	}

	CUserCmd* getUserCmd(void* localPlayer)
	{
		const auto sequenceNumber = getSequenceNumber(localPlayer);
		if (sequenceNumber)
			return getCUserCmdBySequenceNumber(localPlayer, sequenceNumber);
		return nullptr;
	}

	CUserCmd* GetUserCmd()
	{
		return &arrCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
	}



	
};
