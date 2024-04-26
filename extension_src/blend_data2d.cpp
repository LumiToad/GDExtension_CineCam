// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// blend_data2d.cpp

#include "blend_data2d.h"

#include "bind_utils.h"

using namespace godot;

BlendData2D::BlendData2D()
{
	additional_description = "";
	initialize_internal();
}


BlendData2D::~BlendData2D()
{
}


void BlendData2D::_bind_methods()
{
}


void godot::BlendData2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_name(*_get_extension_class_name());
}