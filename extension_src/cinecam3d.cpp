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
	shake_offset_intensity = 0.0;
	shake_offset_duration = 0.0;
	shake_fov_intensity = 0.0;
	shake_fov_duration = 0.0;
	shake_rotation_intensity = 0.0;
	shake_rotation_duration = 0.0;
	tweens_ready = false;
	additional_description = "";
	current_sequence = nullptr;
	highest_prio_vcam = nullptr;
	follow_target = nullptr;
	look_at_target = nullptr;

	initialize_internal();
}


CineCam3D::~CineCam3D()
{

}


void CineCam3D::_bind_methods()
{
	ADD_METHOD_BINDING(init_default_blend_data, CineCam3D);

	ADD_METHOD_BINDING(seq_blend_next, CineCam3D);
	ADD_METHOD_BINDING(seq_blend_prev, CineCam3D);
	ADD_METHOD_ARGS_BINDING(seq_blend_to, CineCam3D, VA_LIST("idx"));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence, CineCam3D, "backwards", DEFVAL(false));
	ADD_METHOD_DEFAULTARGS_BINDING(start_sequence_at, CineCam3D, VA_LIST("backwards", "index"), VA_LIST(DEFVAL(0), DEFVAL(false)));
	ADD_METHOD_BINDING(seq_resume, CineCam3D);
	ADD_METHOD_BINDING(seq_pause, CineCam3D);
	ADD_METHOD_BINDING(seq_stop, CineCam3D);


	ADD_METHOD_ARGS_BINDING(reposition_to_vcam, CineCam3D, "vcam");

	ADD_METHOD_BINDING(prioritized_vcam, CineCam3D);
	ADD_METHOD_BINDING(_on_vcam_priority_changed, CineCam3D);
	ADD_METHOD_BINDING(_move_by_priority_mode, CineCam3D);
	ADD_METHOD_BINDING(_move_by_follow_mode, CineCam3D);
	ADD_METHOD_BINDING(blend_resume, CineCam3D);
	ADD_METHOD_BINDING(blend_pause, CineCam3D);
	ADD_METHOD_ARGS_BINDING(_calc_blend_duration_by_speed, CineCam3D, VA_LIST("current_pos", "target_pos", "speed"));
	ADD_METHOD_ARGS_BINDING(find_vcam_by_id, CineCam3D, "id");

	ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam3D, VA_LIST("p_vcam"));

	ADD_GETSET_HINT_BINDING(get_follow_mode, set_follow_mode, follow_mode, mode, CineCam3D, INT, PROPERTY_HINT_ENUM, TARGET_MODE_HINTS);

	ADD_GETSET_HINT_BINDING(_get_blend_data, _set_blend_data, blend_data, blend_data, CineCam3D, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "BlendData3D");
	ADD_GETSET_HINT_BINDING(get_current_sequence, set_current_sequence, current_sequence, p_sequence, CineCam3D, OBJECT, PROPERTY_HINT_NODE_TYPE, "CamSequence3D");
	ADD_GETSET_HINT_BINDING(get_target, set_target, target, p_target, CineCam3D, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE, "CamTarget3D");
	ADD_GETSET_HINT_BINDING(get_look_at_target, set_look_at_target, look_at_target, p_target, CineCam3D, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE, "CamTarget3D");

	ADD_GETSET_HINT_BINDING(get_shake_offset_intensity, set_shake_offset_intensity, shake_offset_intensity, intensity, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_offset_duration, set_shake_offset_duration, shake_offset_duration, duration, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_HINT_BINDING(get_shake_fov_intensity, set_shake_fov_intensity, shake_fov_intensity, intensity, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_fov_duration, set_shake_fov_duration, shake_fov_duration, duration, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_HINT_BINDING(get_shake_rotation_intensity, set_shake_rotation_intensity, shake_rotation_intensity, intensity, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_rotation_duration, set_shake_rotation_duration, shake_rotation_duration, duration, CineCam3D, FLOAT, PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_BINDING(_get_seq_is_paused, _set_seq_is_paused, sequence_paused, paused, CineCam3D, BOOL);
	ADD_GETSET_BINDING(_get_blend_is_paused, _set_blend_is_paused, blend_paused, paused, CineCam3D, BOOL);

	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_fov, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_rotation, CineCam3D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));

	ADD_METHOD_ARGS_BINDING(blend_to, CineCam3D, VA_LIST("vcam2d", "blend_data"));

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
	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITIZED_VCAM3D_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(OFF);
	BIND_ENUM_CONSTANT(PRIO);
	BIND_ENUM_CONSTANT(PRIO_ONESHOT);
	BIND_ENUM_CONSTANT(PRIO_BLEND);
	BIND_ENUM_CONSTANT(TARGET);
	BIND_ENUM_CONSTANT(TARGET_BLEND);

	ClassDB::bind_method(D_METHOD("_on_blend_completed_internal"), &CineCam3D::_on_blend_completed_internal);
}


