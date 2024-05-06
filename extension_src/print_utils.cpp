// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.cpp

#include "print_utils.h"

using namespace godot;


void PrintUtils::welcome_text()
{
	String CINECAM_VERSION = "v1.0";
	String CINECAM_URL = "https://github.com/lumitoad/";
#ifdef DEBUG_ENABLED
	PRINT("CineCam ", CINECAM_VERSION, " initialized! GDExtension made by LumiToad! (", CINECAM_URL, ")\n");
#endif
}

void PrintUtils::blend_before_init()
{
#ifdef DEBUG_ENABLED
	WARN("Tried blending before tweens initialized!\n", "This is probably a bug in this GDExtension!");
#endif
}


void PrintUtils::no_cam_seq_found()
{
#ifdef DEBUG_ENABLED
	WARN("No CamSequence2D Node found!\n", "Use set_current_sequence method or assign it using the Inspector!");
#endif
}


void PrintUtils::idx_out_of_bounds(int idx, int size)
{
#ifdef DEBUG_ENABLED
	WARN("Index [", idx, "] was outside the bounds of the array. (min 0, current size[", size, "])");
#endif
}


void PrintUtils::speed_0_100(int percent)
{
#ifdef DEBUG_ENABLED
	WARN("Speed must be between 0 % and 100 %, but was: ", percent);
#endif
}


void PrintUtils::no_highest_prio_cam2d(int amount)
{
#ifdef DEBUG_ENABLED
	WARN("Could not detect highest priority of any VirtualCam2D!\nNumber of found cams: ", amount);
#endif
}


void PrintUtils::no_highest_prio_cam3d(int amount)
{
#ifdef DEBUG_ENABLED
	WARN("Could not detect highest priority of any VirtualCam3D!\nNumber of found cams: ", amount);
#endif
}


void PrintUtils::test_warns()
{
	blend_before_init();
	no_cam_seq_found();
	idx_out_of_bounds(0, 0);
	speed_0_100(0);
	no_highest_prio_cam2d(0);
}
