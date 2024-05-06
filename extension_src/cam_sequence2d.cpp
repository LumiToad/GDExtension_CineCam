// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// cam_squence2d.cpp

#include "cam_sequence2d.h"


#include "bind_utils.h"
#include "print_utils.h"

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
	ADD_METHOD_ARGS_BINDING(vcam2d_at, CamSequence2D, "index");

	ADD_GETSET_BINDING(get_current_idx, set_current_idx, current_idx, idx, CamSequence2D, Variant::INT);

	ADD_SIGNAL(MethodInfo(SIGNAL_VCAM_INDEX_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "index")));
}


void CamSequence2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


TypedArray<VirtualCam2D> CamSequence2D::get_vcams_in_children_internal() const
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


void CamSequence2D::_notification(int p_what)
{
	switch (p_what)
	{
		default:
			break;
		case NOTIFICATION_READY:
			vcams = get_vcams_in_children_internal();
			break;
		case NOTIFICATION_CHILD_ORDER_CHANGED:
			vcams = get_vcams_in_children_internal();
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
	if (idx > 0 && idx <= vcams.size())
	{
		return cast_to<VirtualCam2D>(vcams[idx]);
	}

	PrintUtils::idx_out_of_bounds(idx, vcams.size());

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

