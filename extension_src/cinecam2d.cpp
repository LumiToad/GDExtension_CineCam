// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.cpp

#include "cinecam2d.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/engine.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CineCam2D::CineCam2D()
{
	follow_mode = FollowMode::OFF;
	is_shake_offset_active = false;
	is_shake_zoom_active = false;
	is_shake_rotation_active = false;
	shake_offset_intensity = Vector2(0.0, 0.0);
	shake_offset_duration = 0.0;
	shake_zoom_intensity = Vector2(0.0, 0.0);
	shake_zoom_duration = 0.0;
	shake_rotation_intensity = 0.0;
	shake_rotation_duration = 0.0;
	tweens_ready = false;
	additional_description = "";
	current_sequence = nullptr;
	highest_prio_vcam = nullptr;
	follow_target = nullptr;
	blend_duration = -1.0;
	initialize_internal();
}


CineCam2D::~CineCam2D()
{

}


void CineCam2D::_bind_methods()
{
	ADD_METHOD_BINDING(_on_blend_completed_internal, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_remove_vcam_internal, CineCam2D, "p_vcam");
	ADD_METHOD_BINDING(init_default_blend_data, CineCam2D);
	ADD_METHOD_BINDING(seq_blend_next, CineCam2D);
	ADD_METHOD_BINDING(seq_blend_prev, CineCam2D);
	ADD_METHOD_ARGS_BINDING(seq_blend_to, CineCam2D, VA_LIST("idx"));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence, CineCam2D, "backwards", DEFVAL(false));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence_at, CineCam2D, VA_LIST("index", "backwards"), VA_LIST(DEFVAL(0), DEFVAL(false)));
	ADD_METHOD_BINDING(seq_resume, CineCam2D);
	ADD_METHOD_BINDING(seq_pause, CineCam2D);
	ADD_METHOD_BINDING(seq_stop, CineCam2D);
	ADD_METHOD_BINDING(pause_follow_target, CineCam2D);
	ADD_METHOD_BINDING(resume_follow_target, CineCam2D);
	ADD_METHOD_BINDING(toggle_follow_target, CineCam2D);
	ADD_METHOD_BINDING(pause_follow_prio, CineCam2D);
	ADD_METHOD_BINDING(resume_follow_prio, CineCam2D);
	ADD_METHOD_BINDING(toggle_follow_prio, CineCam2D);
	ADD_METHOD_ARGS_BINDING(reposition_to_vcam, CineCam2D, "vcam");
	ADD_METHOD_BINDING(prioritized_vcam, CineCam2D);
	ADD_METHOD_BINDING(_on_vcam_priority_changed, CineCam2D);
	ADD_METHOD_BINDING(_move_by_priority_mode, CineCam2D);
	ADD_METHOD_BINDING(_move_by_follow_mode, CineCam2D);
	ADD_METHOD_BINDING(resume_blend, CineCam2D);
	ADD_METHOD_BINDING(pause_blend, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_calc_blend_duration_by_speed, CineCam2D, VA_LIST("current_pos", "target_pos", "speed"));
	ADD_METHOD_ARGS_BINDING(find_vcam_by_id, CineCam2D, "id");
	ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam2D, VA_LIST("vcam2d"));
	ADD_METHOD_ARGS_BINDING(apply_vcam2d_data, CineCam2D, "vcam2d");
	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_zoom, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_rotation, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_ARGS_BINDING(blend_to, CineCam2D, VA_LIST("vcam2d", "blend_data"));
	ADD_METHOD_BINDING(full_blend_duration, CineCam2D);

	ADD_METHOD_BINDING(get_follow_mode, CineCam2D);
	ADD_METHOD_ARGS_BINDING(set_follow_mode, CineCam2D, "mode");
	ADD_METHOD_BINDING(_get_blend_data, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_blend_data, CineCam2D, "blend_data");
	ADD_METHOD_BINDING(get_current_sequence, CineCam2D);
	ADD_METHOD_ARGS_BINDING(set_current_sequence, CineCam2D, "p_sequence");
	ADD_METHOD_BINDING(get_target, CineCam2D);
	ADD_METHOD_ARGS_BINDING(set_target, CineCam2D, "p_target");
	ADD_METHOD_BINDING(_get_shake_offset_intensity, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_offset_intensity, CineCam2D, "intensity");
	ADD_METHOD_BINDING(_get_shake_offset_duration, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_offset_duration, CineCam2D, "duration");
	ADD_METHOD_BINDING(_get_shake_zoom_intensity, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_zoom_intensity, CineCam2D, "intensity");
	ADD_METHOD_BINDING(_get_shake_zoom_duration, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_zoom_duration, CineCam2D, "duration");
	ADD_METHOD_BINDING(_get_shake_rotation_intensity, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_rotation_intensity, CineCam2D, "intensity");
	ADD_METHOD_BINDING(_get_shake_rotation_duration, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_shake_rotation_duration, CineCam2D, "duration");
	ADD_METHOD_BINDING(_is_seq_paused, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_seq_paused, CineCam2D, "paused");
	ADD_METHOD_BINDING(_is_blend_paused, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_blend_paused, CineCam2D, "paused");
	ADD_METHOD_BINDING(_is_follow_target_paused, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_follow_target_paused, CineCam2D, "paused");
	ADD_METHOD_BINDING(_is_follow_prio_paused, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_set_follow_prio_paused, CineCam2D, "paused");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "follow_mode", PROPERTY_HINT_ENUM, FOLLOW_MODE_HINTS), "set_follow_mode", "get_follow_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "blend_data", PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D"), "_set_blend_data", "_get_blend_data");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_sequence", PROPERTY_HINT_NODE_TYPE, "CamSequence2D"), "set_current_sequence", "get_current_sequence");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::OBJECT, "target", PROPERTY_HINT_NODE_TYPE, "CamTarget2D"), "set_target", "get_target");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_offset_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_offset_intensity", "_get_shake_offset_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shake_offset_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_offset_duration", "_get_shake_offset_duration");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_zoom_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_zoom_intensity", "_get_shake_zoom_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_zoom_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_zoom_duration", "_get_shake_zoom_duration");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_rotation_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_rotation_intensity", "_get_shake_rotation_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_rotation_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_rotation_duration", "_get_shake_rotation_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sequence_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_seq_paused", "_is_seq_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blend_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_blend_paused", "_is_blend_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "follow_target_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_follow_target_paused", "_is_follow_target_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "follow_prio_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_follow_prio_paused", "_is_follow_prio_paused");

	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ROTATION_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ROTATION_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_COMPLETED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_PAUSED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_RESUMED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_COMPLETED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_PAUSED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_RESUMED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_STOPPED));
	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITIZED_VCAM2D_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(OFF);
	BIND_ENUM_CONSTANT(PRIO);
	BIND_ENUM_CONSTANT(PRIO_ONESHOT);
	BIND_ENUM_CONSTANT(PRIO_BLEND);
	BIND_ENUM_CONSTANT(TARGET);
	BIND_ENUM_CONSTANT(TARGET_BLEND);
}


