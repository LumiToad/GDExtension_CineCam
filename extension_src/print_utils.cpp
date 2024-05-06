// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.cpp

#include "print_utils.h"

using namespace godot;


String PrintUtils::get_print(Prints value)
{
	String ret_val;

	return ret_val;
}


String PrintUtils::get_warn(Warnings value)
{
	String ret_val = "CineCam WARN! ";

	switch (value)
	{
	case PrintUtils::BLEND_BEFORE_INIT:
		ret_val += "Tried blending before tweens initialized!\n";
		ret_val += "This is probably a bug in this GDExtension!";
		break;
	case PrintUtils::NO_CAM_SEQ_FOUND:
		ret_val += "No CamSequence2D Node found!\n";
		ret_val += "Use set_current_sequence method or assign it using the Inspector!";
		break;
	case PrintUtils::IDX_OUT_OF_BOUNDS:
		ret_val += "] was outside the bounds of the array. (min 0, current size [";
		break;
	case PrintUtils::SPEED_0_100:
		ret_val += "Speed must be between 0 % and 100 %, but was: ";
		break;
	case PrintUtils::NO_HIGHEST_PRIO_CAM2D:
		ret_val += "Could not detect highest priority of any VirtualCam2D!\nNumber of found cams: ";
		break;
	default:
		break;
	}

	return ret_val;
}


String PrintUtils::get_err(Errors value)
{
	String ret_val;

	return ret_val;
}


void PrintUtils::blend_before_init()
{
	WARN(get_warn(BLEND_BEFORE_INIT));
}


void PrintUtils::no_cam_seq_found()
{
	WARN(get_warn(NO_CAM_SEQ_FOUND));
}


void PrintUtils::idx_out_of_bounds(int idx, int size)
{
	String string = "Index [";
	string += idx;
	string += get_warn(IDX_OUT_OF_BOUNDS);
	string += "]";
	WARN(string);
}


void PrintUtils::speed_0_100(int percent)
{
	WARN(
		get_warn(SPEED_0_100), percent
	);
}


void PrintUtils::no_highest_prio_cam2d(int amount)
{
	WARN(
		get_warn(NO_HIGHEST_PRIO_CAM2D), amount
	);
}


void PrintUtils::test_warns()
{
	blend_before_init();
	no_cam_seq_found();
	idx_out_of_bounds(0, 0);
	speed_0_100(0);
	no_highest_prio_cam2d(0);
}
