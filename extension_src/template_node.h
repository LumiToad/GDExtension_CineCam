// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// template_node.h

#ifndef TEMPLATE_NODE_H
#define TEMPLATE_NODE_H

#include "godot_cpp/classes/node.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class TemplateNode : public Node
	{
		GDCLASS(TemplateNode, Node)

	// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;
	
		void initialize_internal();


	public:
		TemplateNode();
		~TemplateNode();


	protected:
		static void _bind_methods();



	// GODOT Overrides
	public:
		void _notification(int p_what);
		void _ready_internal(bool editor);
		void _process_internal(bool editor);


	// GODOT public
	private:
		

	public:


	protected:
	};
}

#endif // TEMPLATE_NODE_H