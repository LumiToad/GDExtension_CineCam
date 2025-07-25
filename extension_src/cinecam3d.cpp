// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cinecam3d.cpp

#include "cinecam3d.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/engine.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CineCam3D::CineCam3D()
{
	follow_mode = FollowMode::OFF;
	is_shake_offset_active = false;
	is_shake_fov_active = false;
	is_shake_rotation_active = false;
	shake_offset_intensity = Vector2();
	shake_offset_duration = 0.0;
	shake_fov_intensity = 0.0;
	shake_fov_duration = 0.0;
	shake_rotation_intensity = Vector3();
	shake_rotation_duration = 0.0;
	rotation_shake = Vector3();
	tweens_ready = false;
	additional_description = "";
	current_sequence = nullptr;
	highest_prio_vcam = nullptr;
	follow_target = nullptr;
	look_at_target = nullptr;
	blend_duration = -1.0;

	initialize_internal();
}


CineCam3D::~CineCam3D()
{

}


void CineCam3D::_bind_methods()
{
	ADD_METHOD_BINDING(_on_blend_completed_internal, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_remove_vcam_internal, CineCam3D, "p_vcam");
	ADD_METHOD_BINDING(init_default_blend_data, CineCam3D);
	ADD_METHOD_BINDING(seq_blend_next, CineCam3D);
	ADD_METHOD_BINDING(seq_blend_prev, CineCam3D);
	ADD_METHOD_ARGS_BINDING(seq_blend_to, CineCam3D, VA_LIST("idx"));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence, CineCam3D, "backwards", DEFVAL(false));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence_at, CineCam3D, VA_LIST("index", "backwards"), VA_LIST(DEFVAL(0), DEFVAL(false)));
	ADD_METHOD_BINDING(seq_pause, CineCam3D);
	ADD_METHOD_BINDING(seq_stop, CineCam3D);
	ADD_METHOD_ARGS_BINDING(reposition_to_vcam, CineCam3D, "vcam");
	ADD_METHOD_BINDING(prioritized_vcam, CineCam3D);
	ADD_METHOD_BINDING(_on_vcam_priority_changed, CineCam3D);
	ADD_METHOD_BINDING(_move_by_priority_mode, CineCam3D);
	ADD_METHOD_BINDING(_move_by_follow_mode, CineCam3D);
	ADD_METHOD_BINDING(resume_blend, CineCam3D);
	ADD_METHOD_BINDING(pause_blend, CineCam3D);
	ADD_METHOD_BINDING(toggle_blend, CineCam3D);
	ADD_METHOD_BINDING(pause_follow_target, CineCam3D);
	ADD_METHOD_BINDING(resume_follow_target, CineCam3D);
	ADD_METHOD_BINDING(toggle_follow_target, CineCam3D);
	ADD_METHOD_BINDING(pause_follow_prio, CineCam3D);
	ADD_METHOD_BINDING(resume_follow_prio, CineCam3D);
	ADD_METHOD_BINDING(toggle_follow_prio, CineCam3D);
	ADD_METHOD_ARGS_BINDING(apply_vcam3d_data, CineCam3D, "vcam3d");
	ADD_METHOD_ARGS_BINDING(_calc_blend_duration_by_speed, CineCam3D, VA_LIST("current_pos", "target_pos", "speed"));
	ADD_METHOD_ARGS_BINDING(find_vcam_by_id, CineCam3D, "id");
	ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam3D, VA_LIST("p_vcam"));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_fov, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_rotation, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_ARGS_BINDING(blend_to, CineCam3D, VA_LIST("vcam2d", "blend_data"));
	ADD_METHOD_BINDING(full_blend_duration, CineCam3D);

	ADD_METHOD_BINDING(get_follow_mode, CineCam3D);
	ADD_METHOD_ARGS_BINDING(set_follow_mode, CineCam3D, "mode");
	ADD_METHOD_BINDING(_get_blend_data, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_blend_data, CineCam3D, "blend_data");
	ADD_METHOD_BINDING(get_current_sequence, CineCam3D);
	ADD_METHOD_ARGS_BINDING(set_current_sequence, CineCam3D, "p_sequence");
	ADD_METHOD_BINDING(get_target, CineCam3D);
	ADD_METHOD_ARGS_BINDING(set_target, CineCam3D, "p_target");
	ADD_METHOD_BINDING(get_look_at_target, CineCam3D);
	ADD_METHOD_ARGS_BINDING(set_look_at_target, CineCam3D, "p_target");
	ADD_METHOD_BINDING(_get_shake_offset_intensity, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_offset_intensity, CineCam3D, "intensity");
	ADD_METHOD_BINDING(_get_shake_offset_duration, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_offset_duration, CineCam3D, "duration");
	ADD_METHOD_BINDING(_get_shake_fov_intensity, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_fov_intensity, CineCam3D, "intensity");
	ADD_METHOD_BINDING(_get_shake_fov_duration, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_fov_duration, CineCam3D, "duration");
	ADD_METHOD_BINDING(_get_shake_rotation_intensity, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_rotation_intensity, CineCam3D, "intensity");
	ADD_METHOD_BINDING(_get_shake_rotation_duration, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_shake_rotation_duration, CineCam3D, "duration");
	ADD_METHOD_BINDING(_is_seq_paused, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_seq_paused, CineCam3D, "paused");
	ADD_METHOD_BINDING(_is_blend_paused, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_blend_paused, CineCam3D, "paused");
	ADD_METHOD_BINDING(_is_follow_target_paused, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_follow_target_paused, CineCam3D, "paused");
	ADD_METHOD_BINDING(_is_follow_prio_paused, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_set_follow_prio_paused, CineCam3D, "paused");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "follow_mode", PROPERTY_HINT_ENUM, FOLLOW_MODE_HINTS), "set_follow_mode", "get_follow_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "blend_data", PROPERTY_HINT_RESOURCE_TYPE, "BlendData3D"), "_set_blend_data", "_get_blend_data");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "current_sequence", PROPERTY_HINT_NODE_TYPE, "CamSequence3D"), "set_current_sequence", "get_current_sequence");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::OBJECT, "target", PROPERTY_HINT_NODE_TYPE, "CamTarget3D"), "set_target", "get_target");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::OBJECT, "look_at_target", PROPERTY_HINT_NODE_TYPE, "CamTarget3D"), "set_look_at_target", "get_look_at_target");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "shake_offset_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_offset_intensity", "_get_shake_offset_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shake_offset_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_offset_duration", "_get_shake_offset_duration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shake_fov_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_fov_intensity", "_get_shake_fov_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shake_fov_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_fov_duration", "_get_shake_fov_duration");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "shake_rotation_intensity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_rotation_intensity", "_get_shake_rotation_intensity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shake_rotation_duration", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_shake_rotation_duration", "_get_shake_rotation_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sequence_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_seq_paused", "_is_seq_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blend_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_blend_paused", "_is_blend_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "follow_target_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_follow_target_paused", "_is_follow_target_paused");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "follow_prio_pause", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "_set_follow_prio_paused", "_is_follow_prio_paused");

	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_FOV_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_FOV_ENDED));
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
	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITIZED_VCAM3D_CHANGED, PropertyInfo(Variant::OBJECT, "vcam3d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(OFF);
	BIND_ENUM_CONSTANT(PRIO);
	BIND_ENUM_CONSTANT(PRIO_ONESHOT);
	BIND_ENUM_CONSTANT(PRIO_BLEND);
	BIND_ENUM_CONSTANT(TARGET);
	BIND_ENUM_CONSTANT(TARGET_BLEND);
}


