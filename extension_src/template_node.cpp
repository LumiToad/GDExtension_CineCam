// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_node.cpp

#include "template_node.h"

#include "godot_cpp/classes/engine.hpp"

#include "bind_utils.h"

using namespace godot;

TemplateNode::TemplateNode()
{
	additional_description = "";
	initialize_internal();
}


TemplateNode::~TemplateNode()
{
}


void TemplateNode::_bind_methods()
{
}


void godot::TemplateNode::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void TemplateNode::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
	default:
		break;
	case NOTIFICATION_READY:
		_ready_internal(is_in_editor);
		break;
	case NOTIFICATION_PROCESS:
		_process_internal(is_in_editor);
	}
}


void TemplateNode::_ready_internal(bool editor)
{

}


void TemplateNode::_process_internal(bool editor)
{
	double delta = get_process_delta_time();
}