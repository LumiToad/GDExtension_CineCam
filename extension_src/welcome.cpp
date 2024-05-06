// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// welcome.cpp

#include "welcome.h"
#include "bind_utils.h"
#include "godot_cpp/core/class_db.hpp"

using namespace godot;

Welcome::Welcome()
{

}

Welcome::~Welcome()
{

}


void Welcome::print_welcome()
{	
	PrintUtils::welcome_text();
}


void Welcome::_bind_methods()
{
	ADD_METHOD_BINDING(print_welcome, Welcome);
}