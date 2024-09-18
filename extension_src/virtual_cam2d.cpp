// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// virtual_cam2d.cpp

#include "virtual_cam2d.h"

#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/classes/engine.hpp"

#include "cinecam2d.h"
#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

VirtualCam2D::VirtualCam2D()
{
	limit[SIDE_LEFT] = -10000000;
	limit[SIDE_TOP] = -10000000;
	limit[SIDE_RIGHT] = 10000000;
	limit[SIDE_BOTTOM] = 10000000;

	drag_margin[SIDE_LEFT] = 0.2;
	drag_margin[SIDE_TOP] = 0.2;
	drag_margin[SIDE_RIGHT] = 0.2;
	drag_margin[SIDE_BOTTOM] = 0.2;

	priority = 0;
	vcam_id = "";
	found_cam = nullptr;

	additional_description = "This virtual camera contains target information for CineCam2D.\n";
	initialize_internal();
}


VirtualCam2D::~VirtualCam2D()
{

}


void VirtualCam2D::_bind_methods()
{
	ADD_METHOD_BINDING(_register_to_cinecam2d, VirtualCam2D);

	ADD_METHOD_BINDING(get_vcam_id, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_vcam_id, VirtualCam2D, "id");
	ADD_METHOD_BINDING(get_priority, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_priority, VirtualCam2D, "priority");
	ADD_METHOD_BINDING(_get_blend_data, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(_set_blend_data, VirtualCam2D, "p_default_blend");

	ADD_PROPERTY(PropertyInfo(Variant::Variant::STRING, "vcam_id"), "set_vcam_id", "get_vcam_id");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::INT, "priority"), "set_priority", "get_priority");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "blend_data", PROPERTY_HINT_RESOURCE_TYPE, "BlendData2D"), "_set_blend_data", "_get_blend_data");

	ADD_METHOD_BINDING(get_offset, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_offset, VirtualCam2D, "offset");
	ADD_METHOD_BINDING(get_anchor_mode, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_anchor_mode, VirtualCam2D, "anchor_mode");
	ADD_METHOD_BINDING(is_ignoring_rotation, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_ignore_rotation, VirtualCam2D, "ignore");
	ADD_METHOD_BINDING(get_process_callback, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_process_callback, VirtualCam2D, "mode");
	ADD_METHOD_BINDING(is_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_enabled, VirtualCam2D, "enabled");
	ADD_METHOD_BINDING(get_limit_left, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_left, VirtualCam2D, "limit");
	ADD_METHOD_BINDING(get_limit_top, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_top, VirtualCam2D, "limit");
	ADD_METHOD_BINDING(get_limit_right, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_right, VirtualCam2D, "limit");
	ADD_METHOD_BINDING(get_limit_bottom, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_bottom, VirtualCam2D, "limit");
	ADD_METHOD_BINDING(is_limit_smoothing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_smoothing_enabled, VirtualCam2D, "limit_smoothing_enabled");
	ADD_METHOD_BINDING(is_drag_vertical_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_vertical_enabled, VirtualCam2D, "enabled");
	ADD_METHOD_BINDING(is_drag_horizontal_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_horizontal_enabled, VirtualCam2D, "enabled");
	ADD_METHOD_BINDING(get_drag_vertical_offset, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_vertical_offset, VirtualCam2D, "offset");
	ADD_METHOD_BINDING(get_drag_horizontal_offset, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_horizontal_offset, VirtualCam2D, "offset");
	ADD_METHOD_BINDING(get_drag_margin_left, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_margin_left, VirtualCam2D, "drag_margin");
	ADD_METHOD_BINDING(get_drag_margin_top, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_margin_top, VirtualCam2D, "drag_margin");
	ADD_METHOD_BINDING(get_drag_margin_right, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_margin_right, VirtualCam2D, "drag_margin");
	ADD_METHOD_BINDING(get_drag_margin_bottom, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_drag_margin_bottom, VirtualCam2D, "drag_margin");
	ADD_METHOD_BINDING(get_zoom, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_zoom, VirtualCam2D, "zoom");
	ADD_METHOD_BINDING(get_position_smoothing_speed, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_position_smoothing_speed, VirtualCam2D, "position_smoothing_speed");
	ADD_METHOD_BINDING(is_position_smoothing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_position_smoothing_enabled, VirtualCam2D, "enabled");
	ADD_METHOD_BINDING(is_rotation_smoothing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_rotation_smoothing_enabled, VirtualCam2D, "enabled");
	ADD_METHOD_BINDING(get_rotation_smoothing_speed, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_rotation_smoothing_speed, VirtualCam2D, "speed");
	ADD_METHOD_BINDING(is_screen_drawing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_screen_drawing_enabled, VirtualCam2D, "screen_drawing_enabled");
	ADD_METHOD_BINDING(is_limit_drawing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_limit_drawing_enabled, VirtualCam2D, "limit_drawing_enabled");
	ADD_METHOD_BINDING(is_margin_drawing_enabled, VirtualCam2D);
	ADD_METHOD_ARGS_BINDING(set_margin_drawing_enabled, VirtualCam2D, "margin_drawing_enabled");

	ADD_GROUP("Camera2D", "cam2d_");

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "cam2d_offset", PROPERTY_HINT_NONE, "suffix:px"), "set_offset", "get_offset");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_anchor_mode", PROPERTY_HINT_ENUM, "Fixed TopLeft,Drag Center"), "set_anchor_mode", "get_anchor_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_ignore_rotation"), "set_ignore_rotation", "is_ignoring_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_enabled"), "set_enabled", "is_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "cam2d_zoom", PROPERTY_HINT_LINK), "set_zoom", "get_zoom");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_process_callback", PROPERTY_HINT_ENUM, "Physics,Idle"), "set_process_callback", "get_process_callback");

	ADD_SUBGROUP("Limit", "cam_2d_limit_");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_limit_left", PROPERTY_HINT_NONE, "suffix:px"), "set_limit_left", "get_limit_left");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_limit_top", PROPERTY_HINT_NONE, "suffix:px"), "set_limit_top", "get_limit_top");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_limit_right", PROPERTY_HINT_NONE, "suffix:px"), "set_limit_right", "get_limit_right");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam2d_limit_bottom", PROPERTY_HINT_NONE, "suffix:px"), "set_limit_bottom", "get_limit_bottom");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_limit_smoothed"), "set_limit_smoothing_enabled", "is_limit_smoothing_enabled");
	
	ADD_SUBGROUP("Position Smoothing", "cam2d_position_smoothing_");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_position_smoothing_enabled"), "set_position_smoothing_enabled", "is_position_smoothing_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_position_smoothing_speed", PROPERTY_HINT_NONE, "suffix:px/s"), "set_position_smoothing_speed", "get_position_smoothing_speed");

	ADD_SUBGROUP("Rotation Smoothing", "cam2d_rotation_smoothing_");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_rotation_smoothing_enabled"), "set_rotation_smoothing_enabled", "is_rotation_smoothing_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_rotation_smoothing_speed"), "set_rotation_smoothing_speed", "get_rotation_smoothing_speed");

	ADD_SUBGROUP("Drag", "cam2d_drag_");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_drag_horizontal_enabled"), "set_drag_horizontal_enabled", "is_drag_horizontal_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_drag_vertical_enabled"), "set_drag_vertical_enabled", "is_drag_vertical_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_horizontal_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_drag_horizontal_offset", "get_drag_horizontal_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_vertical_offset", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_drag_vertical_offset", "get_drag_vertical_offset");
	
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_left_margin", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_drag_margin_left", "get_drag_margin_left");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_top_margin", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_drag_margin_top", "get_drag_margin_top");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_right_margin", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_drag_margin_right", "get_drag_margin_right");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam2d_drag_bottom_margin", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_drag_margin_bottom", "get_drag_margin_bottom");

	ADD_SUBGROUP("Editor", "cam2d_editor_");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_editor_draw_screen"), "set_screen_drawing_enabled", "is_screen_drawing_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_editor_draw_limits"), "set_limit_drawing_enabled", "is_limit_drawing_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam2d_editor_draw_drag_margin"), "set_margin_drawing_enabled", "is_margin_drawing_enabled");

	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITY_CHANGED, PropertyInfo(Variant::OBJECT, "vcam2d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(Camera2D::AnchorMode::ANCHOR_MODE_FIXED_TOP_LEFT);
	BIND_ENUM_CONSTANT(Camera2D::AnchorMode::ANCHOR_MODE_DRAG_CENTER);
	BIND_ENUM_CONSTANT(Camera2D::Camera2DProcessCallback::CAMERA2D_PROCESS_PHYSICS);
	BIND_ENUM_CONSTANT(Camera2D::Camera2DProcessCallback::CAMERA2D_PROCESS_IDLE);
}