void CineCam2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CineCam2D::init_tweens()
{
	shake_offset_intensity_tween = get_tree()->create_tween();
	shake_offset_intensity_tween->stop();
	
	shake_offset_duration_tween = get_tree()->create_tween();
	shake_offset_duration_tween->stop();
	shake_offset_duration_tween->set_trans(DEFAULT_TRANS);

	shake_zoom_intensity_tween = get_tree()->create_tween();
	shake_zoom_intensity_tween->stop();
	
	shake_zoom_duration_tween = get_tree()->create_tween();
	shake_zoom_duration_tween->stop();
	shake_zoom_duration_tween->set_trans(DEFAULT_TRANS);

	shake_rotation_intensity_tween = get_tree()->create_tween();
	shake_rotation_intensity_tween->stop();

	shake_rotation_duration_tween = get_tree()->create_tween();
	shake_rotation_duration_tween->stop();
	shake_rotation_duration_tween->set_trans(DEFAULT_TRANS);

	blend_position_tween = get_tree()->create_tween();
	blend_position_tween->stop();
	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	blend_rotation_tween = get_tree()->create_tween();
	blend_rotation_tween->stop();

	follow_tween = get_tree()->create_tween();
	follow_tween->stop();

	tweens_ready = true;
}


void CineCam2D::init_default_blend_data()
{	
	blend_data->set_blend_name("CineCam2D blend data");
	blend_data->set_blend_by_value(2.0f);
	blend_data->set_blend_by(BlendData2D::BlendByType::DURATION);
	blend_data->set_ease(Tween::EASE_IN_OUT);
	blend_data->set_trans(Tween::TRANS_CUBIC);
	blend_data->set_blend_rotation(true);
	blend_data->set_callable_on_start(false);
	blend_data->set_callable_on_complete(false);
}


