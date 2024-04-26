// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.cpp

#include "cinecam2d.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

#include "bind_utils.h"

using namespace godot;

CineCam2D::CineCam2D()
{
	is_shake_offset_active = false;
	is_shake_zoom_active = false;
	shake_offset_intensity = 0.0;
	shake_offset_duration = 0.0;
	shake_zoom_intensity = 0.0;
	shake_zoom_duration = 0.0;
	additional_description = "";
	initialize_internal();
}


CineCam2D::~CineCam2D()
{

}


void CineCam2D::_bind_methods()
{
	ADD_GETSET_HINT_BINDING(get_shake_offset_intensity, set_shake_offset_intensity, shake_offset_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_offset_duration, set_shake_offset_duration, shake_offset_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_HINT_BINDING(get_shake_zoom_intensity, set_shake_zoom_intensity, shake_zoom_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_zoom_duration, set_shake_zoom_duration, shake_zoom_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_BINDING(get_default_blend_data, set_default_blend_data, default_blend, p_default_blend, CineCam2D, OBJECT);

	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_zoom, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));

	ADD_METHOD_ARGS_BINDING(blend_to, CineCam2D, VA_LIST("vcam2d", "blend_data"));

	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_ENDED));
}


void CineCam2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CineCam2D::init_tweens()
{
	shake_offset_intensity_tween = get_tree()->create_tween();

	shake_offset_duration_tween = get_tree()->create_tween();
	shake_offset_duration_tween->set_trans(DEFAULT_TRANS);

	shake_zoom_intensity_tween = get_tree()->create_tween();
	
	shake_zoom_duration_tween = get_tree()->create_tween();
	shake_zoom_duration_tween->set_trans(DEFAULT_TRANS);

	blend_position_tween = get_tree()->create_tween();
}


void godot::CineCam2D::blend_to(Node2D* vcam2d, Ref<BlendData2D> blend_data)
{
	VirtualCam2D* vcam_internal = Object::cast_to<VirtualCam2D>(vcam2d);

	blend_position_tween->set_trans(blend_data->get_trans());
	blend_position_tween->set_ease(blend_data->get_ease());

	blend_position_tween->
		tween_method(
			Callable(this, "set_global_position"),
			get_global_position(),
			vcam2d->get_global_position(),
			blend_data->get_duration());
}


void godot::CineCam2D::shake_offset(const double &p_intensity, const double &p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	original_offset = get_offset();

	shake_offset_intensity_tween->set_ease(p_ease);
	shake_offset_intensity_tween->set_trans(p_trans);

	shake_offset_intensity = p_intensity;
	shake_offset_duration = p_duration;

	shake_offset_intensity_tween->play();
	shake_offset_duration_tween->play();

	shake_offset_intensity_tween->
		tween_method(Callable(this, "set_shake_offset_intensity"), shake_offset_intensity, 0.0, p_duration);
	shake_offset_duration_tween
		->tween_method(Callable(this, "set_shake_offset_duration"), shake_offset_duration, 0.0, p_duration);

	is_shake_offset_active = true;
}


void godot::CineCam2D::shake_zoom(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	original_zoom = get_zoom();

	shake_zoom_intensity_tween->set_ease(p_ease);
	shake_zoom_intensity_tween->set_trans(p_trans);

	shake_zoom_intensity = p_intensity;
	shake_zoom_duration = p_duration;
		  
	shake_zoom_intensity_tween->play();
	shake_zoom_duration_tween->play();

	shake_zoom_intensity_tween->
		tween_method(Callable(this, "set_shake_zoom_intensity"), shake_zoom_intensity, 0.0, p_duration);
	shake_zoom_duration_tween
		->tween_method(Callable(this, "set_shake_zoom_duration"), shake_zoom_duration, 0.0, p_duration);

	is_shake_zoom_active = true;
}


void CineCam2D::shake_offset_internal(double delta)
{
	if (!is_shake_offset_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double rng_x = rng.randf_range(-shake_offset_intensity, shake_offset_duration);
	double rng_y = rng.randf_range(-shake_offset_intensity, shake_offset_duration);

	Vector2 shake_vector = original_offset + Vector2(rng_x, rng_y);

	set_offset(shake_vector);

	if (shake_offset_duration <= 0.0)
	{
		is_shake_offset_active = false;
		shake_offset_intensity_tween->stop();
		shake_offset_duration_tween->stop();

		set_offset(original_offset);

		emit_signal(SIGNAL_SHAKE_OFFSET_ENDED);
	}
}


void godot::CineCam2D::shake_zoom_internal(double delta)
{
	if (!is_shake_zoom_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double rng_x = rng.randf_range(0.0, shake_zoom_intensity);
	double rng_y = rng.randf_range(0.0, shake_zoom_intensity);

	Vector2 shake_vector = original_zoom + Vector2(rng_x, rng_y);

	set_zoom(shake_vector);

	if (shake_zoom_duration <= 0.0)
	{
		is_shake_zoom_active = false;
		shake_zoom_intensity_tween->stop();
		shake_zoom_duration_tween->stop();

		set_zoom(original_zoom);

		emit_signal(SIGNAL_SHAKE_ZOOM_ENDED);
	}
}


void CineCam2D::_process(double delta)
{
	shake_offset_internal(delta);
	shake_zoom_internal(delta);
}


void CineCam2D::_ready()
{
	init_tweens();
}


double CineCam2D::get_shake_offset_intensity() const
{
	return shake_offset_intensity;
}


void CineCam2D::set_shake_offset_intensity(const double &intensity)
{
	shake_offset_intensity = intensity;
}


double CineCam2D::get_shake_offset_duration() const
{
	return shake_offset_duration;
}


void CineCam2D::set_shake_offset_duration(const double &duration)
{
	shake_offset_duration = duration;
}


double CineCam2D::get_shake_zoom_intensity() const
{
	return shake_zoom_intensity;
}


void CineCam2D::set_shake_zoom_intensity(const double &intensity)
{
	shake_zoom_intensity = intensity;
}


double CineCam2D::get_shake_zoom_duration() const
{
	return shake_zoom_duration;
}


void CineCam2D::set_shake_zoom_duration(const double &duration)
{
	shake_zoom_duration = duration;
}

Ref<BlendData2D> CineCam2D::get_default_blend_data() const
{
	return default_blend;
}

void CineCam2D::set_default_blend_data(Ref<BlendData2D> blend_data)
{
	default_blend = blend_data;
}
