// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// blend_data2d.cpp

#include "blend_data2d.h"

#include "bind_utils.h"

using namespace godot;

BlendData2D::BlendData2D()
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


BlendData2D::~BlendData2D()
{
}


void BlendData2D::_bind_methods()
{
	ADD_METHOD_BINDING(get_blend_name, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_blend_name, BlendData2D, "p_name");
	ADD_METHOD_BINDING(get_blend_by, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_blend_by, BlendData2D, "p_by");
	ADD_METHOD_BINDING(get_blend_by_value, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_blend_by_value, BlendData2D, "value");
	ADD_METHOD_BINDING(get_ease, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_ease, BlendData2D, "p_ease");
	ADD_METHOD_BINDING(get_trans, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_trans, BlendData2D, "p_trans");
	ADD_METHOD_BINDING(get_callable, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_callable, BlendData2D, "p_callable");
	ADD_METHOD_BINDING(is_blend_rotation, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_blend_rotation, BlendData2D, "rotation");
	ADD_METHOD_BINDING(get_callable_on_start, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_callable_on_start, BlendData2D, "enable");
	ADD_METHOD_BINDING(get_callable_on_complete, BlendData2D);
	ADD_METHOD_ARGS_BINDING(set_callable_on_complete, BlendData2D, "enable");

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


void BlendData2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
}


void BlendData2D::set_blend_name(String p_name)
{
	blend_name = p_name;
}

bool BlendData2D::_is_default_blend() const
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


String BlendData2D::get_blend_name() const
{
	return blend_name;
}


void BlendData2D::set_blend_by(BlendData2D::BlendByType by)
{
	blend_by = by;
}


BlendData2D::BlendByType BlendData2D::get_blend_by() const
{
	return blend_by;
}



void BlendData2D::set_blend_by_value(double p_value)
{
	blend_by_value = p_value;
}


double BlendData2D::get_blend_by_value() const
{
	return blend_by_value;
}


void BlendData2D::set_ease(Tween::EaseType p_ease)
{
	ease = p_ease;
}


Tween::EaseType BlendData2D::get_ease() const
{
	return ease;
}


void BlendData2D::set_trans(Tween::TransitionType p_trans)
{
	trans = p_trans;
}


Tween::TransitionType BlendData2D::get_trans() const
{
	return trans;
}


void BlendData2D::set_callable(Callable p_callable)
{
	callable = p_callable;
}


Callable BlendData2D::get_callable() const
{
	return callable;
}


void BlendData2D::set_blend_rotation(bool rotation)
{
	blend_rotation = rotation;
}


bool BlendData2D::is_blend_rotation() const
{
	return blend_rotation;
}


void BlendData2D::set_callable_on_start(bool enable)
{
	callable_on_start = enable;
}


bool BlendData2D::get_callable_on_start() const
{
	return callable_on_start;
}


void BlendData2D::set_callable_on_complete(bool enable)
{
	callable_on_complete = enable;
}


bool BlendData2D::get_callable_on_complete() const
{
	return callable_on_complete;
}
