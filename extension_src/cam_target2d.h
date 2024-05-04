// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_target2d.h

#ifndef CAM_TARGET2D_H
#define CAM_TARGET2D_H

#include "godot_cpp/classes/node2d.hpp"

#include "gdclass_metadata.h"
#include "blend_data2d.h"

namespace godot
{
	class CamTarget2D : public Node2D
	{
		GDCLASS(CamTarget2D, Node2D)

			// Internal
	private:
		godot::String additional_description;
		GDCLASS_Metadata meta;

		void initialize_internal();


	public:
		CamTarget2D();
		~CamTarget2D();
		

	protected:
		static void _bind_methods();



		// GODOT Overrides
	public:
		void _process(double delta) override;
		void _notification(int p_what);


		// GODOT public
	private:
		Ref<BlendData2D> blend_data;
		Vector2 target_offset;

	public:
		Ref<BlendData2D> get_blend_data() const;
		void set_blend_data(Ref<BlendData2D> blend_data);

		Vector2 get_target_offset() const;
		void set_target_offset(Vector2 offset);

	protected:
	};
}

#endif // CAM_TARGET2D_H