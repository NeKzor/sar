#include "VphysHud.hpp"

#include "Command.hpp"
#include "Features/EntityList.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Scheme.hpp"
#include "Modules/Server.hpp"
#include "Modules/Surface.hpp"
#include "Variable.hpp"

VphysHud vphysHud;

Variable sar_vphys_hud("sar_vphys_hud", "0", 0, "Enables or disables the vphys HUD.\n");
Variable sar_vphys_hud_font("sar_vphys_hud_font", "10", 0, "Sets font of the vphys HUD.\n");
Variable sar_vphys_hud_precision("sar_vphys_hud_precision", "3", 1, 16, "Sets decimal precision of the vphys HUD.\n");
Variable sar_vphys_hud_x("sar_vphys_hud_x", "0", 0, "The x position of the vphys HUD.\n");
Variable sar_vphys_hud_y("sar_vphys_hud_y", "0", 0, "The y position of the vphys HUD.\n");

VphysHud::VphysHud()
	: Hud(HudType_InGame, true)
{
}
bool VphysHud::ShouldDraw()
{
	return sar_vphys_hud.GetBool() && Hud::ShouldDraw() && sv_cheats.GetBool();
}
void VphysHud::Paint(int slot)
{
	auto font = scheme->GetDefaultFont() + sar_vphys_hud_font.GetInt();

	auto cX = sar_vphys_hud_x.GetInt();
	auto cY = sar_vphys_hud_y.GetInt();

	auto fh = surface->GetFontHeight(font);

	auto drawPhysicsInfo = [=](int x, int y, bool crouched, const char *name) {
		auto shadowInfo = GetVphysInfo(slot, crouched);

		auto posColor = Color(255,255,255, shadowInfo.collisionEnabled ? 255 : 100);
		auto enableColor = Color(128, 255, 128, shadowInfo.collisionEnabled ? 255 : 100);
		auto disableColor = Color(255, 128, 128, shadowInfo.collisionEnabled ? 255 : 100);

		auto p = sar_vphys_hud_precision.GetInt();

		surface->DrawTxt(font, x, y, posColor, "%s: ", name);
		surface->DrawTxt(font, x + 10, (y += fh), posColor, "pos: %.*f, %.*f, %.*f", 
			p, shadowInfo.position.x, p, shadowInfo.position.y, p, shadowInfo.position.z);
		surface->DrawTxt(font, x + 10, (y += fh), posColor, "ang: %.*f, %.*f, %.*f", 
			p, shadowInfo.rotation.x, p, shadowInfo.rotation.y, p, shadowInfo.rotation.z);
		surface->DrawTxt(font, x + 10, (y += fh), posColor, "vel: %.*f, %.*f, %.*f", 
			p, shadowInfo.velocity.x, p, shadowInfo.velocity.y, p, shadowInfo.velocity.z);
		surface->DrawTxt(font, x + 10, (y += fh), posColor, "angvel: %.*f, %.*f, %.*f", 
			p, shadowInfo.angularVelocity.x, p, shadowInfo.angularVelocity.y, p, shadowInfo.angularVelocity.z);

		surface->DrawTxt(font, x + 10, (y += fh), shadowInfo.collisionEnabled ? enableColor : disableColor,
			"collision: %s", shadowInfo.collisionEnabled ? "enabled" : "disabled");
		surface->DrawTxt(font, x + 10, (y += fh), shadowInfo.gravityEnabled ? enableColor : disableColor,
			"gravity: %s", shadowInfo.gravityEnabled ? "enabled" : "disabled");
		surface->DrawTxt(font, x + 10, (y += fh), shadowInfo.asleep ? enableColor : disableColor,
			"asleep: %s", shadowInfo.asleep ? "yes" : "no");
	};

	drawPhysicsInfo(cX + 5, cY + 5, false, "shadowStand");
	drawPhysicsInfo(cX + 5, cY + 5 + int(fh * 9.5f), true, "shadowCrouch");
}
bool VphysHud::GetCurrentSize(int &xSize, int &ySize)
{
	return false;
}
VphysShadowInfo VphysHud::GetVphysInfo(int slot, bool crouched)
{
	auto info = VphysShadowInfo();

	info.player = server->GetPlayer(slot + 1);
	if (!info.player) return info;

	auto hitboxOffset = crouched ? Offsets::m_pShadowCrouch : Offsets::m_pShadowStand;

	info.shadow = *reinterpret_cast<void **>((uintptr_t)info.player + hitboxOffset);

	using _IsAsleep = bool(__rescall *)(void *thisptr);
	using _IsEnabled = bool(__rescall *)(void *thisptr);
	using _GetPosition = void(__rescall *)(void *thisptr, Vector *worldPosition, QAngle *angles);
	using _GetVelocity = void(__rescall *)(void *thisptr, Vector *velocity, Vector *angularVelocity);

	auto IsAsleep = Memory::VMT<_IsAsleep>(info.shadow, Offsets::IsAsleep);
	auto IsCollisionEnabled = Memory::VMT<_IsEnabled>(info.shadow, Offsets::IsCollisionEnabled);
	auto IsGravityEnabled = Memory::VMT<_IsEnabled>(info.shadow, Offsets::IsGravityEnabled);
	auto GetPosition = Memory::VMT<_GetPosition>(info.shadow, Offsets::GetPosition);
	auto GetVelocity = Memory::VMT<_GetVelocity>(info.shadow, Offsets::GetVelocity);

	GetPosition(info.shadow, &info.position, &info.rotation);
	GetVelocity(info.shadow, &info.velocity, &info.angularVelocity);
	info.collisionEnabled = IsCollisionEnabled(info.shadow);
	info.gravityEnabled = IsGravityEnabled(info.shadow);
	info.asleep = IsAsleep(info.shadow);

	return info;
}