void CineCam3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CineCam3D::init_tweens()
{
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	shake_offset_intensity_tween = scene_tree->create_tween();
	shake_offset_intensity_tween->stop();

	shake_offset_duration_tween = scene_tree->create_tween();
	shake_offset_duration_tween->stop();
	shake_offset_duration_tween->set_trans(DEFAULT_TRANS);

	shake_fov_intensity_tween = scene_tree->create_tween();
	shake_fov_intensity_tween->stop();

	shake_fov_duration_tween = scene_tree->create_tween();
	shake_fov_duration_tween->stop();
	shake_fov_duration_tween->set_trans(DEFAULT_TRANS);

	shake_rotation_intensity_tween = scene_tree->create_tween();
	shake_rotation_intensity_tween->stop();

	shake_rotation_duration_tween = scene_tree->create_tween();
	shake_rotation_duration_tween->stop();
	shake_rotation_duration_tween->set_trans(DEFAULT_TRANS);

	blend_position_tween = scene_tree->create_tween();
	blend_position_tween->stop();
	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	blend_rotation_tween = scene_tree->create_tween();
	blend_rotation_tween->stop();

	follow_tween = scene_tree->create_tween();
	follow_tween->stop();

	look_at_tween = scene_tree->create_tween();
	look_at_tween->stop();

	tweens_ready = true;
}


