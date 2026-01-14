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
	inline bool esp_enabled = true;
	inline int	esp_bind	= VK_F1;
	inline bool esp_box		= true;
	inline bool esp_teamate = true;

	// colors
	inline float esp_box_color[4]	= { 0.f, 0.f, 1.f, 1.f };
	inline float esp_box_color_2[4] = { 1.f, 0.f, 0.f, 1.f };
	inline float esp_box_thickness	= 1.5f;

	// skeleeton
	inline bool	 esp_skeleton			= true;
	inline float esp_skeleton_color[4]	= { 1.f, 1.f, 1.f, 0.9f };
	inline float esp_skeleton_thickness = 1.8f;

	// name
	inline bool	 esp_name		   = true;
	inline float esp_name_color[4] = { 1.f, 1.f, 1.f, 1.f };

	//health
	inline bool esp_health = true;

	// BHOP
	inline bool bhop_enabled = true;

	// AIMBOT
	inline float aimbot_shoot_threshold = 2.f; // fov threshold in degrees before shooting
	inline DWORD aimbot_shoot_delay		= 35;  // delay before shooting after getting on target
	inline DWORD aimbot_fire_rate		= 35;  // min time between shots in ms
	inline bool	 aimbot_enabled			= true;
	inline bool	 aimbot_friendly_fire	= false;
	inline int	 aimbot_key				= VK_LBUTTON;
	inline float aimbot_fov				= 15.0f;
	inline bool	 aimbot_smooth			= true;
	inline float aimbot_smoothness		= 0.1f;
	inline bool	 aimbot_force_baim		= true;
	inline int	 aimbot_baim_min		= 30;

	// AUTO FIRE
	inline bool	 autofire_enabled = false;
	inline float autofire_fov	  = 0.3f;
}
