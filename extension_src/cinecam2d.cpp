// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.cpp

#include "cinecam2d.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/engine.hpp"

#include "bind_utils.h"

using namespace godot;

CineCam2D::CineCam2D()
{
	set_process_mode(PROCESS_MODE_DISABLED);
	priority_mode = PriorityMode::OFF;
	follow_mode = FollowMode::FOLLOW_OFF;
	is_shake_offset_active = false;
	is_shake_zoom_active = false;
	shake_offset_intensity = 0.0;
	shake_offset_duration = 0.0;
	shake_zoom_intensity = 0.0;
	shake_zoom_duration = 0.0;
	tweens_ready = false;
	additional_description = "";
	current_sequence = nullptr;
	highest_prio_vcam = nullptr;
	follow_target = nullptr;
	initialize_internal();
}


CineCam2D::~CineCam2D()
{

}


void CineCam2D::_bind_methods()
{
	ADD_METHOD_BINDING(init_default_blend_data, CineCam2D);

	ADD_METHOD_BINDING(seq_blend_next, CineCam2D);
	ADD_METHOD_BINDING(seq_blend_prev, CineCam2D);
	ADD_METHOD_ARGS_BINDING(seq_blend_to, CineCam2D, VA_LIST("idx"));
	ADD_METHOD_BINDING(start_sequence, CineCam2D);
	ADD_METHOD_ARGS_BINDING(reposition_to_vcam, CineCam2D, "vcam");

	ADD_METHOD_BINDING(prioritized_vcam, CineCam2D);
	ADD_METHOD_BINDING(_on_vcam_priority_changed, CineCam2D);
	ADD_METHOD_BINDING(_move_by_priority_mode, CineCam2D);
	ADD_METHOD_ARGS_BINDING(_calc_blend_duration_by_speed, CineCam2D, VA_LIST("current_pos", "target_pos", "speed"));
	ADD_METHOD_ARGS_BINDING(find_vcam_by_id, CineCam2D, "id");

	ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam2D, VA_LIST("p_vcam"));

	ADD_GETSET_HINT_BINDING(get_follow_mode, set_follow_mode, follow_mode, mode, CineCam2D, INT, PROPERTY_HINT_ENUM, FOLLOW_MODE_HINTS);
	ADD_GETSET_HINT_BINDING(get_priority_mode, set_priority_mode, priority_mode, mode, CineCam2D, INT, PROPERTY_HINT_ENUM, PRIORITY_MODE_HINTS);

	ADD_GETSET_HINT_BINDING(get_default_blend_data, set_default_blend_data, default_blend, p_default_blend, CineCam2D, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D");
	ADD_GETSET_HINT_BINDING(get_current_sequence, set_current_sequence, current_sequence, p_sequence, CineCam2D, OBJECT, PROPERTY_HINT_NODE_TYPE, "CamSequence2D");
	ADD_GETSET_HINT_BINDING(get_target, set_target, follow_target, p_target, CineCam2D, Variant::OBJECT, PROPERTY_HINT_NODE_TYPE, "CamTarget2D");

	ADD_GETSET_HINT_BINDING(get_shake_offset_intensity, set_shake_offset_intensity, shake_offset_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_offset_duration, set_shake_offset_duration, shake_offset_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_HINT_BINDING(get_shake_zoom_intensity, set_shake_zoom_intensity, shake_zoom_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_zoom_duration, set_shake_zoom_duration, shake_zoom_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_zoom, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));

	ADD_METHOD_ARGS_BINDING(blend_to, CineCam2D, VA_LIST("vcam2d", "blend_data"));

	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_COMPLETED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_COMPLETED));
	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITIZED_VCAM2D_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(OFF);
	BIND_ENUM_CONSTANT(INSTANT);
	BIND_ENUM_CONSTANT(INSTANT_FOLLOW);
	BIND_ENUM_CONSTANT(BLEND);
	BIND_ENUM_CONSTANT(BLEND_FOLLOW);
	
	BIND_ENUM_CONSTANT(FOLLOW_OFF);
	BIND_ENUM_CONSTANT(FOLLOW);
	BIND_ENUM_CONSTANT(FOLLOW_BLEND);

	ClassDB::bind_method(D_METHOD("_on_blend_completed_internal"), &CineCam2D::_on_blend_completed_internal);
}


void CineCam2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());

	init_default_blend_data();
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

	blend_tween = get_tree()->create_tween();
	blend_tween->stop();
	blend_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	follow_tween = get_tree()->create_tween();
	follow_tween->stop();

	tweens_ready = true;
}


void CineCam2D::init_default_blend_data()
{
	if (default_blend.ptr() != nullptr) return;

	default_blend.instantiate();

	default_blend.ptr()->set_blend_name("CineCam2D blend data");
	default_blend.ptr()->set_duration(2.0f);
	default_blend.ptr()->set_speed(2.0f);
	default_blend.ptr()->set_blend_by(BlendData2D::BlendByType::DURATION);
	default_blend.ptr()->set_ease(Tween::EASE_IN_OUT);
	default_blend.ptr()->set_trans(Tween::TRANS_CUBIC);
}