CON_COMMAND(sar_vphys_setgravity, "sar_vphys_setgravity <hitbox> <enabled> - sets gravity flag state to either standing (0) or crouching (1) havok collision shadow\n")
{
	if (engine->demoplayer->IsPlaying()) {
		return;
	}

	if (!sv_cheats.GetBool()) {
		return console->Print("Cannot use sar_vphys_setgravity without sv_cheats set to 1.\n");
	}

	if (args.ArgC() != 3) {
		return console->Print(sar_vphys_setgravity.ThisPtr()->m_pszHelpString);
	}

	auto player = server->GetPlayer(1);
	auto m_pShadowStand = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowStand);
	auto m_pShadowCrouch = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowCrouch);

	using _EnableGravity = bool(__rescall *)(void *thisptr, bool enabled);
	auto EnableGravity = Memory::VMT<_EnableGravity>(m_pShadowStand, Offsets::EnableGravity);

	auto hitbox = std::atoi(args[1]);
	auto enabled = std::atoi(args[2]);

	EnableGravity(hitbox ? m_pShadowCrouch : m_pShadowStand, enabled);
}

CON_COMMAND(sar_vphys_setangle, "sar_vphys_setangle <hitbox> <angle> [component = z] - sets rotation angle to either standing (0) or crouching (1) havok collision shadow\n")
{
	if (engine->demoplayer->IsPlaying()) {
		return;
	}

	if (!sv_cheats.GetBool()) {
		return console->Print("Cannot use sar_vphys_setangle without sv_cheats set to 1.\n");
	}

	if (args.ArgC() < 3 || args.ArgC() > 4) {
		return console->Print(sar_vphys_setangle.ThisPtr()->m_pszHelpString);
	}

	auto player = server->GetPlayer(1);
	auto m_pShadowStand = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowStand);
	auto m_pShadowCrouch = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowCrouch);

	using _SetPosition = void(__rescall *)(void *thisptr, const Vector &worldPosition, const QAngle &angles, bool isTeleport);
	using _GetPosition = void(__rescall *)(void *thisptr, Vector *worldPosition, QAngle *angles);
	auto SetPosition = Memory::VMT<_SetPosition>(m_pShadowStand, Offsets::SetPosition);
	auto GetPosition = Memory::VMT<_GetPosition>(m_pShadowStand, Offsets::GetPosition);

	auto hitbox = std::atoi(args[1]);
	auto angle = float(std::atof(args[2]));
	auto component = args.ArgC() == 4 ? args[3] : "z";

	auto selected = hitbox ? m_pShadowCrouch : m_pShadowStand;

	auto v = Vector();
	auto q = QAngle();
	GetPosition(selected, &v, &q);

	if (!strcmp(component, "x")) {
		q.x = angle;
	} else if (!strcmp(component, "y")) {
		q.y = angle;
	} else {
		q.z = angle;
	}

	SetPosition(selected, v, q, false);
}

