// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// cinecam_editor_start.cpp

#include "cinecam_editor_start.h"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CineCamEditorStart::CineCamEditorStart()
{

}

CineCamEditorStart::~CineCamEditorStart()
{

}


void CineCamEditorStart::print_welcome()
{	
	PrintUtils::welcome_text(CINECAM_VERSION, CINECAM_URL);
}


void CineCamEditorStart::_notification(int p_what)
{
	if (p_what == NOTIFICATION_POSTINITIALIZE)
	{
		// start stuff goes here
		print_welcome();
	}
}


void CineCamEditorStart::_bind_methods()
{
	ADD_METHOD_BINDING(print_welcome, CineCamEditorStart);
}