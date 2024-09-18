// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cinecam3d.h

#ifndef CINECAM3D_H
#define CINECAM3D_H

#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/method_tweener.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam3d.h"
#include "cam_target3d.h"
#include "cam_sequence3d.h"

#define DEFAULT_EASE Tween::EASE_OUT
#define DEFAULT_TRANS Tween::TRANS_CUBIC

#define SIGNAL_SHAKE_OFFSET_STARTED "shake_offset_started"
#define SIGNAL_SHAKE_OFFSET_ENDED "shake_offset_ended"
#define SIGNAL_SHAKE_FOV_STARTED "shake_fov_started"
#define SIGNAL_SHAKE_FOV_ENDED "shake_fov_ended"
#define SIGNAL_SHAKE_ROTATION_STARTED "shake_rotation_started"
#define SIGNAL_SHAKE_ROTATION_ENDED "shake_rotation_ended"

#define SIGNAL_BLEND_STARTED "blend_started"
#define SIGNAL_BLEND_COMPLETED "blend_completed"
#define SIGNAL_BLEND_PAUSED "blend_paused"
#define SIGNAL_BLEND_RESUMED "blend_resumed"

#define SIGNAL_SEQUENCE_STARTED "sequence_started"
#define SIGNAL_SEQUENCE_COMPLETED "sequence_completed"
#define SIGNAL_SEQUENCE_PAUSED "sequence_paused"
#define SIGNAL_SEQUENCE_RESUMED "sequence_resumed"
#define SIGNAL_SEQUENCE_STOPPED "sequence_stopped"

#define SIGNAL_PRIORITIZED_VCAM3D_CHANGED "prioritized_vcam3d_changed"

#define FOLLOW_MODE_HINTS "OFF,PRIO,PRIO_ONESHOT,PRIO_BLEND,TARGET,TARGET_BLEND"

namespace godot
{
	class CineCam3D : public Camera3D
	{
		GDCLASS(CineCam3D, Camera3D)

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

		Ref<Tween> shake_fov_intensity_tween;
		Ref<Tween> shake_fov_duration_tween;
		double original_fov;
		bool is_shake_fov_active;

		Ref<Tween> shake_rotation_intensity_tween;
		Ref<Tween> shake_rotation_duration_tween;
		Vector3 original_rotation; // for resetting shake only
		bool is_shake_rotation_active;

		Ref<Tween> blend_position_tween;
		Ref<Tween> blend_rotation_tween;

		Ref<Tween> follow_tween;
		Vector3 follow_origin;
		Vector3 camera_origin;

		Ref<Tween> look_at_tween;
		Vector3 origin_for_look_at;
		Vector3 rotation_shake;

		bool tweens_ready;
		VirtualCam3D* highest_prio_vcam;
		TypedArray<VirtualCam3D> vcams;
		SceneTree* scene_tree;

		bool sequence_backwards;
		bool sequence_playmode = false;
		bool is_blend_not_stopped = false;

		double blend_duration;

		void initialize_internal();
		void init_tweens();
		void init_default_blend_data();
		void shake_offset_internal();
		void shake_fov_internal();
		void shake_rotation_internal();
		double _calc_blend_duration_by_speed(Vector3 current_pos, Vector3 target_pos, double speed);
		void _move_by_follow_mode();
		void init_active_blend();
		void cycle_sequence_internal();
		void look_at_target_internal();
		Vector3 get_look_at_direction() const;
		void set_quaternion_internal(Quaternion q);


	public:
		CineCam3D();
		~CineCam3D();


	protected:
		static void _bind_methods();


		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _process_internal(bool editor);
		void _notification(int p_what);


		// GODOT public
	private:
		Ref<BlendData3D> blend_data;
		Ref<BlendData3D> active_blend;
		CamSequence3D* current_sequence;
		CineCam3D::FollowMode follow_mode;
		CamTarget3D* follow_target;
		Vector2 shake_offset_intensity;
		double shake_offset_duration;
		double shake_fov_intensity;
		double shake_fov_duration;
		Vector3 shake_rotation_intensity;
		double shake_rotation_duration;
		bool is_sequence_paused = false;
		CamTarget3D* look_at_target;
		bool is_follow_target_paused = false;
		bool is_follow_prio_paused = false;


