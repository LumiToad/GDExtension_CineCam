// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// cinecam_editor_start.h

#ifndef CINECAM_EDITOR_START_H
#define CINECAM_EDITOR_START_H
#define TOOLS_ENABLED

#include "godot_cpp/classes/editor_plugin.hpp"

class EditorLog;

namespace godot
{
	class CineCamEditorStart : public EditorPlugin
	{
		GDCLASS(CineCamEditorStart, EditorPlugin)

	private:
		String CINECAM_VERSION = "v1.0";
		String CINECAM_URL = "https://github.com/LumiToad/GDExtension_CineCam/";


	public:
		CineCamEditorStart();
		~CineCamEditorStart();

		void print_welcome();
		void _notification(int p_what);

	protected:
		static void _bind_methods();

	};
}

#endif // CINECAM_EDITOR_START_H