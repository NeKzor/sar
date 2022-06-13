#pragma once

#include "Math.hpp"

#define CONTENTS_EMPTY 0x0
#define CONTENTS_SOLID 0x1
#define CONTENTS_WINDOW 0x2
#define CONTENTS_AUX 0x4
#define CONTENTS_GRATE 0x8
#define CONTENTS_SLIME 0x10
#define CONTENTS_WATER 0x20
#define CONTENTS_BLOCKLOS 0x40
#define CONTENTS_OPAQUE 0x80
#define LAST_VISIBLE_CONTENTS 0x80
#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS - 1))
#define CONTENTS_TESTFOGVOLUME 0x100
#define CONTENTS_UNUSED 0x200
#define CONTENTS_UNUSED6 0x400
#define CONTENTS_TEAM1 0x800
#define CONTENTS_TEAM2 0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define CONTENTS_MOVEABLE 0x4000
#define CONTENTS_AREAPORTAL 0x8000
#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000
#define CONTENTS_CURRENT_0 0x40000
#define CONTENTS_CURRENT_90 0x80000
#define CONTENTS_CURRENT_180 0x100000
#define CONTENTS_CURRENT_270 0x200000
#define CONTENTS_CURRENT_UP 0x400000
#define CONTENTS_CURRENT_DOWN 0x800000
#define CONTENTS_ORIGIN 0x1000000
#define CONTENTS_MONSTER 0x2000000
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_DETAIL 0x8000000
#define CONTENTS_TRANSLUCENT 0x10000000
#define CONTENTS_LADDER 0x20000000
#define CONTENTS_HITBOX 0x40000000

#define SURF_LIGHT 0x0001
#define SURF_SKY2D 0x0002
#define SURF_SKY 0x0004
#define SURF_WARP 0x0008
#define SURF_TRANS 0x0010
#define SURF_NOPORTAL 0x0020
#define SURF_TRIGGER 0x0040
#define SURF_NODRAW 0x0080
#define SURF_HINT 0x0100
#define SURF_SKIP 0x0200
#define SURF_NOLIGHT 0x0400
#define SURF_BUMPLIGHT 0x0800
#define SURF_NOSHADOWS 0x1000
#define SURF_NODECALS 0x2000
#define SURF_NOCHOP 0x4000
#define SURF_HITBOX 0x8000

#define MASK_ALL (0xFFFFFFFF)
#define MASK_SOLID (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define MASK_PLAYERSOLID (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define MASK_NPCSOLID (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE)
#define MASK_WATER (CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME)
#define MASK_OPAQUE (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS (MASK_OPAQUE | CONTENTS_MONSTER)
#define MASK_BLOCKLOS (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS (MASK_BLOCKLOS | CONTENTS_MONSTER)
#define MASK_VISIBLE (MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS (MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_SHOT (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX)
#define MASK_SHOT_HULL (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE)
#define MASK_SHOT_PORTAL (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE)
#define MASK_SPLITAREAPORTAL (CONTENTS_WATER | CONTENTS_SLIME)
#define MASK_CURRENT (CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN)

struct csurface_t {
	const char *name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t {
	Vector normal;
	float dist;
	unsigned char type;
	unsigned char signbits;
	unsigned char pad[2];
};

struct CBaseTrace {
	Vector startpos;
	Vector endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;
};

struct CGameTrace : public CBaseTrace {
	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	void *m_pEnt;
	int hitbox;
};

struct Ray_t {
	VectorAligned m_Start;        // starting point, centered within the extents
	VectorAligned m_Delta;        // direction + length of the ray
	VectorAligned m_StartOffset;  // Add this to m_Start to get the actual ray start
	VectorAligned m_Extents;      // Describes an axis aligned box extruded along a ray
	const matrix3x4_t *m_pWorldAxisTransform = nullptr;
	bool m_IsRay;    // are there extents zero
	bool m_IsSwept;  // is delta != 0
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(void *pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter {
public:
	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterSimple : public CTraceFilter {
public:
	virtual bool ShouldHitEntity(void *pHandleEntity, int contentsMask) {
		return pHandleEntity != m_pPassEnt;
		//	return false;
	};
	virtual void SetPassEntity(const void *pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

private:
	const void *m_pPassEnt;
	int m_collisionGroup;
};