// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_squence3d.cpp

#include "cam_sequence3d.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/font.hpp"
#include "godot_cpp/classes/window.hpp"
#include "godot_cpp/classes/editor_node3d_gizmo.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CamSequence3D::CamSequence3D()
{
	current_vcam_idx = 0;
	debug_gizmos_color = Color("FC7F7F"); // godot_3d_red
	debug_gizmos_width = 1.0;

	additional_description = "Sequence of VirtualCam3D.\nFor camera sequences. Put VirtualCam3D as children of this node.";
	initialize_internal();
}


CamSequence3D::~CamSequence3D()
{
}


void CamSequence3D::_bind_methods()
{
	ADD_METHOD_ARGS_BINDING(_process_internal, CamSequence3D, "editor");
	ADD_METHOD_ARGS_BINDING(_debug_gizmos_drawing, CamSequence3D, "editor");

	ADD_METHOD_BINDING(get_vcam3d_array, CamSequence3D);
	ADD_METHOD_BINDING(current_vcam, CamSequence3D);
	ADD_METHOD_ARGS_BINDING(vcam3d_at, CamSequence3D, "index");
	ADD_METHOD_BINDING(get_current_idx, CamSequence3D);
	ADD_METHOD_ARGS_BINDING(set_current_idx, CamSequence3D, "idx");

	/*
	ClassDB::bind_method(D_METHOD("get_is_draw_debug_gizmos"), &CamSequence3D::get_is_draw_debug_gizmos);
	ClassDB::bind_method(D_METHOD("set_is_draw_debug_gizmos", "draw"), &CamSequence3D::set_is_draw_debug_gizmos);
	ClassDB::bind_method(D_METHOD("get_debug_gizmos_color"), &CamSequence3D::get_debug_gizmos_color);
	ClassDB::bind_method(D_METHOD("set_debug_gizmos_color", "color"), &CamSequence3D::set_debug_gizmos_color);
	ClassDB::bind_method(D_METHOD("get_debug_gizmos_width"), &CamSequence3D::get_debug_gizmos_width);
	ClassDB::bind_method(D_METHOD("set_debug_gizmos_width", "width"), &CamSequence3D::set_debug_gizmos_width);
	
	//ADD_PROPERTY(PropertyInfo(Variant::Variant::BOOL, "_draw_debug_gizmos"), "set_is_draw_debug_gizmos", "get_is_draw_debug_gizmos");
	//ADD_PROPERTY(PropertyInfo(Variant::Variant::COLOR, "_debug_gizmos_color"), "set_debug_gizmos_color", "get_debug_gizmos_color");
	//ADD_PROPERTY(PropertyInfo(Variant::Variant::FLOAT, "_debug_gizmos_width"), "set_debug_gizmos_width", "get_debug_gizmos_width");
	*/

	ADD_PROPERTY(PropertyInfo(Variant::Variant::INT, "current_idx"), "set_current_idx", "get_current_idx");

	ADD_SIGNAL(MethodInfo(SIGNAL_VCAM_INDEX_CHANGED, PropertyInfo(Variant::OBJECT, "vcam3d"), PropertyInfo(Variant::INT, "index")));
}


void CamSequence3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CamSequence3D::gather_vcams_in_children_internal()
{
	vcams.clear();

	TypedArray<Node> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		VirtualCam3D* vcam = cast_to<VirtualCam3D>(children[i]);
		if (vcam != nullptr)
		{
			vcams.append(vcam);
		}
	}
}


void CamSequence3D::_debug_gizmos_drawing(bool editor)
{
	if (!editor) return;
	if (!is_inside_tree()) return;
	if (!draw_debug_gizmos) return;
	if (vcams.size() < 2) return;

	for (int i = 0; i < vcams.size(); i++)
	{
		if (i + 1 >= vcams.size()) return;

		VirtualCam3D* a = cast_to<VirtualCam3D>(vcams[i]);
		VirtualCam3D* b = cast_to<VirtualCam3D>(vcams[i + 1]);

		Vector3 va = a->get_global_position();
		Vector3 vb = b->get_global_position();

		EditorNode3DGizmo gizmo;
		gizmo.add_lines(
			PackedVector3Array({ va, vb }),
			Ref<Material>()->create_placeholder(),
			false,
			debug_gizmos_color
		);


		//Ref<Font> font = get_window()->get_theme_default_font();
		/*draw_string(
			font,
			va + Vector2(25, -25),
			"#" + String::num(i),
			HORIZONTAL_ALIGNMENT_LEFT,
			-1.0,
			25,
			debug_gizmos_color
		);*/
	}
}


void CamSequence3D::_process(double delta)
{

}


void CamSequence3D::_process_internal(bool editor)
{
	double delta = get_process_delta_time();

	if (editor && draw_debug_gizmos)
	{
		update_gizmos();
	}
}


void CamSequence3D::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
	default:
		break;
	case NOTIFICATION_READY:
		gather_vcams_in_children_internal();
		set_process(true);
		break;
	case NOTIFICATION_CHILD_ORDER_CHANGED:
		gather_vcams_in_children_internal();
		update_gizmos();
		break;
	case NOTIFICATION_PROCESS:
		_process_internal(is_in_editor);
		_debug_gizmos_drawing(is_in_editor);
		break;
	}
}


TypedArray<VirtualCam3D> CamSequence3D::get_vcam3d_array() const
{
	return vcams;
}


VirtualCam3D* CamSequence3D::current_vcam() const
{
	return Object::cast_to<VirtualCam3D>(vcams[current_vcam_idx]);
}


void CamSequence3D::set_current_idx(int idx)
{
	current_vcam_idx = idx;
	emit_signal(SIGNAL_VCAM_INDEX_CHANGED, vcam3d_at(idx), idx);
}


int CamSequence3D::get_current_idx() const
{
	return current_vcam_idx;
}


VirtualCam3D* CamSequence3D::vcam3d_at(int idx) const
{
	if (idx >= 0 && idx <= vcams.size())
	{
		return cast_to<VirtualCam3D>(vcams[idx]);
	}

	PrintUtils::idx_out_of_bounds(__LINE__, __FILE__, idx, vcams.size());

	return nullptr;
}


VirtualCam3D* CamSequence3D::vcam3d_next() const
{
	return vcam3d_at(current_vcam_idx + 1);
}


VirtualCam3D* CamSequence3D::vcam3d_prev() const
{
	return vcam3d_at(current_vcam_idx - 1);
}


VirtualCam3D* CamSequence3D::vcam3d_first() const
{
	return vcam3d_at(0);
}


VirtualCam3D* CamSequence3D::vcam3d_last() const
{
	return vcam3d_at(vcams.size());
}

bool CamSequence3D::get_is_draw_debug_gizmos() const
{
	return draw_debug_gizmos;
}


void CamSequence3D::set_is_draw_debug_gizmos(bool draw)
{
	draw_debug_gizmos = draw;
	if (!draw_debug_gizmos)
	{
		update_gizmos();
	}
}


Color CamSequence3D::get_debug_gizmos_color() const
{
	return debug_gizmos_color;
}


void CamSequence3D::set_debug_gizmos_color(Color color)
{
	debug_gizmos_color = color;
}


double CamSequence3D::get_debug_gizmos_width() const
{
	return debug_gizmos_width;
}


void CamSequence3D::set_debug_gizmos_width(double width)
{
	debug_gizmos_width = width;
}

