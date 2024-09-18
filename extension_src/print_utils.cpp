// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.cpp

#include "print_utils.h"

using namespace godot;

void PrintUtils::welcome_text(String version, String url)
{
#ifdef DEBUG_ENABLED
	PRINT_R("CineCam ", version, " initialized! GDExtension made by LumiToad! ([url]", url, "[/url])\n");
#endif
}


void PrintUtils::blend_before_init(int l, String f)
{
#ifdef DEBUG_ENABLED
	ERROR("Tried blending before tweens initialized!\n", "This is probably a bug in this GDExtension!");
#endif
}


void PrintUtils::no_cam2d_seq_found(int l, String f)
{
#ifdef DEBUG_ENABLED
	WARN("No CamSequence2D Node found!\n", "Use set_current_sequence method or assign it using the Inspector!");
#endif
}


void PrintUtils::no_cam3d_seq_found(int l, String f)
{
#ifdef DEBUG_ENABLED
	WARN("No CamSequence3D Node found!\n", "Use set_current_sequence method or assign it using the Inspector!");
#endif
}



void PrintUtils::idx_out_of_bounds(int l, String f, int idx, int size)
{
#ifdef DEBUG_ENABLED
	ERROR("Index [", idx, "] was outside the bounds of the array. (min 0, current size[", size, "])");
#endif
}


void PrintUtils::speed_0_100(int l, String f, int percent)
{
#ifdef DEBUG_ENABLED
	WARN("Speed must be between 0 % and 100 %, but was: ", percent);
#endif
}


void PrintUtils::no_highest_prio_cam2d(int l, String f, int amount)
{
#ifdef DEBUG_ENABLED
	WARN("Could not detect highest priority of any VirtualCam2D!\nNumber of found cams: ", amount);
#endif
}


void PrintUtils::no_highest_prio_cam3d(int l, String f, int amount)
{
#ifdef DEBUG_ENABLED
	WARN("Could not detect highest priority of any VirtualCam3D!\nNumber of found cams: ", amount);
#endif
}


void PrintUtils::no_target2d_found(int l, String f, String set_to, String follow_mode)
{
#ifdef DEBUG_ENABLED
	WARN("No CamTarget2D set! Follow mode set to: ", set_to, ". (current was : ", follow_mode, ")");
#endif
}


void PrintUtils::no_target3d_found(int l, String f, String set_to, String follow_mode)
{
#ifdef DEBUG_ENABLED
	WARN("No CamTarget3D set! Follow mode set to: ", set_to, ". (current was: ", follow_mode, ")");
#endif
}


void PrintUtils::ignoring_rotation(int l, String f)
{
#ifdef DEBUG_ENABLED
	WARN("The shake rotation has started, but will be ignored, as long as ignore_rotation is true!");
#endif
}


void PrintUtils::blend_running_command_ignored(int l, String f, String command)
{
#ifdef DEBUG_ENABLED
	WARN("A blend is currently running! ", command, " has been ignored!");
#endif
}

void godot::PrintUtils::scene_tree_nullptr(int l, String f)
{
#ifdef DEBUG_ENABLED
	ERROR("The SceneTree was null!");
#endif
}


void PrintUtils::test_warns()
{
	blend_before_init(__LINE__, __FILE__);
	no_cam2d_seq_found(__LINE__, __FILE__);
	idx_out_of_bounds(__LINE__, __FILE__, 0, 0);
	speed_0_100(__LINE__, __FILE__, 0);
	no_highest_prio_cam2d(__LINE__, __FILE__, 0);
	no_target2d_found(__LINE__, __FILE__, "BLA_OFF", "BLA_FOLLOW_BLEND");
	no_target3d_found(__LINE__, __FILE__, "BLA_OFF", "BLA_FOLLOW_BLEND");
}
