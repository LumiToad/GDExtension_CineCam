// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.cpp

#include "cinecam2d.h"

#include "godot_cpp/classes/random_number_generator.hpp"

#include "bind_utils.h"

using namespace godot;

CineCam2D::CineCam2D()
{
	shake_strength = 0.0;
	shake_duration = 0.0;
	additional_description = "";
	initialize();
}

CineCam2D::~CineCam2D()
{
}

void CineCam2D::_bind_methods()
{
	ADD_GETSET_HINT_BINDING(get_shake_strength, set_shake_strength, shake_strength, p_shake_strength, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.0, 0.0, or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_duration, set_shake_duration, shake_duration, p_shake_duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.001, 0.001, or_greater");

	ADD_METHOD_ARGS_BINDING(shake_cam, CineCam2D, VA_LIST("shake_strength", "shake_duration"));

	ADD_SIGNAL(MethodInfo("shake_ended"));
}

void godot::CineCam2D::initialize()
{
	GDCLASS_Metadata meta(get_parent_class_static(), "CineCam2D additional Text.", *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}

void CineCam2D::shake_cam(double p_shake_strength, double p_shake_duration)
{
	shake_strength = p_shake_strength;
	shake_duration = p_shake_duration;
}

void CineCam2D::shake_cam_internal(double delta)
{
	if (shake_duration <= 0.0) return;

	RandomNumberGenerator rng;
	rng.randomize();

	shake_duration -= delta;
	Vector2 shake_vector = Vector2(rng.randf_range(-shake_strength, shake_strength), rng.randf_range(-shake_strength, shake_strength));

	set_global_position(shake_vector);
}

void CineCam2D::_process(double delta)
{
	shake_cam_internal(delta);
}

double CineCam2D::get_shake_strength() const
{
	return shake_strength;
}

void CineCam2D::set_shake_strength(const double p_shake_strength)
{
	shake_strength = p_shake_strength;
}

double CineCam2D::get_shake_duration() const
{
	return shake_duration;
}

void CineCam2D::set_shake_duration(const double p_shake_duration)
{
	shake_duration = p_shake_duration;
}