	public:
		void blend_to(VirtualCam3D* p_vcam, Ref<BlendData3D> blend);
		void pause_blend();
		void resume_blend();
		void toggle_blend();

		void start_sequence(const bool& backwards);
		void start_sequence_at(const int& idx = 0, const bool& backwards = false);
		void seq_blend_next();
		void seq_blend_prev();
		void seq_blend_to(int idx);
		void seq_resume();
		void seq_pause();
		void seq_stop();

		void pause_follow_target();
		void resume_follow_target();
		void toggle_follow_target();

		void pause_follow_prio();
		void resume_follow_prio();
		void toggle_follow_prio();

		void reposition_to_vcam(VirtualCam3D* p_vcam);

		void shake_offset(const Vector2& p_intensity,
			const double& p_duration,
			Tween::EaseType p_ease = DEFAULT_EASE,
			Tween::TransitionType p_trans = DEFAULT_TRANS);

		void shake_fov(const double& p_intensity,
			const double& p_duration,
			Tween::EaseType p_ease = DEFAULT_EASE,
			Tween::TransitionType p_trans = DEFAULT_TRANS);

		void shake_rotation(const Vector3& p_intensity,
			const double& p_duration,
			Tween::EaseType p_ease = DEFAULT_EASE,
			Tween::TransitionType p_trans = DEFAULT_TRANS);

		String apply_vcam3d_data(VirtualCam3D* p_vcam);

		void _register_vcam_internal(VirtualCam3D* p_vcam);
		void _remove_vcam_internal(VirtualCam3D* p_vcam);
		bool _try_set_highest_vcam_internal(VirtualCam3D* p_vcam, int vcam_prio);
		void _on_vcam_priority_changed(VirtualCam3D* p_vcam, int prio);
		void _move_by_priority_mode();
		void _on_blend_started_internal();
		void _on_blend_completed_internal();

		CineCam3D::FollowMode get_follow_mode() const;
		void set_follow_mode(const CineCam3D::FollowMode mode);

		CamTarget3D* get_target() const;
		void set_target(CamTarget3D* p_target);

		Vector2 _get_shake_offset_intensity() const;
		void _set_shake_offset_intensity(const Vector2& p_intensity);

		double _get_shake_offset_duration() const;
		void _set_shake_offset_duration(const double& p_duration);

		double _get_shake_fov_intensity() const;
		void _set_shake_fov_intensity(const double& p_intensity);

		double _get_shake_fov_duration() const;
		void _set_shake_fov_duration(const double& p_duration);

		Vector3 _get_shake_rotation_intensity() const;
		void _set_shake_rotation_intensity(const Vector3& p_intensity);

		double _get_shake_rotation_duration() const;
		void _set_shake_rotation_duration(const double& p_duration);

		Ref<BlendData3D> _get_blend_data() const;
		void _set_blend_data(Ref<BlendData3D> blend);

		CamSequence3D* get_current_sequence() const;
		void set_current_sequence(CamSequence3D* p_sequence);

		VirtualCam3D* prioritized_vcam() const;
		VirtualCam3D* find_vcam_by_id(String id) const;

		void _set_seq_paused(bool paused);
		bool _is_seq_paused() const;

		void _set_blend_paused(bool paused);
		bool _is_blend_paused() const;

		void _set_follow_target_paused(bool paused);
		bool _is_follow_target_paused() const;

		void _set_follow_prio_paused(bool paused);
		bool _is_follow_prio_paused() const;

		void set_look_at_target(CamTarget3D* p_target);
		CamTarget3D* get_look_at_target() const;

		double full_blend_duration() const;

	protected:
	};
}
VARIANT_ENUM_CAST(CineCam3D::FollowMode)

#endif // CINECAM3D_H