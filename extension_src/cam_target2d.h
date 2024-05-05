// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target2d.h

#ifndef CAM_TARGET2D_H
#define CAM_TARGET2D_H

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/tween.hpp"

#include "gdclass_metadata.h"

#define EASE_HINTS "EASE_IN, EASE_OUT, EASE_IN_OUT, EASE_OUT_IN"
#define TRANS_HINTS "TRANS_LINEAR, TRANS_SINE, TRANS_QUINT, TRANS_QUART, TRANS_QUAD, TRANS_EXPO, TRANS_ELASTIC, TRANS_CUBIC, TRANS_CIRC, TRANS_BOUNCE, TRANS_BACK, TRANS_SPRING"

namespace godot
{
	class CamTarget2D : public Node2D
	{
		GDCLASS(CamTarget2D, Node2D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		CamTarget2D();
		~CamTarget2D();

		double get_scaled_speed() const;
		

	protected:
		static void _bind_methods();



		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _notification(int p_what);


		// GODOT public
	private:
		Vector2 target_offset;
		godot::String target_name;
		double speed;
		Tween::EaseType ease;
		Tween::TransitionType trans;
		Callable callable;

	public:
		Vector2 get_target_offset() const;
		void set_target_offset(Vector2 offset);

		void set_target_name(godot::String p_name);
		godot::String get_target_name() const;

		void set_speed(double speed);
		double get_speed() const;

		void set_ease(Tween::EaseType ease);
		Tween::EaseType get_ease() const;

		void set_trans(Tween::TransitionType trans);
		Tween::TransitionType get_trans() const;

		void set_callable(Callable callable);
		Callable get_callable() const;
	protected:
	};
}

#endif // CAM_TARGET2D_H