void CineCam2D::_on_blend_started_internal()
{
	is_blend_not_stopped = true;
	emit_signal(SIGNAL_BLEND_STARTED);

	if (active_blend->get_callable_on_start())
	{
		active_blend->get_callable().call();
	}
}


void CineCam2D::_on_blend_completed_internal()
{
	emit_signal(SIGNAL_BLEND_COMPLETED);

	if (active_blend->get_callable_on_complete())
	{
		active_blend->get_callable().call();
	}
	
	blend_position_tween = get_tree()->create_tween();
	blend_position_tween->stop();
	blend_rotation_tween = get_tree()->create_tween();
	blend_rotation_tween->stop();

	is_blend_not_stopped = false;
	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	blend_duration = -1.0;

	cycle_sequence_internal();
}


void CineCam2D::blend_to(VirtualCam2D* p_vcam, Ref<BlendData2D> blend)
{
	if (!tweens_ready)
	{
		PrintUtils::blend_before_init(__LINE__, __FILE__);
		return;
	}

	blend_position_tween->pause();
	blend_position_tween = get_tree()->create_tween();
	blend_position_tween->stop();
	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));
	is_blend_not_stopped = false;
	
	blend_rotation_tween->pause();
	blend_rotation_tween = get_tree()->create_tween();
	blend_rotation_tween->stop();

	blend_position_tween->set_trans(blend->get_trans());
	blend_position_tween->set_ease(blend->get_ease());

	blend_rotation_tween->set_trans(blend->get_trans());
	blend_rotation_tween->set_ease(blend->get_ease());

	double calc_duration = blend->get_blend_by_value();

	if (blend->get_blend_by() == BlendData2D::BlendByType::SPEED)
	{
		calc_duration = _calc_blend_duration_by_speed(
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration
		);
	}

	blend_position_tween->tween_method(
			Callable(this, "set_global_position"),
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration
		);

	if (blend->is_blend_rotation())
	{
		blend_rotation_tween->tween_method(
			Callable(this, "set_rotation_degrees"),
			get_rotation_degrees(),
			p_vcam->get_rotation_degrees(),
			calc_duration
		);

		blend_rotation_tween->play();
	}

	blend_position_tween->play();
	blend_duration = calc_duration;
	_on_blend_started_internal();
}


void CineCam2D::seq_blend_next()
{
	int idx = current_sequence->get_current_idx() + 1;
	if (idx > current_sequence->get_vcam2d_array().size()) return;

	seq_blend_to(idx);
}


void CineCam2D::seq_blend_prev()
{
	int idx = current_sequence->get_current_idx() - 1;
	if (idx < 0) return;

	seq_blend_to(idx);
}


void CineCam2D::resume_blend()
{
	_set_blend_paused(false);
}