void CineCam3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CineCam3D::init_tweens()
{
	shake_offset_intensity_tween = get_tree()->create_tween();
	shake_offset_intensity_tween->stop();

	shake_offset_duration_tween = get_tree()->create_tween();
	shake_offset_duration_tween->stop();
	shake_offset_duration_tween->set_trans(DEFAULT_TRANS);

	shake_fov_intensity_tween = get_tree()->create_tween();
	shake_fov_intensity_tween->stop();

	shake_fov_duration_tween = get_tree()->create_tween();
	shake_fov_duration_tween->stop();
	shake_fov_duration_tween->set_trans(DEFAULT_TRANS);

	shake_rotation_intensity_tween = get_tree()->create_tween();
	shake_rotation_intensity_tween->stop();

	shake_rotation_duration_tween = get_tree()->create_tween();
	shake_rotation_duration_tween->stop();
	shake_rotation_duration_tween->set_trans(DEFAULT_TRANS);

	blend_tween = get_tree()->create_tween();
	blend_tween->stop();
	blend_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	follow_tween = get_tree()->create_tween();
	follow_tween->stop();

	look_at_tween = get_tree()->create_tween();
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
	blend_data->_set_callable_on_start(false);
	blend_data->_set_callable_on_complete(false);
}


void CineCam3D::_on_blend_started_internal()
{
	emit_signal(SIGNAL_BLEND_STARTED);

	if (active_blend->_get_callable_on_start())
	{
		active_blend->get_callable().call();
	}
}


void CineCam3D::_on_blend_completed_internal()
{
	emit_signal(SIGNAL_BLEND_COMPLETED);

	if (active_blend->_get_callable_on_complete())
	{
		active_blend->get_callable().call();
	}

	blend_tween = get_tree()->create_tween();
	blend_tween->stop();
	is_blend_not_stopped = false;
	blend_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	cycle_sequence_internal();
}


