// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt. (https://github.com/lumitoad/)

// welcome.h

#ifndef WELCOME_H
#define WELCOME_H
#define TOOLS_ENABLED

#include "godot_cpp/classes/object.hpp"

#include "print_utils.h"

namespace godot
{
	class Welcome : public Object
	{
		GDCLASS(Welcome, Object)

	public:
		Welcome();
		~Welcome();

		void print_welcome();

	protected:
		static void _bind_methods();
	};
}

#endif // WELCOME_H