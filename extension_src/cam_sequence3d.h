// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_sequence3d.h

#ifndef CAM_SEQUENCE3D_H
#define CAM_SEQUENCE3D_H

#include "godot_cpp/classes/node3d.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam3d.h"

#define SIGNAL_VCAM_INDEX_CHANGED "index_changed"

namespace godot
{
	class CamSequence3D : public Node3D
	{
		GDCLASS(CamSequence3D, Node3D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();

		void gather_vcams_in_children_internal();
		void _debug_gizmos_drawing(bool editor);


	public:
		CamSequence3D();
		~CamSequence3D();


	protected:
		static void _bind_methods();


		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _process_internal(bool editor);
		void _notification(int p_what);


		// GODOT public
	private:
		TypedArray<VirtualCam3D> vcams;
		int current_vcam_idx;

		bool draw_debug_gizmos;
		Color debug_gizmos_color;
		double debug_gizmos_width;


	public:
		TypedArray<VirtualCam3D> get_vcam3d_array() const;


		VirtualCam3D* current_vcam() const;

		void set_current_idx(int idx);
		int get_current_idx() const;

		VirtualCam3D* vcam3d_at(int idx) const;
		VirtualCam3D* vcam3d_next() const;
		VirtualCam3D* vcam3d_prev() const;
		VirtualCam3D* vcam3d_first() const;
		VirtualCam3D* vcam3d_last() const;

		bool get_is_draw_debug_gizmos() const;
		void set_is_draw_debug_gizmos(bool draw);

		Color get_debug_gizmos_color() const;
		void set_debug_gizmos_color(Color color);

		double get_debug_gizmos_width() const;
		void set_debug_gizmos_width(double width);

	protected:
	};
}

#endif // CAM_SEQUENCE3D_H