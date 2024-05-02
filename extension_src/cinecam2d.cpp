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
	//ADD_METHOD_BINDING(init_default_blend_data, CineCam2D);

	ADD_METHOD_BINDING(seq_blend_next, CineCam2D);
	ADD_METHOD_BINDING(seq_blend_prev, CineCam2D);
	ADD_METHOD_ARGS_BINDING(seq_blend_to, CineCam2D, VA_LIST("idx"));
	//ADD_METHOD_ARGS_BINDING(_register_vcam_internal, CineCam2D, VA_LIST("vcam"));

	//ADD_GETSET_HINT_BINDING(get_default_blend_data, set_default_blend_data, default_blend, p_default_blend, CineCam2D, OBJECT, godot::PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D");
	ADD_GETSET_BINDING(get_current_sequence, set_current_sequence, current_sequence, p_sequence, CineCam2D, Variant::NODE_PATH);

	ADD_GETSET_BINDING(get_priority_mode, set_priority_mode, priority_mode, mode, CineCam2D, Variant::BOOL);

	ADD_GETSET_HINT_BINDING(get_shake_offset_intensity, set_shake_offset_intensity, shake_offset_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_offset_duration, set_shake_offset_duration, shake_offset_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_GETSET_HINT_BINDING(get_shake_zoom_intensity, set_shake_zoom_intensity, shake_zoom_intensity, intensity, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");
	ADD_GETSET_HINT_BINDING(get_shake_zoom_duration, set_shake_zoom_duration, shake_zoom_duration, duration, CineCam2D, FLOAT, godot::PROPERTY_HINT_RANGE, "0.1, 0.001 or_greater");

	ADD_METHOD_DEFAULTARGS_BINDING(shake_offset, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));
	ADD_METHOD_DEFAULTARGS_BINDING(shake_zoom, CineCam2D, VA_LIST("intensity", "duration", "ease", "trans"), VA_LIST(DEFVAL(DEFAULT_EASE), DEFVAL(DEFAULT_TRANS)));

	ADD_METHOD_ARGS_BINDING(blend_to, CineCam2D, VA_LIST("vcam2d", "blend_data"));
	ADD_METHOD_ARGS_BINDING(start_sequence, CineCam2D, VA_LIST("sequence"));


	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_OFFSET_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SHAKE_ZOOM_ENDED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_BLEND_COMPLETED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_STARTED));
	ADD_SIGNAL(MethodInfo(SIGNAL_SEQUENCE_COMPLETED));

	ClassDB::bind_method(D_METHOD("_on_blend_completed_internal"), &CineCam2D::_on_blend_completed_internal);
}


void CineCam2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());

	//init_default_blend_data();
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

/*
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
*/

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


void CineCam2D::blend_to(VirtualCam2D* vcam2d, Ref<BlendData2D> blend_data)
{
	blend_tween->set_trans(blend_data->get_trans());
	blend_tween->set_ease(blend_data->get_ease());

	double calc_duration = blend_data->get_duration();

	if (blend_data->get_blend_by() == BlendData2D::BlendByType::SPEED)
	{
		Vector2 current = get_global_position();
		Vector2 target = vcam2d->get_global_position();

		double distance = sqrt(pow((target.x - current.x), 2.0) + pow((target.y - current.y), 2.0));
		calc_duration = distance / blend_data->get_speed();
	}

	blend_tween->
		tween_method(
			Callable(this, "set_global_position"),
			get_global_position(),
			vcam2d->get_global_position(),
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


void godot::CineCam2D::start_sequence(CamSequence2D* p_sequence)
{
	current_sequence = p_sequence;
	
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


/*
void CineCam2D::_register_vcam_internal(VirtualCam2D p_vcam)
{
	
	if (highest_prio_vcam != nullptr)
	{
		if (vcam->get_priority() > highest_prio_vcam->get_priority())
		{
			highest_prio_vcam = vcam;
			return;
		}
	}

	if (!vcams.has(vcam))
	{
		vcams.append(cast_to<VirtualCam2D>(vcam));
	}

	int prio = -1;

	for (int i = 0; i < vcams.size(); i++)
	{
		VirtualCam2D* cursor = cast_to<VirtualCam2D>(vcams[i]);
		if (cursor->get_priority() > prio)
		{
			highest_prio_vcam = cursor;
		}
	}
	
}
*/

void CineCam2D::_process(double delta)
{
	//if (priority_mode)
	//{
		//set_global_position(highest_prio_vcam->get_global_position());
	//}

	shake_offset_internal(delta);
	shake_zoom_internal(delta);
}


void CineCam2D::_ready()
{
	init_tweens();
}


bool CineCam2D::get_priority_mode() const
{
	return priority_mode;
}


void CineCam2D::set_priority_mode(bool mode)
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

/*
Ref<BlendData2D> CineCam2D::get_default_blend_data() const
{
	return default_blend;
}


void CineCam2D::set_default_blend_data(Ref<BlendData2D> blend_data)
{
	default_blend = blend_data;
}
*/

CamSequence2D* CineCam2D::get_current_sequence() const
{
	return current_sequence;
}


void CineCam2D::set_current_sequence(CamSequence2D* p_sequence)
{
	current_sequence = p_sequence;
}
