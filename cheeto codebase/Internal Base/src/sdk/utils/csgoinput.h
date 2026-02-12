#include "../../sdk/common.h"
#include "../memory/PatternScan.h"
#include "../utils/Memory.h"
#include "../utils/Utils.h"
#include "../utils/QAngle.h"
#include "usermode.h"

#define MULTIPLAYER_BACKUP 150

class CMsgQAngle
{
public:
	class CBasePB* base; //0x00
	char		   pad_0008[16]; //0x08
	QAngle		   angValue; //0x18
	char		   pad_0024[92]; //0x24
}; //Size: 0x0080

class CBasePB
{
public:
	void*	 __vftable; //0x00
	uint32_t nHasBits; //0x08
	uint64_t nCachedBits; //0x0C
	char	 pad_0014[48]; //0x14
}; //Size: 0x0044

class CCSGOInterpolationInfoPB
{
public:
	class CBasePB* base; //0x00
	char		   pad_0008[16]; //0x08
	float		   flFraction; //0x18
	int32_t		   nSrcTick; //0x1C
	int32_t		   nDstTick; //0x20
	char		   pad_0024[28]; //0x24
}; //Size: 0x0040

class CInButtonStatePB
{
public:
	class CBasePB* base; //0x00
	int64_t		   nValue; //0x08
	int64_t		   nValueChanged; //0x10
	int64_t		   nValueScroll; //0x18
	char		   pad_0020[32]; //0x20
}; //Size: 0x0040

class CSubtickMoveStep
{
public:
	class CBasePB* base; //0x00
	char		   pad_0008[16]; //0x08
	int64_t		   nButton; //0x18
	bool		   nPressed; //0x20
	char		   pad_0021[3]; //0x21
	float		   flWhen; //0x24
	float		   flAnalogForwardDelta; //0x28
	float		   flAnalogLeftDelta; //0x2C
	char		   pad_0030[24]; //0x30
}; //Size: 0x0048

class CSGOInputHistoryEntryPB
{
public:
	class CBasePB*	  base; //0x00
	char			  pad_0008[16]; //0x08
	class CMsgQAngle* pViewCmd; //0x18
	class CMsgQAngle* pShootPosition; //0x20
	char			  pad_0028[24]; //0x28
}; //Size: 0x0040

class CBaseUserCmdPB
{
public:
	void*					__vftable; //0x00
	uint32_t				nHasBits; //0x08
	int64_t					nCachedBits; //0x0C
	void*					pArena; //0x14
	int32_t					nCurrentSize; //0x1C
	int32_t					nTotalSize; //0x20
	class CSubtickMoveStep* pRep; //0x24
	char					pad_002C[8]; //0x2C
	class CInButtonStatePB* pInButtonState; //0x34
	class CMsgQAngle*		pViewAngles; //0x3C
	int32_t					nLegacyCommandNumber; //0x44
	int32_t					nClientTick; //0x48
	float					flForwardMove; //0x4C
	float					flSideMove; //0x50
	float					flUpMove; //0x54
	int32_t					nImpulse; //0x58
	int32_t					nWeaponSelect; //0x5C
	int32_t					nRandomSeed; //0x60
	int32_t					nMousedX; //0x64
	int32_t					nMousedY; //0x68
	uint32_t				nConsumedServerAngleChanges; //0x6C
	int32_t					nCmdFlags; //0x70
	uint32_t				nPawnEntityHandle; //0x74
	char					pad_0078[4]; //0x78
}; //Size: 0x007C

class CCSGOUserCmdPB
{
public:
	uint32_t			  nHasBits; //0x00
	char				  pad_0004[4]; //0x04
	uint64_t			  nCachedSize; //0x08
	char				  pad_0010[24]; //0x10
	class CBaseUserCmdPB* pBaseCmd; //0x28
	char				  pad_0030[16]; //0x30
}; //Size: 0x0040

class CInButtonState
{
public:
	void*	__vftable; //0x00
	int64_t nValue; //0x08
	int64_t nValueChanged; //0x10
	int64_t nValueScroll; //0x18
	char	pad_0020[32]; //0x20
}; //Size: 0x0040

class CUserCmd
{
public:
	void*				 __vftable; //0x00
	char				 pad_0008[16]; //0x08
	class CCSGOUserCmdPB csgoUserCmd; //0x18
	class CInButtonState nButtons; //0x58
	char				 pad_0098[24]; //0x98
}; //Size: 0x00B0
