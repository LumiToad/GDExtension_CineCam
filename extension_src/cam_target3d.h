// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target3d.h

#ifndef CAM_TARGET3D_H
#define CAM_TARGET3D_H

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/tween.hpp"

#include "gdclass_metadata.h"

#define EASE_HINTS "EASE_IN, EASE_OUT, EASE_IN_OUT, EASE_OUT_IN"
#define TRANS_HINTS "TRANS_LINEAR, TRANS_SINE, TRANS_QUINT, TRANS_QUART, TRANS_QUAD, TRANS_EXPO, TRANS_ELASTIC, TRANS_CUBIC, TRANS_CIRC, TRANS_BOUNCE, TRANS_BACK, TRANS_SPRING"

namespace godot
{
	class CamTarget3D : public Node3D
	{
		GDCLASS(CamTarget3D, Node3D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		CamTarget3D();
		~CamTarget3D();

		Vector3 scaled_speed() const;


	protected:
		static void _bind_methods();



		// GODOT Overrides
	public:


		// GODOT public
	private:
		Vector3 target_offset;
		double speed_x;
		double speed_y;
		double speed_z;
		Tween::EaseType ease;
		Tween::TransitionType trans;

	public:
		Vector3 get_target_offset() const;
		void set_target_offset(Vector3 offset);

		void set_speed_x(double speed_x);
		double get_speed_x() const;

		void set_speed_y(double speed_y);
		double get_speed_y() const;

		void set_speed_z(double speed_z);
		double get_speed_z() const;

		void set_ease(Tween::EaseType ease);
		Tween::EaseType get_ease() const;

		void set_trans(Tween::TransitionType trans);
		Tween::TransitionType get_trans() const;
	protected:
	};
}

#endif // CAM_TARGET3D_H