// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_sequence2d.h

#ifndef CAM_SEQUENCE2D_H
#define CAM_SEQUENCE2D_H

#include "godot_cpp/classes/node2d.hpp"

#include "gdclass_metadata.h"
#include "virtual_cam2d.h"

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

		TypedArray<VirtualCam2D> get_vcams_in_children_internal() const;


	public:
		CamSequence2D();
		~CamSequence2D();


	protected:
		static void _bind_methods();
		void _notification(int p_what);


		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _ready() override;


		// GODOT public
	private:
		TypedArray<VirtualCam2D> vcams;
		int current_vcam_idx;

	public:
		TypedArray<VirtualCam2D> get_vcam2d_array() const;

		
		VirtualCam2D* current_vcam() const;

		void set_current_idx(int idx);
		int get_current_idx() const;

	protected:
	};
}

#endif // CAM_SEQUENCE2D_H