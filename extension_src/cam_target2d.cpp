// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target2d.cpp

#include "cam_target2d.h"

#include "godot_cpp/variant/utility_functions.hpp"

#include "bind_utils.h"

using namespace godot;

CamTarget2D::CamTarget2D()
{
	additional_description = "Camera target for CineCam2D follow mode.";
	initialize_internal();

	target_offset = Vector2();
	speed = 0.0;
	ease = Tween::EaseType::EASE_IN_OUT;
	trans = Tween::TransitionType::TRANS_CUBIC;
}


CamTarget2D::~CamTarget2D()
{

}


void CamTarget2D::_bind_methods()
{
	ADD_GETSET_BINDING(get_target_offset, set_target_offset, offset, offset, CamTarget2D, Variant::VECTOR2);
	ADD_GETSET_HINT_BINDING(get_speed, set_speed, speed, speed, CamTarget2D, Variant::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0,0.001,suffix:%");
	ADD_GETSET_HINT_BINDING(get_ease, set_ease, ease, ease, CamTarget2D, Variant::INT, PROPERTY_HINT_ENUM, EASE_HINTS);
	ADD_GETSET_HINT_BINDING(get_trans, set_trans, trans, trans, CamTarget2D, Variant::INT, PROPERTY_HINT_ENUM, TRANS_HINTS);

	ADD_METHOD_BINDING(scaled_speed, CamTarget2D);
}


void CamTarget2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


double CamTarget2D::scaled_speed() const
{
	double ret_val = 0.0;
	if (speed > 0.0)
	{
		ret_val = speed / 100;
	}

	return ret_val;
}



Vector2 CamTarget2D::get_target_offset() const
{
	return target_offset;
}


void CamTarget2D::set_target_offset(Vector2 offset)
{
	target_offset = offset;
}


void CamTarget2D::set_speed(double p_speed)
{
	if (speed < 0.0 || speed > 100.0)
	{
		UtilityFunctions::push_warning("WARNING! Speed must be between 0 % and 100 %, but was: ", p_speed);
		return;
	}
	speed = p_speed;
}


double CamTarget2D::get_speed() const
{
	return speed;
}


void CamTarget2D::set_ease(Tween::EaseType p_ease)
{
	ease = p_ease;
}


Tween::EaseType CamTarget2D::get_ease() const
{
	return ease;
}


void CamTarget2D::set_trans(Tween::TransitionType p_trans)
{
	trans = p_trans;
}


Tween::TransitionType CamTarget2D::get_trans() const
{
	return trans;
}
