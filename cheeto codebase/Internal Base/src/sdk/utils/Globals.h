#pragma once
#include <cstdint>
#include <Windows.h>

namespace Globals
{
	// Standard / HOOKS
	inline float ViewMatrix[16] = { 0.f };
	inline int	 ScreenWidth	= 0;
	inline int	 ScreenHeight	= 0;

	// ESP
	inline bool esp_enabled = false;
	inline int	esp_bind	= VK_F1;
	inline bool esp_box		= false;
	inline bool esp_teamate = false;

	// COLORS
	inline float esp_box_color[4]	= { 0.f, 0.f, 1.f, 1.f };
	inline float esp_box_color_2[4] = { 1.f, 0.f, 0.f, 1.f };
	inline float esp_box_thickness	= 1.5f;

	// SKELETON
	inline bool	 esp_skeleton			= false;
	inline float esp_skeleton_color[4]	= { 1.f, 1.f, 1.f, 0.9f };
	inline float esp_skeleton_thickness = 1.8f;

	// NAME
	inline bool	 esp_name		   = false;
	inline float esp_name_color[4] = { 1.f, 1.f, 1.f, 1.f };

	// HEALTH
	inline bool esp_health = false;

	// BHOP
	inline bool bhop_enabled = false;

	// AIMBOT
	inline float aimbot_shoot_threshold = 2.f; // fov threshold in degrees before shooting
	inline DWORD aimbot_shoot_delay		= 35;  // delay before shooting after getting on target
	inline DWORD aimbot_fire_rate		= 35;  // min time between shots in ms
	inline bool	 aimbot_enabled			= false;
	inline bool	 aimbot_friendly_fire	= false;
	inline int	 aimbot_key				= VK_LBUTTON;
	inline float aimbot_fov				= 15.0f;
	inline bool	 aimbot_smooth			= false;
	inline float aimbot_smoothness		= 0.1f;
	inline bool	 aimbot_force_baim		= false;
	inline int	 aimbot_baim_min		= 30;

	// AUTO FIRE
	inline bool	 autofire_enabled = false;
	inline float autofire_fov	  = 0.3f;

	// NO RECOIL
	inline bool norecoil_enabled = false;

	// FOV CROSSHAIR
	inline bool fov_crosshair_enabled = false;

	// AUTO STOP
	inline bool autostop_enabled = false;
	
}
