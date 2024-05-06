// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_squence3d.cpp

#include "cam_sequence3d.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/font.hpp"
#include "godot_cpp/classes/window.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CamSequence3D::CamSequence3D()
{
	current_vcam_idx = 0;
	debug_lines_color = Color("478cbf"); // godot_blue
	debug_lines_width = 1.0;

	additional_description = "Sequence of VirtualCam3D.\nFor camera sequences. Put VirtualCam3D as children of this node.";
	initialize_internal();
}


CamSequence3D::~CamSequence3D()
{
}


void CamSequence3D::_bind_methods()
{
	ADD_METHOD_ARGS_BINDING(_process_internal, CamSequence3D, "editor");
	ADD_METHOD_ARGS_BINDING(_debug_lines_drawing, CamSequence3D, "editor");

	ADD_METHOD_BINDING(get_vcam3d_array, CamSequence3D);
	ADD_METHOD_BINDING(current_vcam, CamSequence3D);
	ADD_METHOD_ARGS_BINDING(vcam3d_at, CamSequence3D, "index");

	ADD_GETSET_BINDING(get_current_idx, set_current_idx, current_idx, idx, CamSequence3D, Variant::INT);
	ADD_GETSET_BINDING(get_is_draw_debug_lines, set_is_draw_debug_lines, _draw_debug_lines, draw, CamSequence3D, Variant::BOOL);
	ADD_GETSET_BINDING(get_debug_lines_color, set_debug_lines_color, _debug_lines_color, color, CamSequence3D, Variant::COLOR);
	ADD_GETSET_BINDING(get_debug_lines_width, set_debug_lines_width, _debug_lines_width, width, CamSequence3D, Variant::FLOAT);



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


void CamSequence3D::_debug_lines_drawing(bool editor)
{
	if (!editor) return;
	if (!is_inside_tree()) return;
	if (!draw_debug_lines) return;
	if (vcams.size() < 2) return;

	for (int i = 0; i < vcams.size(); i++)
	{
		if (i + 1 >= vcams.size()) return;

		VirtualCam3D* a = cast_to<VirtualCam3D>(vcams[i]);
		VirtualCam3D* b = cast_to<VirtualCam3D>(vcams[i + 1]);

		Vector2 va = a->get_global_position();
		Vector2 vb = b->get_global_position();

		draw_line(va, vb, debug_lines_color, debug_lines_width);
		Ref<Font> font = get_window()->get_theme_default_font();
		draw_string(
			font,
			va + Vector2(25, -25),
			"#" + String::num(i),
			HORIZONTAL_ALIGNMENT_LEFT,
			-1.0,
			25,
			debug_lines_color
		);
	}
}


void CamSequence3D::_process_internal(bool editor)
{
	double delta = get_process_delta_time();

	if (editor && draw_debug_lines)
	{
		queue_redraw();
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
		queue_redraw();
		break;
	case NOTIFICATION_PROCESS:
		_process_internal(is_in_editor);
		break;
	case NOTIFICATION_DRAW:
		_debug_lines_drawing(is_in_editor);
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
	if (idx > 0 && idx <= vcams.size())
	{
		return cast_to<VirtualCam3D>(vcams[idx]);
	}

	PrintUtils::idx_out_of_bounds(idx, vcams.size());

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

bool CamSequence3D::get_is_draw_debug_lines() const
{
	return draw_debug_lines;
}


void CamSequence3D::set_is_draw_debug_lines(bool draw)
{
	draw_debug_lines = draw;
	if (!draw_debug_lines)
	{
		queue_redraw();
	}
}


Color CamSequence3D::get_debug_lines_color() const
{
	return debug_lines_color;
}


void CamSequence3D::set_debug_lines_color(Color color)
{
	debug_lines_color = color;
}


double CamSequence3D::get_debug_lines_width() const
{
	return debug_lines_width;
}


void CamSequence3D::set_debug_lines_width(double width)
{
	debug_lines_width = width;
}