void CineCam3D::init_default_blend_data()
{
	blend_data->set_blend_name("CineCam3D blend data");
	blend_data->set_blend_by_value(2.0f);
	blend_data->set_blend_by(BlendData3D::BlendByType::DURATION);
	blend_data->set_ease(Tween::EASE_IN_OUT);
	blend_data->set_trans(Tween::TRANS_CUBIC);
	blend_data->set_blend_rotation(true);
	blend_data->set_callable_on_start(false);
	blend_data->set_callable_on_complete(false);
}


void CineCam3D::_on_blend_started_internal()
{
	emit_signal(SIGNAL_BLEND_STARTED);
	is_blend_not_stopped = true;

	if (active_blend->get_callable_on_start())
	{
		active_blend->get_callable().call();
	}
}


void CineCam3D::_on_blend_completed_internal()
{
	emit_signal(SIGNAL_BLEND_COMPLETED);

	if (active_blend->get_callable_on_complete())
	{
		active_blend->get_callable().call();
	}

	origin_for_look_at = get_look_at_direction();
	
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	blend_position_tween = scene_tree->create_tween();
	blend_position_tween->stop();

	blend_rotation_tween = scene_tree->create_tween();
	blend_rotation_tween->stop();

	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	is_blend_not_stopped = false;

	blend_duration = -1.0;

	cycle_sequence_internal();
}


void CineCam3D::blend_to(VirtualCam3D* p_vcam, Ref<BlendData3D> blend)
{
	if (!tweens_ready)
	{
		PrintUtils::blend_before_init(__LINE__, __FILE__);
		return;
	}
	
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	blend_position_tween->pause();
	blend_position_tween = scene_tree->create_tween();
	blend_position_tween->stop();
	blend_position_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));
	is_blend_not_stopped = false;
	
	blend_rotation_tween->pause();
	blend_rotation_tween = scene_tree->create_tween();
	blend_rotation_tween->stop();

	blend_position_tween->set_trans(blend->get_trans());
	blend_position_tween->set_ease(blend->get_ease());

	blend_rotation_tween->set_trans(blend->get_trans());
	blend_rotation_tween->set_ease(blend->get_ease());

	double calc_duration = blend->get_blend_by_value();

	if (blend->get_blend_by() == BlendData3D::BlendByType::SPEED)
	{
		calc_duration = _calc_blend_duration_by_speed(
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration
		);
	}

	if (blend->is_blend_rotation())
	{
		Vector3 rotation = p_vcam->get_global_rotation();
		rotation.rotate(Vector3(0.0, 1.0, 0.0), 180);
		Quaternion final_rotation = Quaternion(rotation);

		blend_rotation_tween->tween_method(
			Callable(this, "set_quaternion"),
			get_transform().basis.get_quaternion(),
			final_rotation,
			calc_duration
		);

		blend_rotation_tween->play();
	}

	blend_position_tween->tween_method(
		Callable(this, "set_global_position"),
		get_global_position(),
		p_vcam->get_global_position(),
		calc_duration
	);


	blend_position_tween->play();
	blend_duration = calc_duration;
	_on_blend_started_internal();
}