CON_COMMAND(sar_vphys_setspin, "sar_vphys_setspin <hitbox> <angvel> [component = x] - sets rotation speed to either standing (0) or crouching (1) havok collision shadow\n") {
	if (engine->demoplayer->IsPlaying()) {
		return;
	}
	if (!sv_cheats.GetBool()) {
		return console->Print("Cannot use sar_vphys_setspin without sv_cheats set to 1.\n");
	}

	if (args.ArgC() < 3 || args.ArgC() > 4) {
		return console->Print(sar_vphys_setspin.ThisPtr()->m_pszHelpString);
	}

	auto player = server->GetPlayer(1);
	auto m_pShadowStand = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowStand);
	auto m_pShadowCrouch = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowCrouch);

	using _GetVelocity = void(__rescall *)(void *thisptr, Vector *velocity, Vector *angularVelocity);
	auto GetVelocity = Memory::VMT<_GetVelocity>(m_pShadowStand, Offsets::GetVelocity);
	using _SetVelocity = void(__rescall *)(void *thisptr, const Vector *velocity, const Vector *angularVelocity);
	auto SetVelocity = Memory::VMT<_SetVelocity>(m_pShadowStand, Offsets::SetVelocity);

	auto hitbox = std::atoi(args[1]);
	auto angvel = float(std::atof(args[2]));
	auto component = args.ArgC() == 4 ? args[3] : "x";

	auto selected = hitbox ? m_pShadowCrouch : m_pShadowStand;

	auto v = Vector();
	GetVelocity(selected, NULL, &v);
	
	if (!strcmp(component, "y")) {
		v.y = angvel;
	} else if (!strcmp(component, "z")) {
		v.z = angvel;
	} else {
		v.x = angvel;
	}

	SetVelocity(selected, NULL, &v);
}

CON_COMMAND(sar_vphys_setasleep, "sar_vphys_setasleep <hitbox> <asleep> - sets whether your standing (0) or crouching (1) havok collision shadow is asleep\n")
{
	if (engine->demoplayer->IsPlaying()) {
		return;
	}
	if (!sv_cheats.GetBool()) {
		return console->Print("Cannot use sar_vphys_setasleep without sv_cheats set to 1.\n");
	}

	if (args.ArgC() != 3) {
		return console->Print(sar_vphys_setasleep.ThisPtr()->m_pszHelpString);
	}

	auto player = server->GetPlayer(1);
	if (!player) return;

	auto m_pShadowStand = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowStand);
	auto m_pShadowCrouch = *reinterpret_cast<void **>((uintptr_t)player + Offsets::m_pShadowCrouch);

	auto hitbox = std::atoi(args[1]);
	auto asleep = std::atoi(args[2]);

	auto selected = hitbox ? m_pShadowCrouch : m_pShadowStand;

	using _SleepWake = void(__rescall *)(void *thisptr);
	auto SleepWake = Memory::VMT<_SleepWake>(selected, asleep ? Offsets::Sleep : Offsets::Wake);
	SleepWake(selected);
}
