// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// register_types.cpp

#include "register_types.h"

#include "gdextension_interface.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/defs.hpp"
#include "godot_cpp/godot.hpp"

#include "blend_data2d.h"
#include "virtual_cam2d.h"
#include "cam_sequence2d.h"
#include "cinecam2d.h"
#include "cam_target2d.h"

#include "blend_data3d.h"
#include "virtual_cam3d.h"
#include "cam_sequence3d.h"
#include "cinecam3d.h"
#include "cam_target3d.h"

#include "cinecam_editor_start.h"

using namespace godot;

void initialize_cinecam_module(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{	
		GDREGISTER_CLASS(CamTarget2D)
		GDREGISTER_CLASS(BlendData2D)
		GDREGISTER_CLASS(VirtualCam2D)
		GDREGISTER_CLASS(CamSequence2D)
		GDREGISTER_CLASS(CineCam2D)

		GDREGISTER_CLASS(CamTarget3D)
		GDREGISTER_CLASS(BlendData3D)
		GDREGISTER_CLASS(VirtualCam3D)
		GDREGISTER_CLASS(CamSequence3D)
		GDREGISTER_CLASS(CineCam3D)
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		GDREGISTER_CLASS(CineCamEditorStart)
		EditorPlugins::add_plugin_class(StringName("CineCamEditorStart"));
	}
}


void uninitialize_cinecam_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}


extern "C"
{
	// Initialization.
	GDExtensionBool GDE_EXPORT cinecam_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) 
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_cinecam_module);
		init_obj.register_terminator(uninitialize_cinecam_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
