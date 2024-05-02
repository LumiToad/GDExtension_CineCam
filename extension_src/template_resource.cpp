// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_resource.cpp

#include "template_resource.h"

#include "bind_utils.h"

using namespace godot;

TemplateResource::TemplateResource()
{
	additional_description = "";
	initialize_internal();
}


TemplateResource::~TemplateResource()
{
}


void TemplateResource::_bind_methods()
{
}


void godot::TemplateResource::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
}