void VirtualCam2D::_validate_property(PropertyInfo& p_property) const 
{
	if (!position_smoothing_enabled && p_property.name == StringName("cam2d_position_smoothing_speed")) 
	{
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if (!rotation_smoothing_enabled && p_property.name == StringName("cam2d_rotation_smoothing_speed"))
	{
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}


void VirtualCam2D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void VirtualCam2D::init_default_blend_data()
{
	blend_data->set_blend_name("VirtualCam2D blend data");
	blend_data->set_blend_by_value(2.0f);
	blend_data->set_blend_by(BlendData2D::BlendByType::DURATION);
	blend_data->set_ease(Tween::EASE_IN_OUT);
	blend_data->set_trans(Tween::TRANS_CUBIC);
	blend_data->set_callable_on_start(false);
	blend_data->set_callable_on_complete(false);
}


void VirtualCam2D::_register_to_cinecam2d()
{
	Camera2D* camera2d = get_viewport()->get_camera_2d();
	if (camera2d != nullptr && camera2d->has_method("_register_vcam_internal"))
	{
		found_cam = camera2d;
		((CineCam2D*)found_cam)->_register_vcam_internal(this);
	}
}


void VirtualCam2D::_remove_from_cinecam2d()
{
	if (found_cam == nullptr) return;
	if (scene_tree == nullptr) return;
	CineCam2D* cine_cam = cast_to<CineCam2D>(found_cam);
	if (cine_cam == nullptr) return;
	cine_cam->_remove_vcam_internal(this);
}


void VirtualCam2D::_notification(int p_what)
{
	bool is_in_editor = Engine::get_singleton()->is_editor_hint();

	switch (p_what)
	{
		default:
			break;
		case NOTIFICATION_ENTER_TREE:
			if (!is_in_editor)
			{
				scene_tree = get_tree();
			}
			break;
		case NOTIFICATION_READY:
			if (!is_in_editor)
			{
				_register_to_cinecam2d();
			}
			break;
		case NOTIFICATION_PREDELETE:
		case NOTIFICATION_EXIT_TREE:
			if (!is_in_editor)
			{
				_remove_from_cinecam2d();
			}
			break;
		}
}


void VirtualCam2D::set_vcam_id(String id)
{
	vcam_id = id;
}


String VirtualCam2D::get_vcam_id() const
{
	return vcam_id;
}



void VirtualCam2D::set_priority(int prio)
{
	priority = prio;
	emit_signal(SIGNAL_PRIORITY_CHANGED, this, priority);
}


int VirtualCam2D::get_priority() const
{
	return priority;
}


void VirtualCam2D::set_offset(const Vector2& p_offset)
{
	offset = p_offset;
}


Vector2 VirtualCam2D::get_offset() const
{
	return offset;
}


void VirtualCam2D::set_anchor_mode(Camera2D::AnchorMode p_anchor_mode)
{
	anchor_mode = p_anchor_mode;
}


Camera2D::AnchorMode VirtualCam2D::get_anchor_mode() const
{
	return anchor_mode;
}


void VirtualCam2D::set_ignore_rotation(bool p_ignore)
{
	ingore_rotation = p_ignore;
}


bool VirtualCam2D::is_ignoring_rotation() const
{
	return ingore_rotation;
}


void VirtualCam2D::set_enabled(bool p_enabled)
{
	enabled = p_enabled;
}


bool VirtualCam2D::is_enabled() const
{
	return enabled;
}


void VirtualCam2D::set_zoom(const Vector2& p_zoom)
{
	zoom = p_zoom;
}


Vector2 VirtualCam2D::get_zoom() const
{
	return zoom;
}


void VirtualCam2D::set_limit_left(int p_limit)
{
	set_limit(Side::SIDE_LEFT, p_limit);
}


int VirtualCam2D::get_limit_left() const
{
	return get_limit(Side::SIDE_LEFT);
}


void VirtualCam2D::set_limit_top(int p_limit)
{
	set_limit(Side::SIDE_TOP, p_limit);
}


int VirtualCam2D::get_limit_top() const
{
	return get_limit(Side::SIDE_TOP);
}


void VirtualCam2D::set_limit_right(int p_limit)
{
	set_limit(Side::SIDE_RIGHT, p_limit);
}


int VirtualCam2D::get_limit_right() const
{
	return get_limit(Side::SIDE_RIGHT);
}


void VirtualCam2D::set_limit_bottom(int p_limit)
{
	set_limit(Side::SIDE_BOTTOM, p_limit);
}


int VirtualCam2D::get_limit_bottom() const
{
	return get_limit(Side::SIDE_BOTTOM);
}


void VirtualCam2D::set_limit(Side p_side, int p_limit)
{
	ERR_FAIL_INDEX((int)p_side, 4);
	limit[p_side] = p_limit;
}


int VirtualCam2D::get_limit(Side p_side) const
{
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	return limit[p_side];
}


void VirtualCam2D::set_process_callback(Camera2D::Camera2DProcessCallback p_mode)
{
	process_callback = p_mode;
}


Camera2D::Camera2DProcessCallback VirtualCam2D::get_process_callback() const
{
	return process_callback;
}


void VirtualCam2D::set_limit_smoothing_enabled(bool enable)
{
	limit_smoothing_enabled = enable;
	notify_property_list_changed();
}


bool VirtualCam2D::is_limit_smoothing_enabled() const
{
	return limit_smoothing_enabled;
}


void VirtualCam2D::set_position_smoothing_enabled(bool p_enabled)
{
	position_smoothing_enabled = p_enabled;
	notify_property_list_changed();
}


bool VirtualCam2D::is_position_smoothing_enabled() const
{
	return position_smoothing_enabled;
}


void VirtualCam2D::set_position_smoothing_speed(real_t p_speed)
{
	position_smoothing_speed = p_speed;
}


real_t VirtualCam2D::get_position_smoothing_speed() const
{
	return position_smoothing_speed;
}


void VirtualCam2D::set_rotation_smoothing_speed(real_t p_speed)
{
	rotation_smoothing_speed = p_speed;
}


real_t VirtualCam2D::get_rotation_smoothing_speed() const
{
	return rotation_smoothing_speed;
}


void VirtualCam2D::set_rotation_smoothing_enabled(bool p_enabled)
{
	rotation_smoothing_enabled = p_enabled;
	notify_property_list_changed();
}


bool VirtualCam2D::is_rotation_smoothing_enabled() const
{
	return rotation_smoothing_enabled;
}


void VirtualCam2D::set_drag_horizontal_enabled(bool p_enabled)
{
	drag_horizontal_enabled = p_enabled;
}


bool VirtualCam2D::is_drag_horizontal_enabled() const
{
	return drag_horizontal_enabled;
}


void VirtualCam2D::set_drag_vertical_enabled(bool p_enabled)
{
	drag_vertical_enabled = p_enabled;
}


bool VirtualCam2D::is_drag_vertical_enabled() const
{
	return drag_vertical_enabled;
}


void VirtualCam2D::set_drag_margin_left(real_t p_drag_margin)
{
	set_drag_margin(Side::SIDE_LEFT, p_drag_margin);
}


real_t VirtualCam2D::get_drag_margin_left() const
{
	return get_drag_margin(Side::SIDE_LEFT);
}


void VirtualCam2D::set_drag_margin_top(real_t p_drag_margin)
{
	set_drag_margin(Side::SIDE_TOP, p_drag_margin);
}


real_t VirtualCam2D::get_drag_margin_top() const
{
	return get_drag_margin(Side::SIDE_TOP);
}


void VirtualCam2D::set_drag_margin_right(real_t p_drag_margin)
{
	set_drag_margin(Side::SIDE_RIGHT, p_drag_margin);
}


real_t VirtualCam2D::get_drag_margin_right() const
{
	return get_drag_margin(Side::SIDE_RIGHT);
}


void VirtualCam2D::set_drag_margin_bottom(real_t p_drag_margin)
{
	set_drag_margin(Side::SIDE_BOTTOM, p_drag_margin);
}


real_t VirtualCam2D::get_drag_margin_bottom() const
{
	return get_drag_margin(Side::SIDE_BOTTOM);
}


void VirtualCam2D::set_drag_margin(Side p_side, real_t p_drag_margin)
{
	ERR_FAIL_INDEX((int)p_side, 4);
	drag_margin[p_side] = p_drag_margin;
	queue_redraw(); //remove???
}


real_t VirtualCam2D::get_drag_margin(Side p_side) const
{
	ERR_FAIL_INDEX_V((int)p_side, 4, 0);
	return drag_margin[p_side];
}


void VirtualCam2D::set_drag_horizontal_offset(real_t p_offset)
{
	drag_horizontal_offset = p_offset;
}


real_t VirtualCam2D::get_drag_horizontal_offset() const
{
	return drag_horizontal_offset;
}


void VirtualCam2D::set_drag_vertical_offset(real_t p_offset)
{
	drag_vertical_offset = p_offset;
}


real_t VirtualCam2D::get_drag_vertical_offset() const
{
	return drag_vertical_offset;
}

void VirtualCam2D::set_screen_drawing_enabled(bool enable)
{
	screen_drawing_enabled = enable;
}


bool VirtualCam2D::is_screen_drawing_enabled() const
{
	return screen_drawing_enabled;
}


void VirtualCam2D::set_limit_drawing_enabled(bool enable)
{
	limit_drawing_enabled = enable;
}


bool VirtualCam2D::is_limit_drawing_enabled() const
{
	return limit_drawing_enabled;
}


void VirtualCam2D::set_margin_drawing_enabled(bool enable)
{
	margin_drawing_enabled = enable;
}


bool VirtualCam2D::is_margin_drawing_enabled() const
{
	return margin_drawing_enabled;
}

Ref<BlendData2D> VirtualCam2D::_get_blend_data() const
{
	return blend_data;
}

void VirtualCam2D::_set_blend_data(Ref<BlendData2D> blend)
{
	blend_data = blend;
	if (blend.ptr() == nullptr) return;

	if (blend->_is_default_blend())
	{
		init_default_blend_data();
	}
}