// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.h

#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#define PRINT(...) UtilityFunctions::print(__VA_ARGS__)
#define PRINT_R(...) UtilityFunctions::print_rich(__VA_ARGS__)
#define WARN(...) UtilityFunctions::push_warning("CineCam WARN! Line: ", l, " in File: ", f, "\n", __VA_ARGS__, "\n")
#define ERROR(...) UtilityFunctions::push_error("CineCam ERROR! Line: ", l, " in File: ", f, "\n", __VA_ARGS__, "\n")

namespace godot
{
	static class PrintUtils
	{
	public:
		static void welcome_text(String version, String url);
		static void blend_before_init(int l, String f);
		static void no_cam2d_seq_found(int l, String f);
		static void no_cam3d_seq_found(int l, String f);
		static void idx_out_of_bounds(int l, String f, int idx, int size);
		static void speed_0_100(int l, String f,int percent);
		static void no_highest_prio_cam2d(int l, String f, int amount);
		static void no_highest_prio_cam3d(int l, String f, int amount);
		static void no_target2d_found(int l, String f, String set_to, String follow_mode);
		static void no_target3d_found(int l, String f, String set_to, String follow_mode);
		static void ignoring_rotation(int l, String f);

		static void test_warns();
	};
}