// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.cpp

#include "cinecam2d.h"

#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

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
	current_sequence = nullptr;
	highest_prio_vcam = nullptr;
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
	ADD_METHOD_ARGS_BINDING(reposition_to, CineCam2D, "vcam");

	ADD_METHOD_BINDING(prioritized_vcam, CineCam2D);
	ADD_METHOD_BINDING(_on_vcam_priority_changed, CineCam2D);
	ADD_METHOD_ARGS_BINDING(find_vcam_by_id, CineCam2D, "id");

	ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam2D, VA_LIST("p_vcam"));

	ADD_GETSET_HINT_BINDING(get_default_blend_data, set_default_blend_data, default_blend, p_default_blend, CineCam2D, OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D");
	ADD_GETSET_HINT_BINDING(get_current_sequence, set_current_sequence, current_sequence, p_sequence, CineCam2D, OBJECT, PROPERTY_HINT_NODE_TYPE, "CamSequence2D");
	ADD_GETSET_HINT_BINDING(get_priority_mode, set_priority_mode, priority_mode, mode, CineCam2D, INT, PROPERTY_HINT_ENUM, PRIORITY_MODE_HINTS);

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

	BIND_ENUM_CONSTANT(PriorityMode::OFF);
	BIND_ENUM_CONSTANT(PriorityMode::INSTANT);
	BIND_ENUM_CONSTANT(PriorityMode::INSTANT_FOLLOW);
	BIND_ENUM_CONSTANT(PriorityMode::BLEND);
	BIND_ENUM_CONSTANT(PriorityMode::BLEND_FOLLOW);

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
	blend_tween->set_trans(blend_data->get_trans());
	blend_tween->set_ease(blend_data->get_ease());

	double calc_duration = blend_data->get_duration();

	if (blend_data->get_blend_by() == BlendData2D::BlendByType::SPEED)
	{
		Vector2 current = get_global_position();
		Vector2 target = p_vcam->get_global_position();

		double distance = sqrt(pow((target.x - current.x), 2.0) + pow((target.y - current.y), 2.0));
		calc_duration = distance / blend_data->get_speed();
	}

	blend_tween->stop();

	blend_tween->
		tween_method(
			Callable(this, "set_global_position"),
			get_global_position(),
			p_vcam->get_global_position(),
			calc_duration);	

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


void CineCam2D::reposition_to(VirtualCam2D* p_vcam)
{
	set_global_position(p_vcam->get_global_position());
}


void godot::CineCam2D::shake_offset(const double &p_intensity, const double &p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
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

	shake_offset_intensity_tween->
		tween_method(Callable(this, "set_shake_offset_intensity"), shake_offset_intensity, 0.0, p_duration);
	shake_offset_duration_tween
		->tween_method(Callable(this, "set_shake_offset_duration"), shake_offset_duration, 0.0, p_duration);

	is_shake_offset_active = true;
	emit_signal(SIGNAL_SHAKE_OFFSET_STARTED);
}


void godot::CineCam2D::shake_zoom(const double& p_intensity, const double& p_duration, Tween::EaseType p_ease, Tween::TransitionType p_trans)
{
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

	shake_zoom_intensity_tween->
		tween_method(Callable(this, "set_shake_zoom_intensity"), shake_zoom_intensity, 0.0, p_duration);
	shake_zoom_duration_tween
		->tween_method(Callable(this, "set_shake_zoom_duration"), shake_zoom_duration, 0.0, p_duration);

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



void CineCam2D::_register_vcam_internal(VirtualCam2D* p_vcam)
{
	UtilityFunctions::print("VirtualCam: " + p_vcam->get_name());
	
	if (!vcams.has(p_vcam))
	{
		p_vcam->connect(SIGNAL_PRIORITY_CHANGED, Callable(this, "_on_vcam_priority_changed"));
		p_vcam->connect("tree_exiting", Callable(this, "_remove_vcam_internal"));
		vcams.push_back(cast_to<VirtualCam2D>(p_vcam));
	}

	_set_highest_vcam_internal(p_vcam, p_vcam->get_priority());
	_reposition_by_priority_mode();
}


void godot::CineCam2D::_remove_vcam_internal(VirtualCam2D* p_vcam)
{
	if (vcams.has(p_vcam))
	{
		vcams.erase(p_vcam);
	}
}


void CineCam2D::_set_highest_vcam_internal(VirtualCam2D* p_vcam, int vcam_prio)
{
	if (highest_prio_vcam != nullptr)
	{
		if (vcam_prio > highest_prio_vcam->get_priority())
		{
			highest_prio_vcam = p_vcam;
			return;
		}
	}

	int prio = -1;

	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam2D* cursor = cast_to<VirtualCam2D>(vcams[i]);

		if (cursor->get_priority() > prio)
		{
			prio = cursor->get_priority();
			highest_prio_vcam = cursor;
		}
	}
}


void CineCam2D::_on_vcam_priority_changed(VirtualCam2D* p_vcam, int vcam_prio)
{
	_set_highest_vcam_internal(p_vcam, vcam_prio);
	_reposition_by_priority_mode();
}


void CineCam2D::_reposition_by_priority_mode()
{
	if (highest_prio_vcam == nullptr)
	{
		UtilityFunctions::push_error("ERROR! highest_prio_vcam not set!\n This is probably a bug in the GDExtension.");
	}

	switch (priority_mode)
	{
		case PriorityMode::OFF:
			break;
		case PriorityMode::INSTANT:
			reposition_to(highest_prio_vcam);
			break;
		case PriorityMode::INSTANT_FOLLOW:
			reposition_to(highest_prio_vcam);
			//set follow
			break;
		case PriorityMode::BLEND:
			Ref<BlendData2D> blend = default_blend;
			if (highest_prio_vcam->get_default_blend_data().is_valid())
			{
				blend = default_blend;
			}
			blend_to(highest_prio_vcam, highest_prio_vcam->get_default_blend_data());
			break;
		case PriorityMode::BLEND_FOLLOW:
			blend_to(highest_prio_vcam, highest_prio_vcam->get_default_blend_data());
			// set follow
			break;
	}
}


void CineCam2D::_process(double delta)
{
	shake_offset_internal(delta);
	shake_zoom_internal(delta);
}


void CineCam2D::_notification(int p_what)
{
	switch (p_what)
	{
		default:
			break;
		case NOTIFICATION_READY:
			init_tweens();
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
