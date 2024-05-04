// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.h

#ifndef CINECAM2D_H
#define CINECAM2D_H

#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/method_tweener.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam2d.h"
#include "blend_data2d.h"
#include "cam_sequence2d.h"

#define DEFAULT_EASE Tween::EASE_OUT
#define DEFAULT_TRANS Tween::TRANS_CUBIC
#define SIGNAL_SHAKE_OFFSET_STARTED "shake_offset_started"
#define SIGNAL_SHAKE_OFFSET_ENDED "shake_offset_ended"
#define SIGNAL_SHAKE_ZOOM_STARTED "shake_zoom_started"
#define SIGNAL_SHAKE_ZOOM_ENDED "shake_zoom_ended"
#define SIGNAL_BLEND_STARTED "blend_started"
#define SIGNAL_BLEND_COMPLETED "blend_completed"
#define SIGNAL_SEQUENCE_STARTED "sequence_started"
#define SIGNAL_SEQUENCE_COMPLETED "sequence_completed"
#define PRIORITY_MODE_HINTS "OFF,INSTANT,INSTANT_FOLLOW,BLEND,BLEND_FOLLOW"

namespace godot
{
	class CineCam2D : public Camera2D
	{
		GDCLASS(CineCam2D, Camera2D)
		
	public:
		enum PriorityMode
		{
			OFF,
			INSTANT,
			INSTANT_FOLLOW,
			BLEND,
			BLEND_FOLLOW
		};
	// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		Ref<Tween> shake_offset_intensity_tween;
		Ref<Tween> shake_offset_duration_tween;
		Vector2 original_offset;
		bool is_shake_offset_active;

		Ref<Tween> shake_zoom_intensity_tween;
		Ref<Tween> shake_zoom_duration_tween;
		Vector2 original_zoom;
		bool is_shake_zoom_active;

		Ref<Tween> blend_tween;
		VirtualCam2D* highest_prio_vcam;
		TypedArray<VirtualCam2D> vcams;

		void initialize_internal();
		void init_tweens();
		void init_default_blend_data();
		void _on_blend_completed_internal();
		void shake_offset_internal(double);
		void shake_zoom_internal(double);


	public:
		CineCam2D();
		~CineCam2D();


	protected:
		static void _bind_methods();


	// GODOT Overrides
	public:
		void _process(double delta) override;
		void _notification(int p_what);


	// GODOT public
	private:
		Ref<BlendData2D> default_blend;
		CamSequence2D* current_sequence;
		CineCam2D::PriorityMode priority_mode;
		double shake_offset_intensity;
		double shake_offset_duration;
		double shake_zoom_intensity;
		double shake_zoom_duration;

	public:
		void blend_to(VirtualCam2D* p_vcam, Ref<BlendData2D> blend_data);
		void seq_blend_next();
		void seq_blend_prev();
		void seq_blend_to(int idx);

		void reposition_to(VirtualCam2D* p_vcam);

		void shake_offset(const double &p_intensity,
			const double &p_duration,
			Tween::EaseType p_ease = DEFAULT_EASE,
			Tween::TransitionType p_trans = DEFAULT_TRANS);


		void shake_zoom(const double& p_intensity,
			const double& p_duration,
			Tween::EaseType p_ease = DEFAULT_EASE,
			Tween::TransitionType p_trans = DEFAULT_TRANS);

		void start_sequence();
		void _register_vcam_internal(VirtualCam2D* p_vcam);
		void _remove_vcam_internal(VirtualCam2D* p_vcam);
		void _set_highest_vcam_internal(VirtualCam2D* p_vcam, int vcam_prio);
		void _on_vcam_priority_changed(VirtualCam2D* p_vcam, int prio);
		void _reposition_by_priority_mode();

		CineCam2D::PriorityMode get_priority_mode() const;
		void set_priority_mode(const CineCam2D::PriorityMode mode);

		double get_shake_offset_intensity() const;
		void set_shake_offset_intensity(const double &p_intensity);

		double get_shake_offset_duration() const;
		void set_shake_offset_duration(const double &p_duration);

		double get_shake_zoom_intensity() const;
		void set_shake_zoom_intensity(const double &p_intensity);

		double get_shake_zoom_duration() const;
		void set_shake_zoom_duration(const double &p_duration);

		Ref<BlendData2D> get_default_blend_data() const;
		void set_default_blend_data(Ref<BlendData2D> blend_data);
		
		CamSequence2D* get_current_sequence() const;
		void set_current_sequence(CamSequence2D* p_sequence);

		VirtualCam2D* prioritized_vcam() const;
		VirtualCam2D* find_vcam_by_id(String id) const;


	protected:
	};
}

	VARIANT_ENUM_CAST(CineCam2D::PriorityMode)

#endif // CINECAM_H