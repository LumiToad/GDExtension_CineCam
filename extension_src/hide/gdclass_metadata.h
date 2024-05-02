// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// gdclass_metadata.h

#ifndef GDCLASS_METADATA_H
#define GDCLASS_METADATA_H

#include "godot_cpp/classes/node.hpp"

namespace godot
{
	struct GDCLASS_Metadata
	{
		const godot::String HELP_PREFIX = "H_";

		godot::StringName m_parent_name = "";
		godot::String m_additional = "";
		godot::StringName m_type_name = "";
		godot::String m_full_metadata = "";

		GDCLASS_Metadata();
		GDCLASS_Metadata(godot::StringName, godot::StringName);
		GDCLASS_Metadata(godot::StringName, godot::String, godot::StringName);
		~GDCLASS_Metadata();

		void build_metadata_string();
		godot::String get_metadata_string() const;
		void set_metadata_string(const godot::String);
	};
}

#endif // GDCLASS_METADATA_H