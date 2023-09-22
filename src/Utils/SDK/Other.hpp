#pragma once
#pragma warning(suppress : 26495)
#include "Offsets.hpp"

#include "Color.hpp"
#include "Handle.hpp"
#include "Trace.hpp"
#include "UtlMemory.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>

struct cmdalias_t {
	cmdalias_t *next;
	char name[32];
	char *value;
};

struct GameOverlayActivated_t {
	uint8_t m_bActive;
};

enum PaintMode_t {
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
};


struct PaintPowerInfo_t {
	void *vtable;
	Vector m_SurfaceNormal;
	Vector m_ContactPoint;
	int m_PaintPowerType;
	CBaseHandle m_HandleToOther;
	int m_State;
	bool m_IsOnThinSurface;
};


enum {
	PORTAL_COND_TAUNTING = 0,
	PORTAL_COND_POINTING,
	PORTAL_COND_DROWNING,
	PORTAL_COND_DEATH_CRUSH,
	PORTAL_COND_DEATH_GIB,
	PORTAL_COND_LAST
};


class IMaterial;
class IMaterialInternal;

class CMaterial_QueueFriendly {
public:
	void* vtable;
	IMaterialInternal* m_pRealTimeVersion;
};

struct CFontAmalgam {
	struct TFontRange {
		int lowRange;
		int highRange;
		void *pFont;
	};

	CUtlVector<TFontRange> m_Fonts;
	int m_iMaxWidth;
	int m_iMaxHeight;
};


struct FcpsTraceAdapter {
	void (*traceFunc)(const Ray_t &ray, CGameTrace *result, FcpsTraceAdapter *adapter);
	bool (*pointOutsideWorldFunc)(const Vector &test, FcpsTraceAdapter *adapter);
	ITraceFilter *traceFilter;
	unsigned mask;
};

class CPlayerState {
public:
	void *vtable;
	void *m_hTonemapController;
	QAngle v_angle;
};

struct ChallengeNodeData_t {
    const char* m_szMapName;
    Vector m_vecNodeOrigin;
    QAngle m_vecNodeAngles;
};

struct ChapterContextData_t {
    const char* m_szMapName;
    int m_nChapter;
    int m_nSubChapter;
};

class CBotCmd;

class IPlayerInfo {
public:
    virtual const char* GetName() = 0;
    virtual int GetUserID() = 0;
    virtual const char* GetNetworkIDString() = 0;
    virtual int GetTeamIndex() = 0;
    virtual void ChangeTeam(int iTeamNum) = 0;
    virtual int GetFragCount() = 0;
    virtual int GetDeathCount() = 0;
    virtual bool IsConnected() = 0;
    virtual int GetArmorValue() = 0;
    virtual bool IsHLTV() = 0;
    virtual bool IsPlayer() = 0;
    virtual bool IsFakeClient() = 0;
    virtual bool IsDead() = 0;
    virtual bool IsInAVehicle() = 0;
    virtual bool IsObserver() = 0;
    virtual const Vector GetAbsOrigin() = 0;
    virtual const QAngle GetAbsAngles() = 0;
    virtual const Vector GetPlayerMins() = 0;
    virtual const Vector GetPlayerMaxs() = 0;
    virtual const char* GetWeaponName() = 0;
    virtual const char* GetModelName() = 0;
    virtual const int GetHealth() = 0;
    virtual const int GetMaxHealth() = 0;
    virtual CBotCmd GetLastUserCommand() = 0;
};

class IPlayerInfoManager
{
public:
    virtual IPlayerInfo* GetPlayerInfo(edict_t * pEdict) = 0;
    virtual CGlobalVars* GetGlobalVars() = 0;
};

struct PortalPlayerStatistics_t {
    void* unk;
    int iNumPortalsPlaced;
    int iNumStepsTaken;
    float fNumSecondsTaken;
    float fDistanceTaken;
};
