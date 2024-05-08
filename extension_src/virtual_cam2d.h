// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// virtual_cam2d.h

#ifndef VIRTUAL_CAM2D_H
#define VIRTUAL_CAM2D_H

#include "godot_cpp/classes/node2D.hpp"
#include "godot_cpp/classes/camera2d.hpp"

#include "gdclass_metadata.h"
#include "blend_data2d.h"

#define SIGNAL_PRIORITY_CHANGED "priority_changed"

namespace godot
{
	class VirtualCam2D : public Node2D
	{
		GDCLASS(VirtualCam2D, Node2D)

	// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();
		void init_default_blend_data();
		void _register_to_cinecam2d();


	public:
		VirtualCam2D();
		~VirtualCam2D();


	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;


	// GODOT Overrides
	public:
		void _notification(int p_what);


	// GODOT public
	private:
		godot::String vcam_id;
		int priority;

		Vector2 offset;
		Camera2D::AnchorMode anchor_mode = Camera2D::AnchorMode::ANCHOR_MODE_DRAG_CENTER;
		bool ingore_rotation = false;
		bool enabled = true;
		Vector2 zoom;
		Camera2D::Camera2DProcessCallback process_callback;
		Ref<BlendData2D> blend_data;

		int limit[4];
		bool limit_smoothing_enabled = false;

		real_t position_smoothing_speed = 5.0;
		bool position_smoothing_enabled = false;

		real_t rotation_smoothing_speed = 5.0;
		bool rotation_smoothing_enabled = false;

		real_t drag_margin[4];
		bool drag_horizontal_enabled = false;
		bool drag_vertical_enabled = false;
		real_t drag_horizontal_offset = 0.0;
		real_t drag_vertical_offset = 0.0;

		bool screen_drawing_enabled = true;
		bool limit_drawing_enabled = false;
		bool margin_drawing_enabled = false;


	public:
		void set_vcam_id(String id);
		String get_vcam_id() const;


		void set_priority(int prio);
		int get_priority() const;

		void set_offset(const Vector2& p_offset);
		Vector2 get_offset() const;

		void set_anchor_mode(Camera2D::AnchorMode p_anchor_mode);
		Camera2D::AnchorMode get_anchor_mode() const;

		void set_ignore_rotation(bool p_ignore);
		bool is_ignoring_rotation() const;

		void set_enabled(bool p_enabled);
		bool is_enabled() const;

		void set_zoom(const Vector2& p_zoom);
		Vector2 get_zoom() const;

		void set_limit_left(int p_limit);
		int get_limit_left() const;

		void set_limit_top(int p_limit);
		int get_limit_top() const;

		void set_limit_right(int p_limit);
		int get_limit_right() const;

		void set_limit_bottom(int p_limit);
		int get_limit_bottom() const;

		void set_limit(Side p_side, int p_limit);
		int get_limit(Side p_side) const;

		void set_process_callback(Camera2D::Camera2DProcessCallback p_mode);
		Camera2D::Camera2DProcessCallback get_process_callback() const;

		void set_limit_smoothing_enabled(bool enable);
		bool is_limit_smoothing_enabled() const;

		void set_position_smoothing_enabled(bool p_enabled);
		bool is_position_smoothing_enabled() const;

		void set_position_smoothing_speed(real_t p_speed);
		real_t get_position_smoothing_speed() const;

		void set_rotation_smoothing_speed(real_t p_speed);
		real_t get_rotation_smoothing_speed() const;

		void set_rotation_smoothing_enabled(bool p_enabled);
		bool is_rotation_smoothing_enabled() const;

		void set_drag_horizontal_enabled(bool p_enabled);
		bool is_drag_horizontal_enabled() const;

		void set_drag_vertical_enabled(bool p_enabled);
		bool is_drag_vertical_enabled() const;

		void set_drag_margin_left(real_t p_drag_margin);
		real_t get_drag_margin_left() const;

		void set_drag_margin_top(real_t p_drag_margin);
		real_t get_drag_margin_top() const;

		void set_drag_margin_right(real_t p_drag_margin);
		real_t get_drag_margin_right() const;

		void set_drag_margin_bottom(real_t p_drag_margin);
		real_t get_drag_margin_bottom() const;

		void set_drag_margin(Side p_side, real_t p_drag_margin);
		real_t get_drag_margin(Side p_side) const;

		void set_drag_horizontal_offset(real_t p_offset);
		real_t get_drag_horizontal_offset() const;

		void set_drag_vertical_offset(real_t p_offset);
		real_t get_drag_vertical_offset() const;

		void set_screen_drawing_enabled(bool enable);
		bool is_screen_drawing_enabled() const;

		void set_limit_drawing_enabled(bool enable);
		bool is_limit_drawing_enabled() const;

		void set_margin_drawing_enabled(bool enable);
		bool is_margin_drawing_enabled() const;

		Ref<BlendData2D> _get_blend_data() const;
		void _set_blend_data(Ref<BlendData2D> blend);

	protected:
	};
}

#endif // VIRTUAL_CAM2D_H