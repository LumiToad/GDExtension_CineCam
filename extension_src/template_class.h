// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_class.h

#ifndef TEMPLATE_CLASS_H
#define TEMPLATE_CLASS_H

#include "godot_cpp/classes/node.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class Template_Class : public Node
	{
		GDCLASS(Template_Class, Node)

	// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;
	
		void initialize_internal();


	public:
		Template_Class();
		~Template_Class();


	protected:
		static void _bind_methods();



	// GODOT Overrides
	public:
		void _process(double delta) override;


	// GODOT public
	private:
		

	public:


	protected:
	};
}

#endif // TEMPLATE_CLASS_H