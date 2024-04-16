// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// cinecam2d.h

#ifndef CINECAM2D_H
#define CINECAM2D_H

#include "godot_cpp/classes/camera2d.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class CineCam2D : public Camera2D
	{
		GDCLASS(CineCam2D, Camera2D)

	private:
		double shake_strength;
		double shake_duration;
		godot::String additional_description;
		GDCLASS_Metadata meta;

	//INTERNAL Methods
	private: 
		void shake_cam_internal(double);

	protected:
		static void _bind_methods();
		void initialize_internal();

	//GODOT Methods
	public:
		void _process(double delta) override;
		void shake_cam(double, double);

	//GET SET
	public:
		CineCam2D();
		~CineCam2D();

		double get_shake_strength() const;
		void set_shake_strength(const double p_shake_strength);

		double get_shake_duration() const;
		void set_shake_duration(const double p_shake_duration);
	};
}

#endif // CINECAM_H