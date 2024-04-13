// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// gdclass_metadata.cpp

#include "gdclass_metadata.h"

using namespace godot;

GDCLASS_Metadata::GDCLASS_Metadata()
{
	m_parent_name = "";
	m_additional = "";
	m_type_name = "";
}

GDCLASS_Metadata::GDCLASS_Metadata(godot::StringName parent_name, godot::StringName type_name)
{
	m_parent_name = parent_name;
	m_type_name = type_name;

	build_metadata_string();
}

GDCLASS_Metadata::GDCLASS_Metadata(godot::StringName parent_name, godot::String additional, godot::StringName type_name)
{
	m_parent_name = parent_name;
	m_additional = additional;
	m_type_name = type_name;

	build_metadata_string();
}

GDCLASS_Metadata::~GDCLASS_Metadata()
{
}

void GDCLASS_Metadata::build_metadata_string()
{
	godot::String metadata = "";
	if (m_parent_name.length() > 0)
	{
		metadata += "Inherits from " + m_parent_name + ".\n\n";
	}

	if (m_additional.length() > 0)
	{
		metadata += m_additional + "\n";
	}

	if (m_type_name.length() > 0)
	{
		metadata += "See " + HELP_PREFIX + m_type_name + " in editor documentation for help.\n" + "(Press F1 -> type: " + HELP_PREFIX + m_type_name + ")";
	}

	set_metadata_string(metadata);
}

godot::String GDCLASS_Metadata::get_metadata_string() const
{
	return m_full_metadata;
}

void GDCLASS_Metadata::set_metadata_string(const godot::String metadata)
{
	m_full_metadata += metadata;
}