void CineCam2D::_on_blend_completed_internal()
{
	emit_signal(SIGNAL_BLEND_COMPLETED);
	
	blend_tween = get_tree()->create_tween();
	blend_tween->stop();
	blend_tween->connect("finished", Callable(this, "_on_blend_completed_internal"));

	if (current_sequence != nullptr)
	{
		int idx = current_sequence->get_current_idx() + 1;
		if (idx >= current_sequence->get_vcam2d_array().size())
		{
			emit_signal(SIGNAL_SEQUENCE_COMPLETED);
			return;
		}

		seq_blend_next();
	}
}


void CineCam2D::blend_to(VirtualCam2D* p_vcam, Ref<BlendData2D> blend_data)
{
	if (!tweens_ready)
	{
		UtilityFunctions::push_warning("WARNING! Tried blending before tweens initialized!\n This is probably a bug in this GDExtension!");
		return;
	}

	blend_tween = get_tree()->create_tween();
	blend_tween->stop();
	
	blend_tween->set_trans(blend_data->get_trans());
	blend_tween->set_ease(blend_data->get_ease());

	double calc_duration = blend_data->get_duration();

	if (blend_data->get_blend_by() == BlendData2D::BlendByType::SPEED)
	{
		calc_duration = _calc_blend_duration_by_speed(
			get_global_position(),
			p_vcam->get_global_position(),
			blend_data->get_speed()
		);
	}

	blend_tween->tween_method(
			Callable(this, "set_global_position"),
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration
		);	

	emit_signal(SIGNAL_BLEND_STARTED);
	blend_tween->play();
}


void godot::CineCam2D::seq_blend_next()
{
	int idx = current_sequence->get_current_idx() + 1;
	if (idx >= current_sequence->get_vcam2d_array().size()) return;
	
	seq_blend_to(idx);
}


void godot::CineCam2D::seq_blend_prev()
{
	int idx = current_sequence->get_current_idx() - 1;
	if (idx < 0) return;

	seq_blend_to(idx);
}

void godot::CineCam2D::seq_blend_to(int idx)
{
	current_sequence->set_current_idx(idx);

	VirtualCam2D* vcam = current_sequence->current_vcam();
	Ref<BlendData2D> blend = default_blend;

	if (vcam->get_default_blend_data().is_valid())
	{
		blend = vcam->get_default_blend_data();
	}

	blend_to(vcam, blend);
}


void CineCam2D::reposition_to_vcam(VirtualCam2D* p_vcam)
{
	set_global_position(p_vcam->get_global_position());
}


