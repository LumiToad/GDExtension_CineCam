// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_class.cpp

#include "template_class.h"

#include "bind_utils.h"

using namespace godot;

Template_Class::Template_Class()
{
	initialize_internal();
}


Template_Class::~Template_Class()
{
}


void Template_Class::_bind_methods()
{
}


void godot::Template_Class::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), "Some additional Text.", *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void Template_Class::_process(double delta)
{
}