void CineCam3D::seq_blend_next()
{
	int idx = current_sequence->get_current_idx() + 1;
	if (idx > current_sequence->get_vcam3d_array().size()) return;

	seq_blend_to(idx);
}


void CineCam3D::seq_blend_prev()
{
	int idx = current_sequence->get_current_idx() - 1;
	if (idx < 0) return;

	seq_blend_to(idx);
}


void CineCam3D::resume_blend()
{
	_set_blend_paused(false);
}


void CineCam3D::toggle_blend()
{
	_set_blend_paused(!_is_blend_paused());
}


void CineCam3D::start_sequence_at(const int& idx, const bool& backwards)
{
	if (current_sequence == nullptr)
	{
		PrintUtils::no_cam3d_seq_found(__LINE__, __FILE__);

		return;
	}

	sequence_backwards = backwards;
	sequence_playmode = true;

	seq_blend_to(idx);
	emit_signal(SIGNAL_SEQUENCE_STARTED);
}


void CineCam3D::seq_blend_to(int idx)
{
	current_sequence->set_current_idx(idx);

	VirtualCam3D* vcam = current_sequence->current_vcam();
	init_active_blend();

	if (vcam->_get_blend_data().is_valid())
	{
		active_blend = vcam->_get_blend_data();
	}

	blend_to(vcam, active_blend);
}


void CineCam3D::seq_resume()
{
	if (is_sequence_paused && !blend_position_tween->is_running())
	{
		is_sequence_paused = false;
		cycle_sequence_internal();
	}
}


void CineCam3D::seq_pause()
{
	is_sequence_paused = true;
}


void CineCam3D::seq_stop()
{
	sequence_playmode = false;
	current_sequence->set_current_idx(0);
	emit_signal(SIGNAL_SEQUENCE_STOPPED);
}


void CineCam3D::pause_follow_target()
{
	_set_follow_target_paused(true);
}


void CineCam3D::resume_follow_target()
{
	_set_follow_target_paused(false);
}


void CineCam3D::toggle_follow_target()
{
	is_follow_target_paused = !is_follow_target_paused;
}


void CineCam3D::pause_follow_prio()
{
	_set_follow_prio_paused(true);
}


void CineCam3D::resume_follow_prio()
{
	_set_follow_prio_paused(false);
}


void CineCam3D::toggle_follow_prio()
{
	_set_follow_prio_paused(!is_follow_prio_paused);
}


void CineCam3D::reposition_to_vcam(VirtualCam3D* p_vcam)
{
	set_global_position(p_vcam->get_global_position());
}