void CineCam2D::start_sequence_at(const int &idx, const bool& backwards)
{
	if (current_sequence == nullptr)
	{
		PrintUtils::no_cam2d_seq_found(__LINE__, __FILE__);

		return;
	}

	sequence_backwards = backwards;
	sequence_playmode = true;

	seq_blend_to(idx);
	emit_signal(SIGNAL_SEQUENCE_STARTED);
}


void CineCam2D::seq_blend_to(int idx)
{
	current_sequence->set_current_idx(idx);

	VirtualCam2D* vcam = current_sequence->current_vcam();
	init_active_blend();

	if (vcam->_get_blend_data().is_valid())
	{
		active_blend = vcam->_get_blend_data();
	}

	blend_to(vcam, active_blend);
}


void CineCam2D::seq_resume()
{
	if (is_sequence_paused && !blend_position_tween->is_running())
	{
		is_sequence_paused = false;
		cycle_sequence_internal();
	}
}


void CineCam2D::seq_pause()
{
	is_sequence_paused = true;
}


void CineCam2D::seq_stop()
{
	sequence_playmode = false;
	current_sequence->set_current_idx(0);
	emit_signal(SIGNAL_SEQUENCE_STOPPED);
}


void CineCam2D::pause_follow_target()
{
	_set_follow_target_paused(true);
}


void CineCam2D::resume_follow_target()
{
	_set_follow_target_paused(false);
}


void CineCam2D::toggle_follow_target()
{
	is_follow_target_paused = !is_follow_target_paused;
}


void CineCam2D::pause_follow_prio()
{
	_set_follow_prio_paused(true);
}


void CineCam2D::resume_follow_prio()
{
	_set_follow_prio_paused(false);
}


void CineCam2D::toggle_follow_prio()
{
	_set_follow_prio_paused(!is_follow_prio_paused);
}



void CineCam2D::reposition_to_vcam(VirtualCam2D* p_vcam)
{
	set_global_position(p_vcam->get_global_position());
}


void CineCam2D::shake_offset(const Vector2 &p_intensity, const double &p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (shake_offset_duration > 0.0)
	{
		set_offset(original_offset);
	}

	original_offset = get_offset();

	shake_offset_intensity_tween = get_tree()->create_tween();
	shake_offset_intensity_tween->stop();

	if (shake_offset_duration > 0.0)
	{
		shake_offset_duration_tween = get_tree()->create_tween();
		shake_offset_duration_tween->stop();
	}

	shake_offset_intensity_tween->set_ease(p_ease);
	shake_offset_intensity_tween->set_trans(p_trans);

	shake_offset_intensity = p_intensity;
	shake_offset_duration = p_duration;

	shake_offset_intensity_tween->play();
	shake_offset_duration_tween->play();

	shake_offset_intensity_tween->tween_method(
		Callable(this, "_set_shake_offset_intensity"),
		shake_offset_intensity,
		Vector2(0.0, 0.0),
		p_duration
	);

	shake_offset_duration_tween->tween_method(
		Callable(this, "_set_shake_offset_duration"),
		shake_offset_duration,
		0.0,
		p_duration
	);

	is_shake_offset_active = true;
	emit_signal(SIGNAL_SHAKE_OFFSET_STARTED);
}


void CineCam2D::shake_zoom(const Vector2& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (shake_zoom_duration > 0.0)
	{
		set_zoom(original_zoom);
	}

	original_zoom = get_zoom();

	shake_zoom_intensity_tween = get_tree()->create_tween();
	shake_zoom_intensity_tween->stop();

	if (shake_zoom_duration > 0.0)
	{
		shake_zoom_duration_tween = get_tree()->create_tween();
		shake_zoom_duration_tween->stop();
	}

	shake_zoom_intensity_tween->set_ease(p_ease);
	shake_zoom_intensity_tween->set_trans(p_trans);

	shake_zoom_intensity = p_intensity;
	shake_zoom_duration = p_duration;
		  
	shake_zoom_intensity_tween->play();
	shake_zoom_duration_tween->play();

	shake_zoom_intensity_tween->tween_method(
		Callable(this, "_set_shake_zoom_intensity"),
		shake_zoom_intensity,
		Vector2(0.0, 0.0),
		p_duration
	);

	shake_zoom_duration_tween->tween_method(
		Callable(this, "_set_shake_zoom_duration"),
		shake_zoom_duration,
		0.0,
		p_duration
	);

	is_shake_zoom_active = true;
	emit_signal(SIGNAL_SHAKE_ZOOM_STARTED);
}


