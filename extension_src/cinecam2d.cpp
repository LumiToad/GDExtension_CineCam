#include "cinecam2d.h"

using namespace godot;

CineCam2D::CineCam2D()
{
	shake_strength = 0.0;
	shake_duration = 0.0;
}

CineCam2D::~CineCam2D()
{
}

void CineCam2D::_bind_methods()
{
	//ADD_BINDING(get_shake_strength, set_shake_strength, shake_strength, p_shake_strength, CineCam2D, FLOAT)
	//ADD_BINDING(get_shake_duration, set_shake_duration, shake_duration, p_shake_duration, CineCam2D, FLOAT)

	ADD_METHOD_BINDING(shake_cam, CineCam2D)
}

void CineCam2D::shake_cam(double p_shake_strength, double p_shake_duration)
{
	shake_strength = p_shake_strength;
	shake_duration = p_shake_duration;
}

void CineCam2D::shake_cam_internal(double delta)
{
	if (shake_duration <= 0.0) return;

	RandomNumberGenerator rng;
	rng.randomize();

	shake_duration -= delta;
	Vector2 shake_vector = Vector2(rng.randf_range(-shake_strength, shake_strength), rng.randf_range(-shake_strength, shake_strength));

	global_translate(shake_vector);
}

void CineCam2D::_process(double delta)
{
	shake_cam_internal(delta);
}

//Get Set

/*
double CineCam2D::get_shake_strength() const
{
	return shake_strength;
}

void CineCam2D::set_shake_strength(const double p_shake_strength)
{
	shake_strength = p_shake_strength;
}

double CineCam2D::get_shake_duration() const
{
	return shake_duration;
}

void CineCam2D::set_shake_duration(const double p_shake_duration)
{
	shake_duration = p_shake_duration;
}
*/
