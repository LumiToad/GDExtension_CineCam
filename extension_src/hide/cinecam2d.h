// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.h

#ifndef CINECAM2D_H
#define CINECAM2D_H

#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/method_tweener.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam2d.h"
#include "cam_target2d.h"
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
#define TARGET_MODE_HINTS "OFF,PRIO,PRIO_ONESHOT,PRIO_BLEND,TARGET,TARGET_BLEND"
#define SIGNAL_PRIORITIZED_VCAM2D_CHANGED "prioritized_vcam2d_changed"

namespace godot
{
	class CineCam2D : public Camera2D
	{
		GDCLASS(CineCam2D, Camera2D)
		
	public:
		enum FollowMode
		{
			OFF,
			PRIO,
			PRIO_ONESHOT,
			PRIO_BLEND,
			TARGET,
			TARGET_BLEND
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
		
		Ref<Tween> follow_tween;
		Vector2 follow_origin;

		bool tweens_ready;
		VirtualCam2D* highest_prio_vcam;
		TypedArray<VirtualCam2D> vcams;

		void initialize_internal();
		void init_tweens();
		void init_default_blend_data();
		void shake_offset_internal(double);
		void shake_zoom_internal(double);
		double _calc_blend_duration_by_speed(Vector2 current_pos, Vector2 target_pos, double speed);
		void _move_by_follow_mode();


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
		Ref<BlendData2D> blend_data;
		Ref<BlendData2D> active_blend;
		CamSequence2D* current_sequence;
		CineCam2D::FollowMode follow_mode;
		CamTarget2D* follow_target;
		double shake_offset_intensity;
		double shake_offset_duration;
		double shake_zoom_intensity;
		double shake_zoom_duration;

	public:
		void blend_to(VirtualCam2D* p_vcam, Ref<BlendData2D> blend_data);
		void seq_blend_next();
		void seq_blend_prev();
		void seq_blend_to(int idx);

		void reposition_to_vcam(VirtualCam2D* p_vcam);

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
		bool _try_set_highest_vcam_internal(VirtualCam2D* p_vcam, int vcam_prio);
		void _on_vcam_priority_changed(VirtualCam2D* p_vcam, int prio);
		void _move_by_priority_mode();
		void _on_blend_started_internal();
		void _on_blend_completed_internal();

		CineCam2D::FollowMode get_follow_mode() const;
		void set_follow_mode(const CineCam2D::FollowMode mode);

		CamTarget2D* get_target() const;
		void set_target(CamTarget2D* p_target);

		double get_shake_offset_intensity() const;
		void set_shake_offset_intensity(const double &p_intensity);

		double get_shake_offset_duration() const;
		void set_shake_offset_duration(const double &p_duration);

		double get_shake_zoom_intensity() const;
		void set_shake_zoom_intensity(const double &p_intensity);

		double get_shake_zoom_duration() const;
		void set_shake_zoom_duration(const double &p_duration);

		Ref<BlendData2D> _get_blend_data() const;
		void _set_blend_data(Ref<BlendData2D> blend);
		
		CamSequence2D* get_current_sequence() const;
		void set_current_sequence(CamSequence2D* p_sequence);

		VirtualCam2D* prioritized_vcam() const;
		VirtualCam2D* find_vcam_by_id(String id) const;


	protected:
	};
}
	VARIANT_ENUM_CAST(CineCam2D::FollowMode)

#endif // CINECAM_H