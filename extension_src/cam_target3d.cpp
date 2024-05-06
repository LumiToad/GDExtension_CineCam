// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target3d.cpp

#include "cam_target3d.h"

#include "godot_cpp/variant/utility_functions.hpp"

#include "bind_utils.h"

using namespace godot;

CamTarget3D::CamTarget3D()
{
	additional_description = "Camera target for CineCam3D follow mode.";
	initialize_internal();

	target_offset = Vector3();
	speed = 0.0;
	ease = Tween::EaseType::EASE_IN_OUT;
	trans = Tween::TransitionType::TRANS_CUBIC;
}


CamTarget3D::~CamTarget3D()
{

}


void CamTarget3D::_bind_methods()
{
	ADD_GETSET_BINDING(get_target_offset, set_target_offset, offset, offset, CamTarget3D, Variant::VECTOR2);
	ADD_GETSET_HINT_BINDING(get_speed, set_speed, speed, speed, CamTarget3D, Variant::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0,0.001,suffix:%");
	ADD_GETSET_HINT_BINDING(get_ease, set_ease, ease, ease, CamTarget3D, Variant::INT, PROPERTY_HINT_ENUM, EASE_HINTS);
	ADD_GETSET_HINT_BINDING(get_trans, set_trans, trans, trans, CamTarget3D, Variant::INT, PROPERTY_HINT_ENUM, TRANS_HINTS);

	ADD_METHOD_BINDING(scaled_speed, CamTarget3D);
}


void CamTarget3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


double CamTarget3D::scaled_speed() const
{
	double ret_val = 0.0;
	if (speed > 0.0)
	{
		ret_val = speed / 100;
	}

	return ret_val;
}



Vector3 CamTarget3D::get_target_offset() const
{
	return target_offset;
}


void CamTarget3D::set_target_offset(Vector3 offset)
{
	target_offset = offset;
}


void CamTarget3D::set_speed(double p_speed)
{
	if (speed < 0.0 || speed > 100.0)
	{
		UtilityFunctions::push_warning("WARNING! Speed must be between 0 % and 100 %, but was: ", p_speed);
		return;
	}
	speed = p_speed;
}


double CamTarget3D::get_speed() const
{
	return speed;
}


void CamTarget3D::set_ease(Tween::EaseType p_ease)
{
	ease = p_ease;
}


Tween::EaseType CamTarget3D::get_ease() const
{
	return ease;
}


void CamTarget3D::set_trans(Tween::TransitionType p_trans)
{
	trans = p_trans;
}


Tween::TransitionType CamTarget3D::get_trans() const
{
	return trans;
}
