// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_sequence2d.h

#ifndef CAM_SEQUENCE2D_H
#define CAM_SEQUENCE2D_H

#include "godot_cpp/classes/node2d.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam2d.h"

#define SIGNAL_VCAM_INDEX_CHANGED "index_changed"

namespace godot
{
	class CamSequence2D : public Node2D
	{
		GDCLASS(CamSequence2D, Node2D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();

		void gather_vcams_in_children_internal();
		void _debug_lines_drawing(bool editor);


	public:
		CamSequence2D();
		~CamSequence2D();


	protected:
		static void _bind_methods();


		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _process_internal(bool editor);
		void _notification(int p_what);


		// GODOT public
	private:
		TypedArray<VirtualCam2D> vcams;
		int current_vcam_idx;
		
		bool draw_debug_lines;
		Color debug_lines_color;
		double debug_lines_width;


	public:
		TypedArray<VirtualCam2D> get_vcam2d_array() const;

		
		VirtualCam2D* current_vcam() const;

		void set_current_idx(int idx);
		int get_current_idx() const;

		VirtualCam2D* vcam2d_at(int idx) const;
		VirtualCam2D* vcam2d_next() const;
		VirtualCam2D* vcam2d_prev() const;
		VirtualCam2D* vcam2d_first() const;
		VirtualCam2D* vcam2d_last() const;

		bool get_is_draw_debug_lines() const;
		void set_is_draw_debug_lines(bool draw);

		Color get_debug_lines_color() const;
		void set_debug_lines_color(Color color);

		double get_debug_lines_width() const;
		void set_debug_lines_width(double width);

	protected:
	};
}

#endif // CAM_SEQUENCE2D_H