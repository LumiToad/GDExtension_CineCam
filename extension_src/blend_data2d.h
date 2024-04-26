// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// blend_data2d.h

#ifndef BLEND_DATA2D_H
#define BLEND_DATA2D_H

#include "godot_cpp/classes/resource.hpp"

#include "gdclass_metadata.h"

namespace godot
{
	class BlendData2D : public Resource
	{
		GDCLASS(BlendData2D, Resource)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		BlendData2D();
		~BlendData2D();


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

#endif // BLEND_DATA2D_H