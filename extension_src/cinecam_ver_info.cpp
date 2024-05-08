// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// welcome.cpp

#include "cinecam_ver_info.h"
#include "bind_utils.h"
#include "godot_cpp/core/class_db.hpp"

using namespace godot;

CineCamVerInfo::CineCamVerInfo()
{

}

CineCamVerInfo::~CineCamVerInfo()
{

}


void CineCamVerInfo::print_welcome()
{	
	PrintUtils::welcome_text();
}


void CineCamVerInfo::_notification(int p_what)
{
	if (p_what == NOTIFICATION_POSTINITIALIZE)
	{
		print_welcome();
		EditorPlugins::remove_plugin_class(StringName("CineCamVerInfo"));
	}
}


void CineCamVerInfo::_bind_methods()
{
	ADD_METHOD_BINDING(print_welcome, CineCamVerInfo);
}