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

		double scaled_speed() const;


	protected:
		static void _bind_methods();



		// GODOT Overrides
	public:


		// GODOT public
	private:
		Vector3 target_offset;
		double speed;
		Tween::EaseType ease;
		Tween::TransitionType trans;

	public:
		Vector3 get_target_offset() const;
		void set_target_offset(Vector3 offset);

		void set_speed(double speed);
		double get_speed() const;

		void set_ease(Tween::EaseType ease);
		Tween::EaseType get_ease() const;

		void set_trans(Tween::TransitionType trans);
		Tween::TransitionType get_trans() const;
	protected:
	};
}

#endif // CAM_TARGET3D_H