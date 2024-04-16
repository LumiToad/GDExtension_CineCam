// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// register_types.cpp

#include "register_types.h"

#include "gdextension_interface.h"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"
		 
#include "gdexample.h"
#include "cinecam2d.h"
#include "template_class.h"

using namespace godot;

void initialize_cinecam_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;

	GDREGISTER_CLASS(CineCam2D)
	GDREGISTER_CLASS(GDExample)
	//GDREGISTER_CLASS(Template_Class)
}

void uninitialize_cinecam_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}

extern "C"
{
	// Initialization.
	GDExtensionBool GDE_EXPORT cinecam_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) 
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_cinecam_module);
		init_obj.register_terminator(uninitialize_cinecam_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}