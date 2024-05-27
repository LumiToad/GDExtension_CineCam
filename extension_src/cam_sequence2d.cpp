// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_squence2d.cpp

#include "cam_sequence2d.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/font.hpp"
#include "godot_cpp/classes/window.hpp"

#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

CamSequence2D::CamSequence2D()
{
	current_vcam_idx = 0;
	debug_lines_color = Color("8DA5F3"); // godot_3d_blue
	debug_lines_width = 1.0;

	additional_description = "Sequence of VirtualCam2D.\nFor camera sequences. Put VirtualCam2D as children of this node.";
	initialize_internal();
}


CamSequence2D::~CamSequence2D()
{
}


void CamSequence2D::_bind_methods()
{
	ADD_METHOD_ARGS_BINDING(_process_internal, CamSequence2D, "editor");
	ADD_METHOD_ARGS_BINDING(_debug_lines_drawing, CamSequence2D, "editor");

	ADD_METHOD_BINDING(get_vcam2d_array, CamSequence2D);
	ADD_METHOD_BINDING(current_vcam, CamSequence2D);
	ADD_METHOD_ARGS_BINDING(vcam2d_at, CamSequence2D, "index");
	ADD_METHOD_BINDING(get_current_idx, CamSequence2D);
	ADD_METHOD_ARGS_BINDING(set_current_idx, CamSequence2D, "idx");
	ADD_METHOD_BINDING(get_is_draw_debug_lines, CamSequence2D);
	ADD_METHOD_ARGS_BINDING(set_is_draw_debug_lines, CamSequence2D, "draw");
	ADD_METHOD_BINDING(get_debug_lines_color, CamSequence2D);
	ADD_METHOD_ARGS_BINDING(set_debug_lines_color, CamSequence2D, "color");
	ADD_METHOD_BINDING(get_debug_lines_width, CamSequence2D);
	ADD_METHOD_ARGS_BINDING(set_debug_lines_width, CamSequence2D, "width");

	ADD_PROPERTY(PropertyInfo(Variant::Variant::INT, "current_idx"), "set_current_idx", "get_current_idx");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::BOOL, "_draw_debug_lines"), "set_is_draw_debug_lines", "get_is_draw_debug_lines");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::COLOR, "_debug_lines_color"), "set_debug_lines_color", "get_debug_lines_color");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::FLOAT, "_debug_lines_width"), "set_debug_lines_width", "get_debug_lines_width");
	// ADD_PROPERTY(PropertyInfo(Variant::INT, "follow_mode", PROPERTY_HINT_ENUM, "OFF,PRIO,PRIO_ONESHOT,PRIO_BLEND,TARGET,TARGET_BLEND"), "set_follow_mode", "get_follow_mode");

	ADD_SIGNAL(MethodInfo(SIGNAL_VCAM_INDEX_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "index")));
}


void CamSequence2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void CamSequence2D::gather_vcams_in_children_internal()
{
	vcams.clear();

	TypedArray<Node> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		VirtualCam2D* vcam = cast_to<VirtualCam2D>(children[i]);
		if (vcam != nullptr)
		{
			vcams.append(vcam);
		}
	}
}


void CamSequence2D::_debug_lines_drawing(bool editor)
{
	if (!editor) return;
	if (!is_inside_tree()) return;
	if (!draw_debug_lines) return;
	if (vcams.size() < 2) return;

	for (int i = 0; i < vcams.size(); i++)
	{
		if (i + 1 >= vcams.size()) return;

		VirtualCam2D* a = cast_to<VirtualCam2D>(vcams[i]);
		VirtualCam2D* b = cast_to<VirtualCam2D>(vcams[i + 1]);

		Vector2 va = a->get_global_position();
		Vector2 vb = b->get_global_position();

		draw_dashed_line(va, vb, debug_lines_color, debug_lines_width);
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


void CamSequence2D::_process(double delta)
{

}


void CamSequence2D::_process_internal(bool editor)
{
	double delta = get_process_delta_time();

	if (editor && draw_debug_lines)
	{
		queue_redraw();
	}
}


void CamSequence2D::_notification(int p_what)
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


TypedArray<VirtualCam2D> CamSequence2D::get_vcam2d_array() const
{
	return vcams;
}


VirtualCam2D* CamSequence2D::current_vcam() const
{
	return Object::cast_to<VirtualCam2D>(vcams[current_vcam_idx]);
}


void CamSequence2D::set_current_idx(int idx)
{
	current_vcam_idx = idx;
	emit_signal(SIGNAL_VCAM_INDEX_CHANGED, vcam2d_at(idx), idx);
}


int CamSequence2D::get_current_idx() const
{
	return current_vcam_idx;
}


VirtualCam2D* CamSequence2D::vcam2d_at(int idx) const
{
	if (idx >= 0 && idx <= vcams.size())
	{
		return cast_to<VirtualCam2D>(vcams[idx]);
	}

	PrintUtils::idx_out_of_bounds(__LINE__, __FILE__, idx, vcams.size());

	return nullptr;
}


VirtualCam2D* CamSequence2D::vcam2d_next() const
{
	return vcam2d_at(current_vcam_idx + 1);
}


VirtualCam2D* CamSequence2D::vcam2d_prev() const
{
	return vcam2d_at(current_vcam_idx - 1);
}


VirtualCam2D* CamSequence2D::vcam2d_first() const
{
	return vcam2d_at(0);
}


VirtualCam2D* CamSequence2D::vcam2d_last() const
{
	return vcam2d_at(vcams.size());
}

bool CamSequence2D::get_is_draw_debug_lines() const
{
	return draw_debug_lines;
}


void CamSequence2D::set_is_draw_debug_lines(bool draw)
{
	draw_debug_lines = draw;
	if (!draw_debug_lines)
	{
		queue_redraw();
	}
}


Color CamSequence2D::get_debug_lines_color() const
{
	return debug_lines_color;
}


void CamSequence2D::set_debug_lines_color(Color color)
{
	debug_lines_color = color;
}


double CamSequence2D::get_debug_lines_width() const
{
	return debug_lines_width;
}


void CamSequence2D::set_debug_lines_width(double width)
{
	debug_lines_width = width;
}

