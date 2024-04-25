// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_object.h

#ifndef TEMPLATE_OBJECT_H
#define TEMPLATE_OBJECT_H

#include "godot_cpp/core/object.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class TemplateObject : public Object
	{
		GDCLASS(TemplateObject, Object)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		TemplateObject();
		~TemplateObject();


	protected:
		static void _bind_methods();



		// GODOT Overrides
	public:


		// GODOT public
	private:


	public:


	protected:
	};
}

#endif // TEMPLATE_OBJECT_H