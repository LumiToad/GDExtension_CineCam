// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// blend_data3d.cpp

#include "blend_data3d.h"

#include "bind_utils.h"

using namespace godot;

BlendData3D::BlendData3D()
{
	blend_name = "";
	blend_by = BlendByType::DURATION;
	blend_by_value = 0.0;
	ease = Tween::EaseType::EASE_IN;
	trans = Tween::TransitionType::TRANS_LINEAR;
	blend_rotation = true;
	callable_on_start = false;
	callable_on_complete = false;

	additional_description = "";
	initialize_internal();
}


BlendData3D::~BlendData3D()
{
}


void BlendData3D::_bind_methods()
{
	ADD_METHOD_BINDING(get_blend_name, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_blend_name, BlendData3D, "p_name");
	ADD_METHOD_BINDING(get_blend_by, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_blend_by, BlendData3D, "p_by");
	ADD_METHOD_BINDING(get_blend_by_value, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_blend_by_value, BlendData3D, "value");
	ADD_METHOD_BINDING(get_ease, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_ease, BlendData3D, "p_ease");
	ADD_METHOD_BINDING(get_trans, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_trans, BlendData3D, "p_trans");
	ADD_METHOD_BINDING(get_callable, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_callable, BlendData3D, "p_callable");
	ADD_METHOD_BINDING(is_blend_rotation, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_blend_rotation, BlendData3D, "rotation");
	ADD_METHOD_BINDING(get_callable_on_start, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_callable_on_start, BlendData3D, "enable");
	ADD_METHOD_BINDING(get_callable_on_complete, BlendData3D);
	ADD_METHOD_ARGS_BINDING(set_callable_on_complete, BlendData3D, "enable");

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "blend_name"), "set_blend_name", "get_blend_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "blend_by", PROPERTY_HINT_ENUM, BLEND_BY_HINTS), "set_blend_by", "get_blend_by");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "blend_by_value"), "set_blend_by_value", "get_blend_by_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ease", PROPERTY_HINT_ENUM, EASE_HINTS), "set_ease", "get_ease");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trans", PROPERTY_HINT_ENUM, TRANS_HINTS), "set_trans", "get_trans");
	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "callable", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_callable", "get_callable");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blend_rotation"), "set_blend_rotation", "is_blend_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "on_start"), "set_callable_on_start", "get_callable_on_start");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "on_complete"), "set_callable_on_complete", "get_callable_on_complete");

	BIND_ENUM_CONSTANT(SPEED)
		BIND_ENUM_CONSTANT(DURATION)
}


void BlendData3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
}


bool BlendData3D::_is_default_blend() const
{
	if (
		blend_name == "" &&
		blend_by == BlendByType::DURATION &&
		blend_by_value == 0.0 &&
		ease == Tween::EaseType::EASE_IN &&
		trans == Tween::TransitionType::TRANS_LINEAR &&
		blend_rotation == true &&
		callable_on_start == false &&
		callable_on_complete == false
		)
	{
		return true;
	}

	return false;
}


void BlendData3D::set_blend_name(String p_name)
{
	blend_name = p_name;
}


String BlendData3D::get_blend_name() const
{
	return blend_name;
}


void BlendData3D::set_blend_by(BlendData3D::BlendByType by)
{
	blend_by = by;
}


BlendData3D::BlendByType BlendData3D::get_blend_by() const
{
	return blend_by;
}


void BlendData3D::set_blend_by_value(double p_value)
{
	blend_by_value = p_value;
}


double BlendData3D::get_blend_by_value() const
{
	return blend_by_value;
}


void BlendData3D::set_ease(Tween::EaseType p_ease)
{
	ease = p_ease;
}


Tween::EaseType BlendData3D::get_ease() const
{
	return ease;
}


void BlendData3D::set_trans(Tween::TransitionType p_trans)
{
	trans = p_trans;
}


Tween::TransitionType BlendData3D::get_trans() const
{
	return trans;
}


void BlendData3D::set_callable(Callable p_callable)
{
	callable = p_callable;
}


Callable BlendData3D::get_callable() const
{
	return callable;
}


void BlendData3D::set_blend_rotation(bool rotation)
{
	blend_rotation = rotation;
}


bool BlendData3D::is_blend_rotation() const
{
	return blend_rotation;
}


void BlendData3D::set_callable_on_start(bool enable)
{
	callable_on_start = enable;
}


bool BlendData3D::get_callable_on_start() const
{
	return callable_on_start;
}


void BlendData3D::set_callable_on_complete(bool enable)
{
	callable_on_complete = enable;
}


bool BlendData3D::get_callable_on_complete() const
{
	return callable_on_complete;
}
