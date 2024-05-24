// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// blend_data3d.h

#ifndef BLEND_DATA3D_H
#define BLEND_DATA3D_H

#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/method_tweener.hpp"

#include "gdclass_metadata.h"

#define BLEND_BY_HINTS "DURATION ,SPEED"
#define EASE_HINTS "EASE_IN, EASE_OUT, EASE_IN_OUT, EASE_OUT_IN"
#define TRANS_HINTS "TRANS_LINEAR, TRANS_SINE, TRANS_QUINT, TRANS_QUART, TRANS_QUAD, TRANS_EXPO, TRANS_ELASTIC, TRANS_CUBIC, TRANS_CIRC, TRANS_BOUNCE, TRANS_BACK, TRANS_SPRING"

namespace godot
{
	class BlendData3D : public Resource
	{
		GDCLASS(BlendData3D, Resource)

	public:
		enum BlendByType
		{
			DURATION,
			SPEED
		};

		// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		BlendData3D();
		~BlendData3D();
		bool _is_default_blend() const;


	protected:
		static void _bind_methods();

		// GODOT Overrides

		// GODOT public
	private:
		godot::String blend_name;
		BlendData3D::BlendByType blend_by;
		double blend_by_value;
		Tween::EaseType ease;
		Tween::TransitionType trans;
		Callable callable;
		bool blend_rotation;
		bool callable_on_start;
		bool callable_on_complete;


	public:
		void set_blend_name(godot::String p_name);
		godot::String get_blend_name() const;

		void set_blend_by(BlendData3D::BlendByType by);
		BlendData3D::BlendByType get_blend_by() const;

		void set_blend_by_value(double p_value);
		double get_blend_by_value() const;

		void set_ease(Tween::EaseType ease);
		Tween::EaseType get_ease() const;

		void set_trans(Tween::TransitionType trans);
		Tween::TransitionType get_trans() const;

		void set_callable(Callable callable);
		Callable get_callable() const;

		void set_blend_rotation(bool rotation);
		bool is_blend_rotation() const;

		void set_callable_on_start(bool enable);
		bool get_callable_on_start() const;

		void set_callable_on_complete(bool enable);
		bool get_callable_on_complete() const;
	};
}
VARIANT_ENUM_CAST(BlendData3D::BlendByType);

#endif // BLEND_DATA3D_H