void godot::CineCam2D::shake_offset(const double &p_intensity, const double &p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	set_offset(original_offset);

	original_offset = get_offset();

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


void godot::CineCam2D::shake_zoom(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
	set_zoom(original_zoom);

	original_zoom = get_zoom();

	if (shake_zoom_duration > 0.0)
	{
		shake_zoom_intensity_tween->stop();
		shake_zoom_duration_tween->stop();
	}

	shake_zoom_intensity_tween->set_ease(p_ease);
	shake_zoom_intensity_tween->set_trans(p_trans);

	shake_zoom_intensity = p_intensity;
	shake_zoom_duration = p_duration;
		  
	shake_zoom_intensity_tween->play();
	shake_zoom_duration_tween->play();

	shake_zoom_intensity_tween->tween_method(
		Callable(this, "set_shake_zoom_intensity"),
		shake_zoom_intensity,
		0.0,
		p_duration
	);

	shake_zoom_duration_tween->tween_method(
		Callable(this, "set_shake_zoom_duration"),
		shake_zoom_duration,
		0.0,
		p_duration
	);

	is_shake_zoom_active = true;
	emit_signal(SIGNAL_SHAKE_ZOOM_STARTED);
}


void CineCam2D::start_sequence()
{
	if (current_sequence == nullptr)
	{
		UtilityFunctions::push_warning("WARNING! No CamSequence2D Node found!");
		UtilityFunctions::push_warning("WARNING! Use set_current_sequence method or assign it using the Inspector!");
		return;
	}
	
	seq_blend_to(0);
	emit_signal(SIGNAL_SEQUENCE_STARTED);
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


double CineCam2D::_calc_blend_duration_by_speed(Vector2 current_pos, Vector2 target_pos, double speed)
{
	double distance = sqrt(pow((target_pos.x - current_pos.x), 2.0) + pow((target_pos.y - current_pos.y), 2.0));
	return distance / speed;
}


void CineCam2D::follow_blend_internal(CamTarget2D* p_target)
{
	follow_tween = get_tree()->create_tween();
	follow_tween->stop();

	follow_tween->set_trans(p_target->get_trans());
	follow_tween->set_ease(p_target->get_ease());

	follow_tween->play();
}


void CineCam2D::_register_vcam_internal(VirtualCam2D* p_vcam)
{
	if (!vcams.has(p_vcam))
	{
		p_vcam->connect(SIGNAL_PRIORITY_CHANGED, Callable(this, "_on_vcam_priority_changed"));
		p_vcam->connect("tree_exiting", Callable(this, "_remove_vcam_internal"));
		vcams.push_back(cast_to<VirtualCam2D>(p_vcam));
	}

	if (_try_set_highest_vcam_internal(p_vcam, p_vcam->get_priority()))
	{
		_move_by_priority_mode();
	}
}


void godot::CineCam2D::_remove_vcam_internal(VirtualCam2D* p_vcam)
{
	if (vcams.has(p_vcam))
	{
		vcams.erase(p_vcam);
	}
}


bool CineCam2D::_try_set_highest_vcam_internal(VirtualCam2D* p_vcam, int vcam_prio)
{
	bool priority_changed = false;
	int prio = -1;

	if (highest_prio_vcam != nullptr)
	{
		if (vcam_prio > highest_prio_vcam->get_priority())
		{
			highest_prio_vcam = p_vcam;
			priority_changed = true;
			emit_signal(SIGNAL_PRIORITIZED_VCAM2D_CHANGED, p_vcam, vcam_prio);
			return priority_changed;
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

	if (priority_mode != PriorityMode::OFF && highest_prio_vcam == nullptr)
	{
		UtilityFunctions::push_warning(
			"WARNING! Could not detect highest priority of any VirtualCam2D!\nNumber of found cams: " + vcams.size()
		);
	}

	Ref<BlendData2D> blend = default_blend;
	if (highest_prio_vcam->get_default_blend_data().is_valid())
	{
		blend = highest_prio_vcam->get_default_blend_data();
	}

	switch (priority_mode)
	{
		case PriorityMode::OFF:
			break;
		case PriorityMode::INSTANT:
			reposition_to_vcam(highest_prio_vcam);
			break;
		case PriorityMode::INSTANT_FOLLOW:
			reposition_to_vcam(highest_prio_vcam);
			break;
		case PriorityMode::BLEND:
			blend_to(highest_prio_vcam, blend);
			break;
		case PriorityMode::BLEND_FOLLOW:
			blend_to(highest_prio_vcam, blend);
			follow_blend_internal(follow_target);
			break;
	}
}


void CineCam2D::_process(double delta)
{
	shake_offset_internal(delta);
	shake_zoom_internal(delta);

	if (follow_mode != FollowMode::FOLLOW_OFF && priority_mode != PriorityMode::OFF)
	{
		UtilityFunctions::push_warning
		("WARNING! Can't use follow mode and priority mode simulaneously!\n Priority mode will be used.");
		follow_mode = FollowMode::FOLLOW_OFF;
	}

	if (priority_mode == PriorityMode::INSTANT_FOLLOW)
	{
		reposition_to_vcam(highest_prio_vcam);
	}

	switch (follow_mode)
	{
		default:
			break;
		case FOLLOW_OFF:
			break;
		case FOLLOW:
			if (follow_target == nullptr)
			{
				UtilityFunctions::push_warning("WARNING! No target set! Can't follow! ::573");
				follow_mode = FollowMode::FOLLOW_OFF;
			}
			set_global_position(follow_target->get_global_position() + follow_target->get_target_offset());
			break;
		case FOLLOW_BLEND:
			UtilityFunctions::print(follow_origin);
			Vector2 delta_value = follow_target->get_global_position() - follow_origin;
			Vector2 result = follow_tween->interpolate_value(
				follow_origin,
				delta_value * follow_target->get_scaled_speed(),
				1.0,
				1.0,
				follow_target->get_trans(),
				follow_target->get_ease()
			);

			Vector2 offset = follow_target->get_target_offset();
			set_global_position(result + offset);
			follow_origin = result + offset;
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
				set_process_mode(PROCESS_MODE_INHERIT);
				init_tweens();
				_move_by_priority_mode();
				// _move_by_follow_mode
			}
			break;
	}
}


CineCam2D::PriorityMode CineCam2D::get_priority_mode() const
{
	return priority_mode;
}


void CineCam2D::set_priority_mode(CineCam2D::PriorityMode mode)
{
	priority_mode = mode;
}


CineCam2D::FollowMode CineCam2D::get_follow_mode() const
{
	return follow_mode;
}


void CineCam2D::set_follow_mode(CineCam2D::FollowMode mode)
{
	follow_mode = mode;

	if (Engine::get_singleton()->is_editor_hint()) return;

	if (follow_target == nullptr)
	{
		UtilityFunctions::push_warning("WARNING! No target set! Can't follow! ::632");
		follow_mode = FollowMode::FOLLOW_OFF;
		return;
	}

	if (follow_mode == FollowMode::FOLLOW_BLEND)
	{
		follow_blend_internal(follow_target);
	}
}


CamTarget2D* CineCam2D::get_target() const
{
	return follow_target;
}


void CineCam2D::set_target(CamTarget2D* p_target)
{
	follow_target = p_target;
	follow_origin = follow_target->get_global_position();

	if (Engine::get_singleton()->is_editor_hint()) return;

	if (follow_mode == FollowMode::FOLLOW_BLEND)
	{
		follow_blend_internal(follow_target);
	}
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


VirtualCam2D* godot::CineCam2D::find_vcam_by_id(String id) const
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
