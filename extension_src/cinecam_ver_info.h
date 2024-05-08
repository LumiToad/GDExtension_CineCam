// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// cinecam_ver_info.h

#ifndef CINECAM_VER_INFO_H
#define CINECAM_VER_INFO_H
#define TOOLS_ENABLED

#include "godot_cpp/classes/editor_plugin.hpp"

#include "print_utils.h"

namespace godot
{
	class CineCamVerInfo : public EditorPlugin
	{
		GDCLASS(CineCamVerInfo, EditorPlugin)

	public:
		CineCamVerInfo();
		~CineCamVerInfo();

		void print_welcome();
		void _notification(int p_what);

	protected:
		static void _bind_methods();
	};
}

#endif // CINECAM_VER_INFO_H