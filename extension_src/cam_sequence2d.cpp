// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_squence2d.cpp

#include "cam_sequence2d.h"

#include "bind_utils.h"

using namespace godot;

CamSequence2D::CamSequence2D()
{
	current_vcam_idx = 0;

	additional_description = "Sequence of VirtualCam2D.\nFor camera sequences. Put VirtualCam2D as children of this node.";
	initialize_internal();
}


CamSequence2D::~CamSequence2D()
{
}


void CamSequence2D::_bind_methods()
{
	ADD_METHOD_BINDING(get_vcam2d_array, CamSequence2D);
	ADD_METHOD_BINDING(current_vcam, CamSequence2D);

	ADD_GETSET_BINDING(get_current_idx, set_current_idx, current_idx, idx, CamSequence2D, Variant::INT);
}


void godot::CamSequence2D::_notification(int p_what)
{
	if (p_what == NOTIFICATION_CHILD_ORDER_CHANGED)
	{
		vcams = get_vcams_in_children_internal();
	}
}


void godot::CamSequence2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


TypedArray<VirtualCam2D> godot::CamSequence2D::get_vcams_in_children_internal() const
{
	TypedArray<VirtualCam2D> ret_val;

	TypedArray<Node> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		VirtualCam2D* vcam = Object::cast_to<VirtualCam2D>(children[i]);
		if (vcam != nullptr)
		{
			ret_val.append(vcam);
		}
	}

	return ret_val;
}


void CamSequence2D::_process(double delta)
{
}


void CamSequence2D::_ready()
{
	vcams = get_vcams_in_children_internal();
}


TypedArray<VirtualCam2D> godot::CamSequence2D::get_vcam2d_array() const
{
	return vcams;
}


VirtualCam2D* CamSequence2D::current_vcam() const
{
	return Object::cast_to<VirtualCam2D>(vcams[current_vcam_idx]);
}


void godot::CamSequence2D::set_current_idx(int idx)
{
	current_vcam_idx = idx;
}


int CamSequence2D::get_current_idx() const
{
	return current_vcam_idx;
}


