// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_resource.h

#ifndef TEMPLATE_RESOURCE_H
#define TEMPLATE_RESOURCE_H

#include "godot_cpp/classes/resource.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class TemplateResource : public Resource
	{
		GDCLASS(TemplateResource, Resource)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		TemplateResource();
		~TemplateResource();


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

#endif // TEMPLATE_RESOURCE_H