// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// print_utils.h

#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#define PRINT(STRING) UtilityFunctions::print((STRING))
#define WARN(STRING) UtilityFunctions::push_warning((STRING))
#define ERROR(STRING) UtilityFunctions::push_error((STRING))

namespace godot
{
	static class PrintUtils
	{
	public:
		static enum Prints
		{

		};

		static enum Warnings
		{
			BLEND_BEFORE_INIT,
			NO_CAM_SEQ_FOUND,
			IDX_OUT_OF_BOUNDS,
			SPEED_0_100,
			NO_HIGHEST_PRIO_CAM2D
		};

		static enum Errors
		{

		};

		static String get_print(Prints value);
		static String get_warn(Warnings value);
		static String get_err(Errors value);

		static void blend_before_init();
		static void no_cam_seq_found();
		static void idx_out_of_bounds(int idx, int size);
		static void speed_0_100(int percent);
		static void no_highest_prio_cam2d(int amount);
		static void test_warns();
	};
}