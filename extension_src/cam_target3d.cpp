// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target3d.cpp

#include "cam_target3d.h"

#include "godot_cpp/variant/utility_functions.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CamTarget3D::CamTarget3D()
{
	additional_description = "Camera target for CineCam3D follow mode.";
	initialize_internal();

	target_offset = Vector3();
	speed_x = 0.0;
	speed_y = 0.0;
	speed_z = 0.0;
	ease = Tween::EaseType::EASE_IN_OUT;
	trans = Tween::TransitionType::TRANS_CUBIC;
}


CamTarget3D::~CamTarget3D()
{

}


void CamTarget3D::_bind_methods()
{
	ADD_GETSET_BINDING(get_target_offset, set_target_offset, offset, offset, CamTarget3D, Variant::VECTOR3);
	ADD_GETSET_HINT_BINDING(get_speed_x, set_speed_x, speed_x, speed_x, CamTarget3D, Variant::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0,0.001,suffix:%");
	ADD_GETSET_HINT_BINDING(get_speed_y, set_speed_y, speed_y, speed_y, CamTarget3D, Variant::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0,0.001,suffix:%");
	ADD_GETSET_HINT_BINDING(get_speed_z, set_speed_z, speed_z, speed_z, CamTarget3D, Variant::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0,0.001,suffix:%");
	ADD_GETSET_HINT_BINDING(get_ease, set_ease, ease, ease, CamTarget3D, Variant::INT, PROPERTY_HINT_ENUM, EASE_HINTS);
	ADD_GETSET_HINT_BINDING(get_trans, set_trans, trans, trans, CamTarget3D, Variant::INT, PROPERTY_HINT_ENUM, TRANS_HINTS);

	ADD_METHOD_BINDING(scaled_speed, CamTarget3D);
}


void CamTarget3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


Vector3 CamTarget3D::scaled_speed() const
{
	Vector3 ret_val = Vector3(0.0, 0.0, 0.0);
	if (speed_x > 0.0)
	{
		ret_val.x = speed_x / 100;
	}
	if (speed_y > 0.0)
	{
		ret_val.y = speed_y / 100;
	}
	if (speed_z > 0.0)
	{
		ret_val.z = speed_z / 100;
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


void CamTarget3D::set_speed_x(double p_speed_x)
{
	if (p_speed_x < 0.0 || p_speed_x > 100.0)
	{
		PrintUtils::speed_0_100(p_speed_x);
		return;
	}
	speed_x = p_speed_x;
}


double CamTarget3D::get_speed_x() const
{
	return speed_x;
}


void CamTarget3D::set_speed_y(double p_speed_y)
{
	if (p_speed_y < 0.0 || p_speed_y > 100.0)
	{
		PrintUtils::speed_0_100(p_speed_y);
		return;
	}
	speed_y = p_speed_y;
}


double CamTarget3D::get_speed_y() const
{
	return speed_y;
}


void CamTarget3D::set_speed_z(double p_speed_z)
{
	if (p_speed_z < 0.0 || p_speed_z > 100.0)
	{
		PrintUtils::speed_0_100(p_speed_z);
		return;
	}
	speed_z = p_speed_z;
}


double CamTarget3D::get_speed_z() const
{
	return speed_z;
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
