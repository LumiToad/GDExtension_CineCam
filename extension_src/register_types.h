// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// register_types.h

#ifndef CINECAM_REGISTER_TYPES_H
#define CINECAM_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_cinecam_module(ModuleInitializationLevel p_level);
void uninitialize_cinecam_module(ModuleInitializationLevel p_level);

#endif // REGISTER_TYPES_H