void CineCam2D::shake_rotation(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (is_ignoring_rotation())
	{
		PrintUtils::ignoring_rotation(__LINE__, __FILE__);
	}

	if (shake_rotation_duration > 0.0)
	{
		set_rotation_degrees(original_rotation);
	}

	original_rotation = get_rotation_degrees();

	shake_rotation_intensity_tween = get_tree()->create_tween();
	shake_rotation_intensity_tween->stop();

	if (shake_rotation_duration > 0.0)
	{
		shake_rotation_duration_tween = get_tree()->create_tween();
		shake_rotation_duration_tween->stop();
	}

	shake_rotation_intensity_tween->set_ease(p_ease);
	shake_rotation_intensity_tween->set_trans(p_trans);

	shake_rotation_intensity = p_intensity;
	shake_rotation_duration = p_duration;

	shake_rotation_intensity_tween->play();
	shake_rotation_duration_tween->play();

	shake_rotation_intensity_tween->tween_method(
		Callable(this, "_set_shake_rotation_intensity"),
		shake_rotation_intensity,
		0.0,
		p_duration
	);

	shake_rotation_duration_tween->tween_method(
		Callable(this, "_set_shake_rotation_duration"),
		shake_rotation_duration,
		0.0,
		p_duration
	);

	is_shake_rotation_active = true;
	emit_signal(SIGNAL_SHAKE_ROTATION_STARTED);
}


void CineCam2D::start_sequence(const bool& backwards)
{
	int idx = 0 + (backwards ? current_sequence->get_vcam2d_array().size() - 1 : 0);
	start_sequence_at(idx, backwards);
}


void CineCam2D::pause_blend()
{
	_set_blend_paused(true);
}


