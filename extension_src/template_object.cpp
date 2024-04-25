// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_object.cpp

#include "template_object.h"

#include "bind_utils.h"

using namespace godot;

TemplateObject::TemplateObject()
{
	additional_description = "";
	initialize_internal();
}


TemplateObject::~TemplateObject()
{
}


void TemplateObject::_bind_methods()
{
}


void godot::TemplateObject::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
}
