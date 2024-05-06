// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.h

#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#define PRINT(...) UtilityFunctions::print(...)
#define WARN(...) UtilityFunctions::push_warning("CineCam WARN! Line: ", __LINE__, " in File: ", __FILE__, "\n", __VA_ARGS__, \n)
#define ERROR(...) UtilityFunctions::push_error("CineCam ERROR! Line: ", __LINE__, " in File: ", __FILE__, "\n", __VA_ARGS__, \n)

namespace godot
{
	static class PrintUtils
	{
	public:
		static void blend_before_init();
		static void no_cam_seq_found();
		static void idx_out_of_bounds(int idx, int size);
		static void speed_0_100(int percent);
		static void no_highest_prio_cam2d(int amount);
		static void test_warns();
	};
}