void CineCam3D::blend_to(VirtualCam3D* p_vcam, Ref<BlendData3D> blend_data)
{
	if (!tweens_ready)
	{
		PrintUtils::blend_before_init(__LINE__, __FILE__);
		return;
	}

	if (blend_tween->is_running())
	{
		blend_tween = get_tree()->create_tween();
		blend_tween->stop();
		is_blend_not_stopped = false;
	}

	blend_tween->set_trans(blend_data->get_trans());
	blend_tween->set_ease(blend_data->get_ease());

	double calc_duration = blend_data->get_blend_by_value();

	if (blend_data->get_blend_by() == BlendData3D::BlendByType::SPEED)
	{
		calc_duration = _calc_blend_duration_by_speed(
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration
		);
	}

	blend_tween->tween_method(
		Callable(this, "set_global_position"),
		get_global_position(),
		p_vcam->get_global_position(),
		calc_duration
	);

	blend_tween->play();
	is_blend_not_stopped = true;
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


void CineCam3D::blend_resume()
{
	_set_blend_is_paused(false);
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
	if (is_sequence_paused && !blend_tween->is_running())
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
	emit_signal(SIGNAL_SEQUENCE_STOPPED);
}


void CineCam3D::reposition_to_vcam(VirtualCam3D* p_vcam)
{
	set_global_position(p_vcam->get_global_position());
}


void CineCam3D::shake_offset(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	set_h_offset(original_offset.x);
	set_v_offset(original_offset.y);

	original_offset.x = get_h_offset();
	original_offset.y = get_v_offset();

	if (shake_offset_duration > 0.0)
	{
		shake_offset_intensity_tween->stop();
		shake_offset_duration_tween->stop();
	}

	shake_offset_intensity_tween->set_ease(p_ease);
	shake_offset_intensity_tween->set_trans(p_trans);

	shake_offset_intensity = p_intensity;
	shake_offset_duration = p_duration;

	shake_offset_intensity_tween->play();
	shake_offset_duration_tween->play();

	shake_offset_intensity_tween->tween_method(
		Callable(this, "set_shake_offset_intensity"),
		shake_offset_intensity,
		0.0,
		p_duration
	);

	shake_offset_duration_tween->tween_method(
		Callable(this, "set_shake_offset_duration"),
		shake_offset_duration,
		0.0,
		p_duration
	);

	is_shake_offset_active = true;
	emit_signal(SIGNAL_SHAKE_OFFSET_STARTED);
	
}


void CineCam3D::shake_fov(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	set_fov(original_fov);

	original_fov = get_fov();

	if (shake_fov_duration > 0.0)
	{
		shake_fov_intensity_tween->stop();
		shake_fov_duration_tween->stop();
	}

	shake_fov_intensity_tween->set_ease(p_ease);
	shake_fov_intensity_tween->set_trans(p_trans);

	shake_fov_intensity = p_intensity;
	shake_fov_duration = p_duration;

	shake_fov_intensity_tween->play();
	shake_fov_duration_tween->play();

	shake_fov_intensity_tween->tween_method(
		Callable(this, "set_shake_fov_intensity"),
		shake_fov_intensity,
		0.0,
		p_duration
	);

	shake_fov_duration_tween->tween_method(
		Callable(this, "set_shake_fov_duration"),
		shake_fov_duration,
		0.0,
		p_duration
	);

	is_shake_fov_active = true;
	emit_signal(SIGNAL_SHAKE_FOV_STARTED);
}


void CineCam3D::shake_rotation(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	set_rotation(original_rotation);

	original_rotation = get_rotation();

	if (shake_rotation_duration > 0.0)
	{
		shake_rotation_intensity_tween->stop();
		shake_rotation_duration_tween->stop();
	}

	shake_rotation_intensity_tween->set_ease(p_ease);
	shake_rotation_intensity_tween->set_trans(p_trans);

	shake_rotation_intensity = p_intensity;
	shake_rotation_duration = p_duration;

	shake_rotation_intensity_tween->play();
	shake_rotation_duration_tween->play();

	shake_rotation_intensity_tween->tween_method(
		Callable(this, "set_shake_rotation_intensity"),
		shake_rotation_intensity,
		0.0,
		p_duration
	);

	shake_rotation_duration_tween->tween_method(
		Callable(this, "set_shake_rotation_duration"),
		shake_rotation_duration,
		0.0,
		p_duration
	);

	is_shake_rotation_active = true;
	emit_signal(SIGNAL_SHAKE_ROTATION_STARTED);
}


void CineCam3D::start_sequence(const bool& backwards)
{
	int idx = 0 + (backwards ? current_sequence->get_vcam3d_array().size() - 1 : 0);
	start_sequence_at(backwards, idx);
}


void CineCam3D::blend_pause()
{
	_set_blend_is_paused(true);
}


void CineCam3D::shake_offset_internal()
{
	if (!is_shake_offset_active) return;

	RandomNumberGenerator rng;
	rng.randomize();
	double rng_x = rng.randf_range(-shake_offset_intensity, shake_offset_duration);
	double rng_y = rng.randf_range(-shake_offset_intensity, shake_offset_duration);

	Vector2 shake_vector = original_offset + Vector2(rng_x, rng_y);

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
	double rng_x = rng.randf_range(-shake_rotation_intensity, shake_rotation_intensity);
	double rng_y = rng.randf_range(-shake_rotation_intensity, shake_rotation_intensity);
	double rng_z = rng.randf_range(-shake_rotation_intensity, shake_rotation_intensity);

	Vector3 shake_vector = original_rotation + Vector3(rng_x, rng_y, rng_z);

	set_rotation(shake_vector);

	if (shake_rotation_duration <= 0.0)
	{
		is_shake_rotation_active = false;
		shake_rotation_intensity_tween->stop();
		shake_rotation_duration_tween->stop();

		set_rotation(original_rotation);

		emit_signal(SIGNAL_SHAKE_ROTATION_ENDED);
	}
}


double CineCam3D::_calc_blend_duration_by_speed(Vector3 current_pos, Vector3 target_pos, double speed)
{
	double distance = sqrt(pow((target_pos.x - current_pos.x), 2.0) + pow((target_pos.y - current_pos.y), 2.0));
	return distance / speed;
}


void CineCam3D::_move_by_follow_mode()
{
	if (!tweens_ready) return;
	if (follow_mode != FollowMode::TARGET_BLEND) return;

	follow_origin = follow_target->get_global_position();

	follow_tween = get_tree()->create_tween();
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


void CineCam3D::_register_vcam_internal(VirtualCam3D* p_vcam)
{
	if (!vcams.has(p_vcam))
	{
		p_vcam->connect(SIGNAL_PRIORITY_CHANGED, Callable(this, "_on_vcam_priority_changed"));
		p_vcam->connect("tree_exiting", Callable(this, "_remove_vcam_internal"));
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
		vcams.erase(p_vcam);
	}
}


bool CineCam3D::_try_set_highest_vcam_internal(VirtualCam3D* p_vcam, int vcam_prio)
{
	bool priority_changed = false;
	int prio = -1;

	if (highest_prio_vcam != nullptr)
	{
		if (vcam_prio > highest_prio_vcam->get_priority())
		{
			highest_prio_vcam = p_vcam;
			priority_changed = true;
			emit_signal(SIGNAL_PRIORITIZED_VCAM3D_CHANGED, p_vcam, vcam_prio);
			return priority_changed;
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

	if (
		follow_mode != FollowMode::OFF ||
		follow_mode != FollowMode::TARGET ||
		follow_mode != FollowMode::TARGET_BLEND
		)
	{
		if (highest_prio_vcam == nullptr)
		{
			PrintUtils::no_highest_prio_cam3d(__LINE__, __FILE__, vcams.size());
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
	case FollowMode::PRIO:
		reposition_to_vcam(highest_prio_vcam);
		break;
	case FollowMode::PRIO_BLEND:
		blend_to(highest_prio_vcam, active_blend);
		break;
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

	if (look_at_target != nullptr)
	{
		Vector3 final_look_at =
			look_at_target->get_global_position() + look_at_target->get_target_offset();
		Vector3 delta_value = final_look_at - origin_for_look_at;
		Vector3 result = look_at_tween->interpolate_value(
			origin_for_look_at,
			delta_value * look_at_target->scaled_speed(),
			1.0,
			1.0,
			look_at_target->get_trans(),
			look_at_target->get_ease()
		);

		look_at(result);
		origin_for_look_at = result;
	}

	switch (follow_mode)
	{
		default:
			break;
		case PRIO:
			reposition_to_vcam(highest_prio_vcam);
			break;
		case OFF:
			break;
		case TARGET:
			if (follow_target == nullptr)
			{
				PrintUtils::no_target3d_found(__LINE__, __FILE__, "OFF", "TARGET");
				follow_mode = FollowMode::OFF;
				break;
			}
			
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
		case NOTIFICATION_READY:
			if (!is_in_editor)
			{
				init_tweens();
				_move_by_priority_mode();
				_move_by_follow_mode();
				camera_origin = get_global_position();
				origin_for_look_at = get_global_rotation();
				original_offset.x = get_h_offset();
				original_offset.y = get_v_offset();
				original_fov = get_fov();
				original_rotation = get_rotation();
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


double CineCam3D::get_shake_offset_intensity() const
{
	return shake_offset_intensity;
}


void CineCam3D::set_shake_offset_intensity(const double& intensity)
{
	shake_offset_intensity = intensity;
}


double CineCam3D::get_shake_offset_duration() const
{
	return shake_offset_duration;
}


void CineCam3D::set_shake_offset_duration(const double& duration)
{
	shake_offset_duration = duration;
}


double CineCam3D::get_shake_fov_intensity() const
{
	return shake_fov_intensity;
}


void CineCam3D::set_shake_fov_intensity(const double& intensity)
{
	shake_fov_intensity = intensity;
}


double CineCam3D::get_shake_fov_duration() const
{
	return shake_fov_duration;
}


void CineCam3D::set_shake_fov_duration(const double& duration)
{
	shake_fov_duration = duration;
}


double CineCam3D::get_shake_rotation_intensity() const
{
	return shake_rotation_intensity;
}


void CineCam3D::set_shake_rotation_intensity(const double& intensity)
{
	shake_rotation_intensity = intensity;
}


double CineCam3D::get_shake_rotation_duration() const
{
	return shake_rotation_duration;
}


void CineCam3D::set_shake_rotation_duration(const double& duration)
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


void CineCam3D::_set_seq_is_paused(bool paused)
{
	is_sequence_paused = paused;
	emit_signal(paused ? SIGNAL_SEQUENCE_PAUSED : SIGNAL_SEQUENCE_RESUMED);
}


bool CineCam3D::_get_seq_is_paused() const
{
	return is_sequence_paused;
}


void CineCam3D::_set_blend_is_paused(bool paused)
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	if (!is_blend_not_stopped) return;

	if (paused)
	{
		blend_tween->pause();
		emit_signal(SIGNAL_BLEND_PAUSED);
	}
	else
	{
		blend_tween->play();
		is_blend_not_stopped = true;
		emit_signal(SIGNAL_BLEND_RESUMED);
	}
}


bool CineCam3D::_get_blend_is_paused() const
{
	if (Engine::get_singleton()->is_editor_hint()) return false;

	return !blend_tween->is_running();
}


void CineCam3D::set_look_at_target(CamTarget3D* p_target)
{
	look_at_target = p_target;
}


CamTarget3D* CineCam3D::get_look_at_target() const
{
	return look_at_target;
}
