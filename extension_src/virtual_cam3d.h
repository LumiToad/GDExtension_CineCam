// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// virtual_cam3d.h

#ifndef VIRTUAL_CAM3D_H
#define VIRTUAL_CAM3D_H

#include "godot_cpp/templates/vector.hpp"
#include "godot_cpp/classes/camera_attributes.hpp"
#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/camera_attributes_physical.hpp"
#include "godot_cpp/classes/environment.hpp"

#include "gdclass_metadata.h"
#include "blend_data3d.h"

#ifdef MINGW_ENABLED
#undef near
#undef far
#endif

#define SIGNAL_PRIORITY_CHANGED "priority_changed"

namespace godot
{
	class VirtualCam3D : public Node3D
	{
		GDCLASS(VirtualCam3D, Node3D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;
		Camera3D* found_cam;

		void initialize_internal();
		void init_default_blend_data();
		void _register_to_cinecam3d();
		void _remove_from_cinecam3d();


	public:
		VirtualCam3D();
		~VirtualCam3D();


	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;


		// GODOT Overrides
	public:
		void _notification(int p_what);


		// GODOT public
	private:
		// original VirtualCam3D
		godot::String vcam_id;
		int priority;
		Ref<BlendData3D> blend_data;
		SceneTree* scene_tree;

		// from Camera3D
		bool force_change = false;
		bool current = true;
		Viewport* viewport = nullptr;

		Camera3D::ProjectionType mode = Camera3D::PROJECTION_PERSPECTIVE;

		real_t fov = 75.0;
		real_t size = 1.0;
		Vector2 frustum_offset;
		real_t near = 0.05;
		real_t far = 4000.0;
		real_t v_offset = 0.0;
		real_t h_offset = 0.0;
		Camera3D::KeepAspect keep_aspect = Camera3D::KEEP_HEIGHT;

		RID scenario_id;

		uint32_t layers = 0xfffff;

		Ref<Environment> environment;
		Ref<CameraAttributes> attributes;
		void _attributes_changed();

		friend class Viewport;
		TypedArray<Plane> _get_frustum() const;

		Camera3D::DopplerTracking doppler_tracking = Camera3D::DOPPLER_TRACKING_DISABLED;

		RID pyramid_shape;
		godot::Vector<Vector3> pyramid_shape_points;

	public:
		// original VirtualCam3D
		void set_vcam_id(String id);
		String get_vcam_id() const;

		void set_priority(int prio);
		int get_priority() const;

		Ref<BlendData3D> _get_blend_data() const;
		void _set_blend_data(Ref<BlendData3D> blend);

		// from Camera3D
		void set_perspective(real_t p_fovy_degrees, real_t p_z_near, real_t p_z_far);
		void set_orthogonal(real_t p_size, real_t p_z_near, real_t p_z_far);
		void set_frustum(real_t p_size, Vector2 p_offset, real_t p_z_near, real_t p_z_far);
		void set_projection(Camera3D::ProjectionType p_mode);

		void set_current(bool p_enabled);
		bool is_current() const;

		real_t get_fov() const;
		real_t get_size() const;
		real_t get_far() const;
		real_t get_near() const;
		Vector2 get_frustum_offset() const;

		Camera3D::ProjectionType get_projection() const;

		void set_fov(real_t p_fov);
		void set_size(real_t p_size);
		void set_far(real_t p_far);
		void set_near(real_t p_near);
		void set_frustum_offset(Vector2 p_offset);

		virtual Transform3D get_camera_transform() const;
		virtual Projection get_camera_projection() const;

		bool is_position_behind(const Vector3& p_pos) const;

		Vector<Vector3> get_near_plane_points() const;

		void set_cull_mask(uint32_t p_layers);
		uint32_t get_cull_mask() const;

		void set_cull_mask_value(int p_layer_number, bool p_enable);
		bool get_cull_mask_value(int p_layer_number) const;

		virtual TypedArray<Plane> get_frustum() const;
		bool is_position_in_frustum(const Vector3& p_position) const;

		void set_environment(const Ref<Environment>& p_environment);
		Ref<Environment> get_environment() const;

		void set_attributes(const Ref<CameraAttributes>& p_effects);
		Ref<CameraAttributes> get_attributes() const;

		void set_keep_aspect_mode(Camera3D::KeepAspect p_aspect);
		Camera3D::KeepAspect get_keep_aspect_mode() const;

		void set_v_offset(real_t p_offset);
		real_t get_v_offset() const;

		void set_h_offset(real_t p_offset);
		real_t get_h_offset() const;

		void set_doppler_tracking(Camera3D::DopplerTracking p_tracking);
		Camera3D::DopplerTracking get_doppler_tracking() const;

		RID get_pyramid_shape_rid();


	protected:
		void _update_camera_mode();

		Projection _get_camera_projection(real_t p_near) const;
	};
}

#endif // VIRTUAL_CAM3D_H