void CineCam2D::shake_offset_internal()
{
	if (!is_shake_offset_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double rng_x = rng.randf_range(-shake_offset_intensity.x, shake_offset_intensity.x);
	double rng_y = rng.randf_range(-shake_offset_intensity.y, shake_offset_intensity.y);

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


void CineCam2D::shake_zoom_internal()
{
	if (!is_shake_zoom_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double rng_x = rng.randf_range(0.0, shake_zoom_intensity.x);
	double rng_y = rng.randf_range(0.0, shake_zoom_intensity.y);

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


void CineCam2D::shake_rotation_internal()
{
	if (!is_shake_rotation_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double shake_amount = 
		original_rotation + rng.randf_range(-shake_rotation_intensity, shake_rotation_intensity);
	shake_amount = Math::clamp(shake_amount, -360.0, 360.0);

	set_rotation_degrees(shake_amount);

	if (shake_rotation_duration <= 0.0)
	{
		is_shake_rotation_active = false;
		shake_rotation_intensity_tween->stop();
		shake_rotation_duration_tween->stop();

		set_rotation_degrees(original_rotation);

		emit_signal(SIGNAL_SHAKE_ROTATION_ENDED);
	}
}


double CineCam2D::_calc_blend_duration_by_speed(Vector2 current_pos, Vector2 target_pos, double speed)
{
	double distance = sqrt(pow((target_pos.x - current_pos.x), 2.0) + pow((target_pos.y - current_pos.y), 2.0));
	return distance / speed;
}


void CineCam2D::_move_by_follow_mode()
{
	if (!tweens_ready) return;
	if (follow_mode != FollowMode::TARGET_BLEND) return;

	if (follow_target == nullptr)
	{
		PrintUtils::no_target2d_found(__LINE__, __FILE__, "OFF", "TARGET_BLEND");
		follow_mode = FollowMode::OFF;
		return;
	}

	follow_origin = follow_target->get_global_position();

	follow_tween = get_tree()->create_tween();
	follow_tween->stop();

	follow_tween->set_trans(follow_target->get_trans());
	follow_tween->set_ease(follow_target->get_ease());
}


void CineCam2D::init_active_blend()
{
	
	if (blend_data.ptr() != nullptr)
	{
		active_blend = blend_data;
	}
	else
	{
		active_blend.instantiate();
	}
}


void CineCam2D::cycle_sequence_internal()
{
	if (
		current_sequence != nullptr &&
		!is_sequence_paused &&
		sequence_playmode
		)
	{
		if (!sequence_backwards)
		{
			int idx = current_sequence->get_current_idx() + 1;
			if (idx >= current_sequence->get_vcam2d_array().size())
			{
				emit_signal(SIGNAL_SEQUENCE_COMPLETED);
				return;
			}
			seq_blend_next();
		}
		else
		{
			int idx = current_sequence->get_current_idx() - 1;
			if (idx < 0)
			{
				emit_signal(SIGNAL_SEQUENCE_COMPLETED);
				return;
			}
			seq_blend_prev();
		}
	}
}


void CineCam2D::_register_vcam_internal(VirtualCam2D* p_vcam)
{
	if (!vcams.has(p_vcam))
	{
		p_vcam->connect(SIGNAL_PRIORITY_CHANGED, Callable(this, "_on_vcam_priority_changed"));
		vcams.push_back(cast_to<VirtualCam2D>(p_vcam));
	}

	if (_try_set_highest_vcam_internal(p_vcam, p_vcam->get_priority()))
	{
		_move_by_priority_mode();
	}
}


void CineCam2D::_remove_vcam_internal(VirtualCam2D* p_vcam)
{
	if (vcams.has(p_vcam))
	{
		if (highest_prio_vcam == p_vcam)
		{
			highest_prio_vcam = nullptr;
		}

		vcams.erase(p_vcam);
		_try_set_highest_vcam_internal(nullptr, -1);	
		_move_by_priority_mode();
	}
}


bool CineCam2D::_try_set_highest_vcam_internal(VirtualCam2D* p_vcam, int vcam_prio)
{
	bool priority_changed = false;
	int prio = -1;

	if (highest_prio_vcam != nullptr)
	{
		if (highest_prio_vcam != p_vcam)
		{
			if (vcam_prio >= highest_prio_vcam->get_priority())
			{
				highest_prio_vcam = p_vcam;
				priority_changed = true;
				emit_signal(SIGNAL_PRIORITIZED_VCAM2D_CHANGED, p_vcam, vcam_prio);
				return priority_changed;
			}
		}

		prio = highest_prio_vcam->get_priority();
	}
	
	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam2D* cursor = cast_to<VirtualCam2D>(vcams[i]);

		if (cursor->get_priority() > prio)
		{
			prio = cursor->get_priority();
			highest_prio_vcam = cursor;
			priority_changed = true;
		}
	}

	if (priority_changed)
	{
		emit_signal(SIGNAL_PRIORITIZED_VCAM2D_CHANGED, p_vcam, vcam_prio);
	}

	return priority_changed;
}


void CineCam2D::_on_vcam_priority_changed(VirtualCam2D* p_vcam, int vcam_prio)
{
	if (_try_set_highest_vcam_internal(p_vcam, vcam_prio))
	{
		_move_by_priority_mode();
	}
}


void CineCam2D::_move_by_priority_mode()
{
	if (!tweens_ready) return;

	if (
		follow_mode == FollowMode::PRIO ||
		follow_mode == FollowMode::PRIO_BLEND ||
		follow_mode == FollowMode::PRIO_ONESHOT
		)
	{
		if (highest_prio_vcam == nullptr)
		{
			PrintUtils::no_highest_prio_cam2d(__LINE__, __FILE__, vcams.size());
		}
	}

	init_active_blend();
	if (highest_prio_vcam->_get_blend_data().is_valid())
	{
		active_blend = highest_prio_vcam->_get_blend_data();
	}

	switch (follow_mode)
	{
		case FollowMode::OFF:
			break;
		case FollowMode::PRIO_ONESHOT:
			reposition_to_vcam(highest_prio_vcam);
			break;
		case FollowMode::PRIO_BLEND:
			blend_to(highest_prio_vcam, active_blend);
			break;
	}
}


void CineCam2D::_process(double delta)
{
	// must be overridden and exist in .cpp for NOTIFICATION_PROCESS to work.
}


void CineCam2D::_process_internal(bool editor)
{
	if (editor) return;

	double delta = get_process_delta_time();

	shake_offset_internal();
	shake_zoom_internal();
	shake_rotation_internal();

	switch (follow_mode)
	{
		default:
			break;
		case PRIO:
			if (is_follow_prio_paused) break;
			reposition_to_vcam(highest_prio_vcam);
			break;
		case OFF:
			break;
		case TARGET:
			if (follow_target == nullptr)
			{
				PrintUtils::no_target2d_found(__LINE__, __FILE__, "OFF", "TARGET");
				follow_mode = FollowMode::OFF;
				break;
			}

			if (is_follow_target_paused) break;

			set_global_position(follow_target->get_global_position() + follow_target->get_target_offset());
			break;
		case TARGET_BLEND:
			if (follow_target == nullptr)
			{
				PrintUtils::no_target2d_found(__LINE__, __FILE__, "OFF", "TARGET_BLEND");
				follow_mode = FollowMode::OFF;
				break;
			}

			if (is_follow_target_paused) break;

			Vector2 offset = follow_target->get_target_offset();
			Vector2 delta_value = (follow_target->get_global_position() + offset) - follow_origin;
			Vector2 result = follow_tween->interpolate_value(
				follow_origin,
				delta_value * follow_target->scaled_speed(),
				1.0,
				1.0,
				follow_target->get_trans(),
				follow_target->get_ease()
			);

			set_global_position(result);
			follow_origin = result;
			break;
	}
}


void CineCam2D::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
		default:
			break;
		case NOTIFICATION_READY:
			if (!is_in_editor)
			{
				init_tweens();
				_move_by_priority_mode();
				_move_by_follow_mode();
				original_offset = get_offset();
				original_zoom = get_zoom();
				original_rotation = get_rotation_degrees();
			}
			break;
		case NOTIFICATION_PROCESS:
			_process_internal(is_in_editor);
	}
}


CineCam2D::FollowMode CineCam2D::get_follow_mode() const
{
	return follow_mode;
}


void CineCam2D::set_follow_mode(CineCam2D::FollowMode mode)
{
	follow_mode = mode;

	_move_by_priority_mode();
	_move_by_follow_mode();
}


CamTarget2D* CineCam2D::get_target() const
{
	return follow_target;
}


void CineCam2D::set_target(CamTarget2D* p_target)
{
	follow_target = p_target;

	if (Engine::get_singleton()->is_editor_hint()) return;
	_move_by_follow_mode();
}


Vector2 CineCam2D::_get_shake_offset_intensity() const
{
	return shake_offset_intensity;
}


void CineCam2D::_set_shake_offset_intensity(const Vector2 &intensity)
{
	shake_offset_intensity = intensity;
}


double CineCam2D::_get_shake_offset_duration() const
{
	return shake_offset_duration;
}


void CineCam2D::_set_shake_offset_duration(const double &duration)
{
	shake_offset_duration = duration;
}


Vector2 CineCam2D::_get_shake_zoom_intensity() const
{
	return shake_zoom_intensity;
}


String CineCam2D::apply_vcam2d_data(VirtualCam2D* p_vcam)
{
	String ret_val = "Applied properties:\n";

	TypedArray<Dictionary> properties = p_vcam->get_property_list();
	for (int i = 0; i < properties.size(); i++)
	{
		Dictionary prop = properties[i];
		StringName prop_name = prop["name"];
		if (prop_name.contains("cam2d_"))
		{
			StringName short_name = prop_name.replace("cam2d_", "");
			Variant prop_from_cam = p_vcam->get(prop_name);
			if (get(short_name) != prop_from_cam)
			{
				ret_val += short_name;
				ret_val += " from: ";
				ret_val += get(short_name);
				ret_val += " to: ";
				ret_val += prop_from_cam;
				ret_val += "\n";
				set(short_name, prop_from_cam);
			}
		}
	}

	return ret_val;
}


void CineCam2D::_set_shake_zoom_intensity(const Vector2 &intensity)
{
	shake_zoom_intensity = intensity;
}


double CineCam2D::_get_shake_zoom_duration() const
{
	return shake_zoom_duration;
}


void CineCam2D::_set_shake_zoom_duration(const double &duration)
{
	shake_zoom_duration = duration;
}


double CineCam2D::_get_shake_rotation_intensity() const
{
	return shake_rotation_intensity;
}


void CineCam2D::_set_shake_rotation_intensity(const double& intensity)
{
	shake_rotation_intensity = intensity;
}


double CineCam2D::_get_shake_rotation_duration() const
{
	return shake_rotation_duration;
}


void CineCam2D::_set_shake_rotation_duration(const double& duration)
{
	shake_rotation_duration = duration;
}


Ref<BlendData2D> CineCam2D::_get_blend_data() const
{
	return blend_data;
}


void CineCam2D::_set_blend_data(Ref<BlendData2D> blend)
{
	blend_data = blend;
	if (blend.ptr() == nullptr) return;

	if (blend->_is_default_blend())
	{
		init_default_blend_data();
	}
}


CamSequence2D* CineCam2D::get_current_sequence() const
{
	return current_sequence;
}


void CineCam2D::set_current_sequence(CamSequence2D* p_sequence)
{
	current_sequence = p_sequence;
}


VirtualCam2D* CineCam2D::prioritized_vcam() const
{
	return highest_prio_vcam;
}


VirtualCam2D* CineCam2D::find_vcam_by_id(String id) const
{
	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam2D* cursor = cast_to<VirtualCam2D>(vcams[i]);
		if (cursor->get_vcam_id() == id)
		{
			return cursor;
		}
	}
	return nullptr;
}


void CineCam2D::_set_seq_paused(bool paused)
{
	is_sequence_paused = paused;
	emit_signal(paused ? SIGNAL_SEQUENCE_PAUSED : SIGNAL_SEQUENCE_RESUMED);
}


bool CineCam2D::_is_seq_paused() const
{
	return is_sequence_paused;
}


void CineCam2D::_set_blend_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	if (!is_blend_not_stopped) return;

	if (paused)
	{
		blend_position_tween->pause();
		blend_rotation_tween->pause();
		emit_signal(SIGNAL_BLEND_PAUSED);
	}
	else
	{
		blend_position_tween->play();
		blend_rotation_tween->play();
		is_blend_not_stopped = true;
		emit_signal(SIGNAL_BLEND_RESUMED);
	}
}


bool CineCam2D::_is_blend_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;

	return !blend_position_tween->is_running();
}


void CineCam2D::_set_follow_target_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	is_follow_target_paused = paused;
}


bool CineCam2D::_is_follow_target_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return is_follow_target_paused;
}


void CineCam2D::_set_follow_prio_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	is_follow_prio_paused = paused;

	if (follow_mode == FollowMode::PRIO_BLEND)
	{
		_set_blend_paused(paused);
	}
}


bool CineCam2D::_is_follow_prio_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return is_follow_prio_paused;
}

double CineCam2D::full_blend_duration() const
{
	return blend_duration;
}