void CineCam3D::shake_offset(const Vector2& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	if (shake_offset_duration > 0.0)
	{
		set_h_offset(original_offset.x);
		set_v_offset(original_offset.y);
	}

	original_offset.x = get_h_offset();
	original_offset.y = get_v_offset();

	shake_offset_intensity_tween = scene_tree->create_tween();
	shake_offset_intensity_tween->stop();

	if (shake_offset_duration > 0.0)
	{
		shake_offset_duration_tween = scene_tree->create_tween();
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


void CineCam3D::shake_fov(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	if (shake_fov_duration > 0.0)
	{
		set_fov(original_fov);
	}

	original_fov = get_fov();

	shake_fov_intensity_tween = scene_tree->create_tween();
	shake_fov_intensity_tween->stop();

	if (shake_fov_duration > 0.0)
	{
		shake_fov_duration_tween = scene_tree->create_tween();
		shake_fov_duration_tween->stop();
	}

	shake_fov_intensity_tween->set_ease(p_ease);
	shake_fov_intensity_tween->set_trans(p_trans);

	shake_fov_intensity = p_intensity;
	shake_fov_duration = p_duration;

	shake_fov_intensity_tween->play();
	shake_fov_duration_tween->play();

	shake_fov_intensity_tween->tween_method(
		Callable(this, "_set_shake_fov_intensity"),
		shake_fov_intensity,
		0.0,
		p_duration
	);

	shake_fov_duration_tween->tween_method(
		Callable(this, "_set_shake_fov_duration"),
		shake_fov_duration,
		0.0,
		p_duration
	);

	is_shake_fov_active = true;
	emit_signal(SIGNAL_SHAKE_FOV_STARTED);
}


void CineCam3D::shake_rotation(const Vector3& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	if (is_blend_not_stopped && active_blend->is_blend_rotation()) return;

	if (shake_rotation_duration > 0.0 && look_at_target == nullptr)
	{
		set_global_rotation(original_rotation);
	}

	original_rotation = get_global_rotation();

	shake_rotation_intensity_tween = scene_tree->create_tween();
	shake_rotation_intensity_tween->stop();

	if (shake_rotation_duration > 0.0)
	{
		shake_rotation_duration_tween = scene_tree->create_tween();
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
		Vector3(0.0, 0.0, 0.0),
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


String CineCam3D::apply_vcam3d_data(VirtualCam3D* p_vcam)
{
	String ret_val = "Applied properties:\n";

	TypedArray<Dictionary> properties = p_vcam->get_property_list();
	for (int i = 0; i < properties.size(); i++)
	{
		Dictionary prop = properties[i];
		StringName prop_name = prop["name"];
		if (prop_name.contains("cam3d_"))
		{
			StringName short_name = prop_name.replace("cam3d_", "");
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


void CineCam3D::start_sequence(const bool& backwards)
{
	int idx = 0 + (backwards ? current_sequence->get_vcam3d_array().size() - 1 : 0);
	start_sequence_at(idx, backwards);
}


void CineCam3D::pause_blend()
{
	_set_blend_paused(true);
}


void CineCam3D::shake_offset_internal()
{
	if (!is_shake_offset_active) return;

	RandomNumberGenerator rng;
	rng.randomize();

	Vector2 shake_vector = original_offset + Vector2(
		rng.randf_range(-shake_offset_intensity.x, shake_offset_intensity.x),
		rng.randf_range(-shake_offset_intensity.y, shake_offset_intensity.y)
	);

	set_h_offset(shake_vector.x);
	set_v_offset(shake_vector.y);

	if (shake_offset_duration <= 0.0)
	{
		is_shake_offset_active = false;
		shake_offset_intensity_tween->stop();
		shake_offset_duration_tween->stop();

		set_h_offset(original_offset.x);
		set_v_offset(original_offset.y);

		emit_signal(SIGNAL_SHAKE_OFFSET_ENDED);
	}
}


void CineCam3D::shake_fov_internal()
{
	if (!is_shake_fov_active) return;

	RandomNumberGenerator rng;
	rng.randomize();

	set_fov(original_fov + rng.randf_range(0.0, shake_fov_intensity));

	if (shake_fov_duration <= 0.0)
	{
		is_shake_fov_active = false;
		shake_fov_intensity_tween->stop();
		shake_fov_duration_tween->stop();

		set_fov(original_fov);

		emit_signal(SIGNAL_SHAKE_FOV_ENDED);
	}
}


void CineCam3D::shake_rotation_internal()
{
	if (!is_shake_rotation_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	Vector3 origin = Vector3(0.0, 0.0, 0.0);
	double divisor = 1;

	if (look_at_target == nullptr)
	{
		origin = original_rotation;
		divisor = 100;
	}

	Vector3 shake_vector = origin + Vector3(
		rng.randf_range(-shake_rotation_intensity.x, shake_rotation_intensity.x) / divisor,
		rng.randf_range(-shake_rotation_intensity.y, shake_rotation_intensity.y) / divisor,
		rng.randf_range(-shake_rotation_intensity.z, shake_rotation_intensity.z) / divisor
	);
		
	rotation_shake = shake_vector;
	if (look_at_target == nullptr)
	{
		set_global_rotation(shake_vector);
	}

	if (shake_rotation_duration <= 0.0)
	{
		is_shake_rotation_active = false;
		shake_rotation_intensity_tween->stop();
		shake_rotation_duration_tween->stop();

		rotation_shake = Vector3(0.0, 0.0, 0.0);
		set_global_rotation(original_rotation);

		emit_signal(SIGNAL_SHAKE_ROTATION_ENDED);
	}
}


double CineCam3D::_calc_blend_duration_by_speed(Vector3 current_pos, Vector3 target_pos, double speed)
{
	double distance = sqrt(
		pow((target_pos.x - current_pos.x), 2.0) +
		pow((target_pos.y - current_pos.y), 2.0) +
		pow((target_pos.z - current_pos.z), 2.0)
	);
	return distance / speed;
}


void CineCam3D::_move_by_follow_mode()
{
	if (!tweens_ready) return;
	
	if (scene_tree == nullptr)
	{
		PrintUtils::scene_tree_nullptr(__LINE__, __FILE__);
		return;
	}

	if (follow_mode != FollowMode::TARGET_BLEND) return;

	if (follow_target == nullptr)
	{
		PrintUtils::no_target3d_found(__LINE__, __FILE__, "OFF", "TARGET_BLEND");
		follow_mode = FollowMode::OFF;
		return;
	}

	follow_origin = follow_target->get_global_position();

	follow_tween = scene_tree->create_tween();
	follow_tween->stop();

	follow_tween->set_trans(follow_target->get_trans());
	follow_tween->set_ease(follow_target->get_ease());
}


void CineCam3D::init_active_blend()
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


void CineCam3D::cycle_sequence_internal()
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
			if (idx >= current_sequence->get_vcam3d_array().size())
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


void CineCam3D::look_at_target_internal()
{
	if (is_blend_not_stopped && active_blend->is_blend_rotation()) return;

	if (look_at_target != nullptr)
	{
		Vector3 final_look_at =
			look_at_target->get_global_position() +
			look_at_target->get_target_offset();
		Vector3 delta_value = final_look_at - origin_for_look_at;
		Vector3 result = look_at_tween->interpolate_value(
			origin_for_look_at,
			delta_value * look_at_target->scaled_speed(),
			1.0,
			1.0,
			look_at_target->get_trans(),
			look_at_target->get_ease()
		);

		look_at(result + rotation_shake);
		origin_for_look_at = result + rotation_shake;
	}
}

Vector3 CineCam3D::get_look_at_direction() const
{
	Transform3D transform = get_global_transform();

	return transform.get_origin() - transform.basis.get_column(2);
}


void CineCam3D::set_quaternion_internal(Quaternion q)
{
	get_global_transform().basis.set_quaternion(q);
}


void CineCam3D::_register_vcam_internal(VirtualCam3D* p_vcam)
{
	if (!vcams.has(p_vcam))
	{
		p_vcam->connect(SIGNAL_PRIORITY_CHANGED, Callable(this, "_on_vcam_priority_changed"));
		vcams.push_back(cast_to<VirtualCam3D>(p_vcam));
	}

	if (_try_set_highest_vcam_internal(p_vcam, p_vcam->get_priority()))
	{
		_move_by_priority_mode();
	}
}


void CineCam3D::_remove_vcam_internal(VirtualCam3D* p_vcam)
{
	if (vcams.has(p_vcam))
	{
		if (highest_prio_vcam == p_vcam)
		{
			highest_prio_vcam = nullptr;
		}

		vcams.erase(p_vcam);
		_try_set_highest_vcam_internal(nullptr, -1);
		if (vcams.size() > 0)
		{
			_move_by_priority_mode();
		}
	}
}


bool CineCam3D::_try_set_highest_vcam_internal(VirtualCam3D* p_vcam, int vcam_prio)
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
				emit_signal(SIGNAL_PRIORITIZED_VCAM3D_CHANGED, p_vcam, vcam_prio);
				return priority_changed;
			}
		}

		prio = highest_prio_vcam->get_priority();
	}

	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam3D* cursor = cast_to<VirtualCam3D>(vcams[i]);

		if (cursor->get_priority() > prio)
		{
			prio = cursor->get_priority();
			highest_prio_vcam = cursor;
			priority_changed = true;
		}
	}

	if (priority_changed)
	{
		emit_signal(SIGNAL_PRIORITIZED_VCAM3D_CHANGED, p_vcam, vcam_prio);
	}

	return priority_changed;
}


void CineCam3D::_on_vcam_priority_changed(VirtualCam3D* p_vcam, int vcam_prio)
{
	if (_try_set_highest_vcam_internal(p_vcam, vcam_prio))
	{
		_move_by_priority_mode();
	}
}


void CineCam3D::_move_by_priority_mode()
{
	if (!tweens_ready) return;

	switch (follow_mode)
	{
	default:
	case FollowMode::OFF:
	case FollowMode::TARGET:
	case FollowMode::TARGET_BLEND:
		return;
		break;
	case FollowMode::PRIO:
	case FollowMode::PRIO_BLEND:
	case FollowMode::PRIO_ONESHOT:
		if (highest_prio_vcam == nullptr)
		{
			PrintUtils::no_highest_prio_cam3d(__LINE__, __FILE__, vcams.size());
			return;
		}
	}

	init_active_blend();
	if (highest_prio_vcam->_get_blend_data().is_valid())
	{
		active_blend = highest_prio_vcam->_get_blend_data();
	}

	if (follow_mode == FollowMode::PRIO_ONESHOT)
	{
		reposition_to_vcam(highest_prio_vcam);
	}

	if (follow_mode == FollowMode::PRIO_BLEND)
	{
		blend_to(highest_prio_vcam, active_blend);
	}
}


void CineCam3D::_process(double delta)
{
	// must be overridden and exist in .cpp for NOTIFICATION_PROCESS to work.
}


void CineCam3D::_process_internal(bool editor)
{
	if (editor) return;

	double delta = get_process_delta_time();

	shake_offset_internal();
	shake_fov_internal();
	shake_rotation_internal();

	look_at_target_internal();

	if (is_blend_not_stopped) return;

	switch (follow_mode)
	{
		default:
			break;
		case OFF:
			break;
		case PRIO:
			if (is_follow_prio_paused) break;
			reposition_to_vcam(highest_prio_vcam);
			break;
		case TARGET:
			if (follow_target == nullptr)
			{
				PrintUtils::no_target3d_found(__LINE__, __FILE__, "OFF", "TARGET");
				follow_mode = FollowMode::OFF;
				break;
			}

			if (is_follow_target_paused) break;

			set_global_position(
				camera_origin + (follow_target->get_global_position() + follow_target->get_target_offset())
			);
			break;
		case TARGET_BLEND:
			if (follow_target == nullptr)
			{
				PrintUtils::no_target3d_found(__LINE__, __FILE__, "OFF", "TARGET_BLEND");
				follow_mode = FollowMode::OFF;
				break;
			}
			
			if (is_follow_target_paused) break;

			Vector3 final_pos =
				camera_origin + (follow_target->get_global_position() + follow_target->get_target_offset());
			Vector3 delta_value = final_pos - follow_origin;
			Vector3 result = follow_tween->interpolate_value(
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


void CineCam3D::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
		default:
			break;
		case NOTIFICATION_ENTER_TREE:
			if (!is_in_editor)
			{
			scene_tree = get_tree();
			}
			break;
		case NOTIFICATION_READY:
			if (!is_in_editor)
			{
				init_tweens();
				set_follow_mode(follow_mode);
				camera_origin = get_global_position();
				origin_for_look_at = get_look_at_direction();
				original_offset.x = get_h_offset();
				original_offset.y = get_v_offset();
				original_fov = get_fov();
				original_rotation = get_global_rotation();
			}
			break;
		case NOTIFICATION_PROCESS:
			_process_internal(is_in_editor);
			break;
	}
}


CineCam3D::FollowMode CineCam3D::get_follow_mode() const
{
	return follow_mode;
}


void CineCam3D::set_follow_mode(CineCam3D::FollowMode mode)
{
	if (is_blend_not_stopped)
	{
		PrintUtils::blend_running_command_ignored(__LINE__, __FILE__, "set_follow_mode");
		return;
	}

	follow_mode = mode;

	_move_by_priority_mode();
	_move_by_follow_mode();
}


CamTarget3D* CineCam3D::get_target() const
{
	return follow_target;
}


void CineCam3D::set_target(CamTarget3D* p_target)
{
	follow_target = p_target;

	if (Engine::get_singleton()->is_editor_hint()) return;
	_move_by_follow_mode();
}


Vector2 CineCam3D::_get_shake_offset_intensity() const
{
	return shake_offset_intensity;
}


void CineCam3D::_set_shake_offset_intensity(const Vector2& intensity)
{
	shake_offset_intensity = intensity;
}


double CineCam3D::_get_shake_offset_duration() const
{
	return shake_offset_duration;
}


void CineCam3D::_set_shake_offset_duration(const double& duration)
{
	shake_offset_duration = duration;
}


double CineCam3D::_get_shake_fov_intensity() const
{
	return shake_fov_intensity;
}


void CineCam3D::_set_shake_fov_intensity(const double& intensity)
{
	shake_fov_intensity = intensity;
}


double CineCam3D::_get_shake_fov_duration() const
{
	return shake_fov_duration;
}


void CineCam3D::_set_shake_fov_duration(const double& duration)
{
	shake_fov_duration = duration;
}


Vector3 CineCam3D::_get_shake_rotation_intensity() const
{
	return shake_rotation_intensity;
}


void CineCam3D::_set_shake_rotation_intensity(const Vector3& intensity)
{
	shake_rotation_intensity = intensity;
}


double CineCam3D::_get_shake_rotation_duration() const
{
	return shake_rotation_duration;
}


void CineCam3D::_set_shake_rotation_duration(const double& duration)
{
	shake_rotation_duration = duration;
}


Ref<BlendData3D> CineCam3D::_get_blend_data() const
{
	return blend_data;
}


void CineCam3D::_set_blend_data(Ref<BlendData3D> blend)
{
	blend_data = blend;
	if (blend.ptr() == nullptr) return;

	if (blend->_is_default_blend())
	{
		init_default_blend_data();
	}
}


CamSequence3D* CineCam3D::get_current_sequence() const
{
	return current_sequence;
}


void CineCam3D::set_current_sequence(CamSequence3D* p_sequence)
{
	current_sequence = p_sequence;
}


VirtualCam3D* CineCam3D::prioritized_vcam() const
{
	return highest_prio_vcam;
}


VirtualCam3D* CineCam3D::find_vcam_by_id(String id) const
{
	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam3D* cursor = cast_to<VirtualCam3D>(vcams[i]);
		if (cursor->get_vcam_id() == id)
		{
			return cursor;
		}
	}
	return nullptr;
}


void CineCam3D::_set_seq_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	is_sequence_paused = paused;
	emit_signal(paused ? SIGNAL_SEQUENCE_PAUSED : SIGNAL_SEQUENCE_RESUMED);
}


bool CineCam3D::_is_seq_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return is_sequence_paused;
}


void CineCam3D::_set_blend_paused(bool paused)
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


bool CineCam3D::_is_blend_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return !blend_position_tween->is_running();
}


void CineCam3D::_set_follow_target_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	is_follow_target_paused = paused;
}


bool CineCam3D::_is_follow_target_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return is_follow_target_paused;
}


void CineCam3D::_set_follow_prio_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	is_follow_prio_paused = paused;

	if (follow_mode == FollowMode::PRIO_BLEND)
	{
		_set_blend_paused(paused);
	}
}


bool CineCam3D::_is_follow_prio_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;
	return is_follow_prio_paused;
}


void CineCam3D::set_look_at_target(CamTarget3D* p_target)
{
	look_at_target = p_target;
	if (tweens_ready)
	{
		origin_for_look_at = get_look_at_direction();
	}
}


CamTarget3D* CineCam3D::get_look_at_target() const
{
	return look_at_target;
}


double CineCam3D::full_blend_duration() const
{
	return blend_duration;
}