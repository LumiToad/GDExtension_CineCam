#ifndef CINECAM2D_H
#define CINECAM2D_H

#include "bind_utils.h"

#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

namespace godot
{
	class CineCam2D : public Camera2D
	{
		GDCLASS(CineCam2D, Camera2D)

	private:
		double shake_strength;
		double shake_duration;

	//INTERNAL Methods
	private: 
		void shake_cam_internal(double);

	protected:
		static void _bind_methods();

	//GODOT Methods
	public:
		void _process(double delta) override;
		void shake_cam(double, double);

	//GET SET
	public:
		CineCam2D();
		~CineCam2D();

		/*double get_shake_strength() const;
		void set_shake_strength(const double p_shake_strength);

		double get_shake_duration() const;
		void set_shake_duration(const double p_shake_duration);*/
	};
}

#endif