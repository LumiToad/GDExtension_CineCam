// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target2d.cpp

#include "cam_target2d.h"

#include "godot_cpp/classes/engine.hpp"

#include "bind_utils.h"

using namespace godot;

CamTarget2D::CamTarget2D()
{
	additional_description = "";
	initialize_internal();
}


CamTarget2D::~CamTarget2D()
{

}


void CamTarget2D::_bind_methods()
{
	ADD_GETSET_HINT_BINDING(get_blend_data, set_blend_data, blend2d, blend_data, CamTarget2D, OBJECT, godot::PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D");
	ADD_GETSET_BINDING(get_target_offset, set_target_offset, offset, offset, CamTarget2D, Variant::VECTOR2);
}


void CamTarget2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CamTarget2D::_process(double delta)
{
}


void CamTarget2D::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
	default:
		break;
	case NOTIFICATION_READY:
		if (!is_in_editor)
		{
			// _ready()
		}
		break;
	}
}


Ref<BlendData2D> CamTarget2D::get_blend_data() const
{
	return blend_data;
}


void CamTarget2D::set_blend_data(Ref<BlendData2D> data)
{
	blend_data = data;
}


Vector2 CamTarget2D::get_target_offset() const
{
	return target_offset;
}


void CamTarget2D::set_target_offset(Vector2 offset)
{
	target_offset = offset;
}