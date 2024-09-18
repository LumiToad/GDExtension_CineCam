// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)

// virtual_cam3d.cpp

#include "virtual_cam3d.h"

#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/collision_object3d.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/variant/projection.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/rendering_server.hpp"
#include "godot_cpp/classes/physics_server3d.hpp"
#include "godot_cpp/variant/array.hpp"

#include "cinecam3d.h"
#include "bind_utils.h"
#include "print_utils.h"

using namespace godot;

VirtualCam3D::VirtualCam3D()
{
	priority = 0;
	vcam_id = "";
	found_cam = nullptr;

	set_perspective(75.0, 0.05, 4000.0);
	set_notify_transform(true);
	set_disable_scale(true);

	additional_description = "This virtual camera contains target information for CineCam3D.\n";
	initialize_internal();
}


VirtualCam3D::~VirtualCam3D()
{
	if (pyramid_shape.is_valid())
	{
		ERR_FAIL_NULL(PhysicsServer3D::get_singleton());
		// PhysicsServer3D::get_singleton()->free(pyramid_shape);
	}
}


void VirtualCam3D::_bind_methods()
{
	ADD_METHOD_BINDING(_register_to_cinecam3d, VirtualCam3D);

	ADD_METHOD_BINDING(get_vcam_id, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_vcam_id, VirtualCam3D, "id");
	ADD_METHOD_BINDING(get_priority, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_priority, VirtualCam3D, "priority");
	ADD_METHOD_BINDING(_get_blend_data, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(_set_blend_data, VirtualCam3D, "p_default_blend");

	ADD_PROPERTY(PropertyInfo(Variant::Variant::STRING, "vcam_id"), "set_vcam_id", "get_vcam_id");
	ADD_PROPERTY(PropertyInfo(Variant::Variant::INT, "priority"), "set_priority", "get_priority");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "blend_data", PROPERTY_HINT_RESOURCE_TYPE, "BlendData3D"), "_set_blend_data", "_get_blend_data");

	ADD_METHOD_ARGS_BINDING(is_position_behind, VirtualCam3D, "world_point");
	ADD_METHOD_ARGS_BINDING(set_perspective, VirtualCam3D, VA_LIST("fov", "z_near", "z_far"));
	ADD_METHOD_ARGS_BINDING(set_orthogonal, VirtualCam3D, VA_LIST("size", "z_near", "z_far"));
	ADD_METHOD_ARGS_BINDING(set_frustum, VirtualCam3D, VA_LIST("size", "offset", "z_near", "z_far"));
	ADD_METHOD_ARGS_BINDING(set_current, VirtualCam3D, "enabled");
	ADD_METHOD_BINDING(is_current, VirtualCam3D);
	ADD_METHOD_BINDING(get_camera_transform, VirtualCam3D);
	ADD_METHOD_BINDING(get_camera_projection, VirtualCam3D);
	ADD_METHOD_BINDING(get_frustum_offset, VirtualCam3D);
	ADD_METHOD_BINDING(get_size, VirtualCam3D);
	ADD_METHOD_BINDING(get_far, VirtualCam3D);
	ADD_METHOD_BINDING(get_near, VirtualCam3D);
	ADD_METHOD_BINDING(get_fov, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_fov, VirtualCam3D, "fov");
	ADD_METHOD_ARGS_BINDING(set_frustum_offset, VirtualCam3D, "offset");
	ADD_METHOD_ARGS_BINDING(set_size, VirtualCam3D, "size");
	ADD_METHOD_ARGS_BINDING(set_far, VirtualCam3D, "far");
	ADD_METHOD_ARGS_BINDING(set_near, VirtualCam3D, "set_near");
	ADD_METHOD_BINDING(get_projection, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_projection, VirtualCam3D, "mode");
	ADD_METHOD_ARGS_BINDING(set_h_offset, VirtualCam3D, "offset");
	ADD_METHOD_BINDING(get_h_offset, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_v_offset, VirtualCam3D, "offset");
	ADD_METHOD_BINDING(get_v_offset, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_cull_mask, VirtualCam3D, "mask");
	ADD_METHOD_BINDING(get_cull_mask, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_environment, VirtualCam3D, "env");
	ADD_METHOD_BINDING(get_environment, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_attributes, VirtualCam3D, "env");
	ADD_METHOD_BINDING(get_attributes, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_keep_aspect_mode, VirtualCam3D, "mode");
	ADD_METHOD_BINDING(get_keep_aspect_mode, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_doppler_tracking, VirtualCam3D, "mode");
	ADD_METHOD_BINDING(get_doppler_tracking, VirtualCam3D);
	ADD_METHOD_BINDING(get_frustum, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(is_position_in_frustum, VirtualCam3D, "world_point");
	ADD_METHOD_BINDING(get_pyramid_shape_rid, VirtualCam3D);
	ADD_METHOD_ARGS_BINDING(set_cull_mask_value, VirtualCam3D, VA_LIST("layer_number", "value"));
	ADD_METHOD_ARGS_BINDING(get_cull_mask_value, VirtualCam3D, "layer_number");

	ADD_GROUP("Camera3D", "cam3d_");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam3d_keep_aspect", PROPERTY_HINT_ENUM, "Keep Width,Keep Height"), "set_keep_aspect_mode", "get_keep_aspect_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam3d_cull_mask", PROPERTY_HINT_LAYERS_3D_RENDER), "set_cull_mask", "get_cull_mask");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cam3d_environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment"), "set_environment", "get_environment");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cam3d_attributes", PROPERTY_HINT_RESOURCE_TYPE, "CameraAttributesPractical,CameraAttributesPhysical"), "set_attributes", "get_attributes");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_h_offset", PROPERTY_HINT_NONE, "suffix:m"), "set_h_offset", "get_h_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_v_offset", PROPERTY_HINT_NONE, "suffix:m"), "set_v_offset", "get_v_offset");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam3d_doppler_tracking", PROPERTY_HINT_ENUM, "Disabled,Idle,Physics"), "set_doppler_tracking", "get_doppler_tracking");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cam3d_projection", PROPERTY_HINT_ENUM, "Perspective,Orthogonal,Frustum"), "set_projection", "get_projection");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cam3d_current"), "set_current", "is_current");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_fov", PROPERTY_HINT_RANGE, "1,179,0.1,degrees"), "set_fov", "get_fov");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_size", PROPERTY_HINT_RANGE, "0.001,100,0.001,or_greater,suffix:m"), "set_size", "get_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "cam3d_frustum_offset", PROPERTY_HINT_NONE, "suffix:m"), "set_frustum_offset", "get_frustum_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_near", PROPERTY_HINT_RANGE, "0.001,10,0.001,or_greater,exp,suffix:m"), "set_near", "get_near");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cam3d_far", PROPERTY_HINT_RANGE, "0.01,4000,0.01,or_greater,exp,suffix:m"), "set_far", "get_far");

	ADD_SIGNAL(MethodInfo(SIGNAL_PRIORITY_CHANGED, PropertyInfo(Variant::OBJECT, "vcam3d"), PropertyInfo(Variant::INT, "priority")));

	BIND_ENUM_CONSTANT(Camera3D::ProjectionType::PROJECTION_PERSPECTIVE);
	BIND_ENUM_CONSTANT(Camera3D::ProjectionType::PROJECTION_ORTHOGONAL);
	BIND_ENUM_CONSTANT(Camera3D::ProjectionType::PROJECTION_FRUSTUM);

	BIND_ENUM_CONSTANT(Camera3D::KeepAspect::KEEP_WIDTH);
	BIND_ENUM_CONSTANT(Camera3D::KeepAspect::KEEP_HEIGHT);

	BIND_ENUM_CONSTANT(Camera3D::DopplerTracking::DOPPLER_TRACKING_DISABLED);
	BIND_ENUM_CONSTANT(Camera3D::DopplerTracking::DOPPLER_TRACKING_IDLE_STEP);
	BIND_ENUM_CONSTANT(Camera3D::DopplerTracking::DOPPLER_TRACKING_PHYSICS_STEP);
}


void VirtualCam3D::initialize_internal()
{
	GDCLASS_Metadata meta(get_parent_class_static(), additional_description, *_get_extension_class_name());
	set_editor_description(meta.get_metadata_string());
}


void VirtualCam3D::init_default_blend_data()
{
	blend_data->set_blend_name("VirtualCam3D blend data");
	blend_data->set_blend_by_value(2.0f);
	blend_data->set_blend_by(BlendData3D::BlendByType::DURATION);
	blend_data->set_ease(Tween::EASE_IN_OUT);
	blend_data->set_trans(Tween::TRANS_CUBIC);
	blend_data->set_blend_rotation(true);
	blend_data->set_callable_on_start(false);
	blend_data->set_callable_on_complete(false);
}


void VirtualCam3D::_register_to_cinecam3d()
{
	Camera3D* camera3d = get_viewport()->get_camera_3d();
	if (camera3d != nullptr && camera3d->has_method("_register_vcam_internal"))
	{
		found_cam = camera3d;
		((CineCam3D*)found_cam)->_register_vcam_internal(this);
	}
}

void VirtualCam3D::_remove_from_cinecam3d()
{
	if (found_cam == nullptr) return;
	if (scene_tree == nullptr) return;
	CineCam3D* cine_cam = cast_to<CineCam3D>(found_cam);
	if (cine_cam == nullptr) return;
	cine_cam->_remove_vcam_internal(this);
}


void VirtualCam3D::_notification(int p_what)
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
		case NOTIFICATION_ENTER_WORLD:
			viewport = get_viewport();
			ERR_FAIL_NULL(viewport);
	#ifdef TOOLS_ENABLED
			if (is_in_editor)
			{
				viewport->connect(StringName("size_changed"), callable_mp((Node3D*)this, &VirtualCam3D::update_gizmos));
			}
	#endif
			break;
		case NOTIFICATION_EXIT_WORLD:
	#ifdef TOOLS_ENABLED
					if (viewport && is_in_editor)
					{
						viewport->disconnect(StringName("size_changed"), callable_mp((Node3D*)this, &VirtualCam3D::update_gizmos));
					}
	#endif
				break;
		case NOTIFICATION_READY:
			if (!is_in_editor)
			{
				_register_to_cinecam3d();
			}
			break;
		case NOTIFICATION_EXIT_TREE:
			if (!is_in_editor)
			{
				_remove_from_cinecam3d();
			}
			break;
	}
}

// original VirtualCam3D

void VirtualCam3D::set_vcam_id(String id)
{
	vcam_id = id;
}


String VirtualCam3D::get_vcam_id() const
{
	return vcam_id;
}



void VirtualCam3D::set_priority(int prio)
{
	priority = prio;
	emit_signal(SIGNAL_PRIORITY_CHANGED, this, priority);
}


int VirtualCam3D::get_priority() const
{
	return priority;
}


Ref<BlendData3D> VirtualCam3D::_get_blend_data() const
{
	return blend_data;
}

void VirtualCam3D::_set_blend_data(Ref<BlendData3D> blend)
{
	blend_data = blend;
	if (blend.ptr() == nullptr) return;

	if (blend->_is_default_blend())
	{
		init_default_blend_data();
	}
}

// from Camera3D


void VirtualCam3D::_update_camera_mode()
{
	force_change = true;
	switch (mode)
	{
		case Camera3D::PROJECTION_PERSPECTIVE:
			set_perspective(fov, near, far);
			break;
		case Camera3D::PROJECTION_ORTHOGONAL:
			set_orthogonal(size, near, far);
			break;
		case Camera3D::PROJECTION_FRUSTUM:
			set_frustum(size, frustum_offset, near, far);
			break;
	}
}


void VirtualCam3D::_validate_property(PropertyInfo& p_property) const
{
	if (p_property.name == StringName("cam3d_fov"))
	{
		if (mode != Camera3D::PROJECTION_PERSPECTIVE)
		{
			p_property.usage = PROPERTY_USAGE_NO_EDITOR;
		}
	}
	else if (p_property.name == StringName("cam3d_size"))
	{
		if (mode != Camera3D::PROJECTION_ORTHOGONAL && mode != Camera3D::PROJECTION_FRUSTUM)
		{
			p_property.usage = PROPERTY_USAGE_NO_EDITOR;
		}
	}
	else if (p_property.name == StringName("cam3d_frustum_offset"))
	{
		if (mode != Camera3D::PROJECTION_FRUSTUM)
		{
			p_property.usage = PROPERTY_USAGE_NO_EDITOR;
		}
	}

	if (attributes.is_valid()) 
	{
		const CameraAttributesPhysical* physical_attributes = Object::cast_to<CameraAttributesPhysical>(attributes.ptr());
		if (physical_attributes) 
		{
			if (
				PRINT("Should_Change");
				p_property.name == StringName("near") ||
				p_property.name == StringName("far") ||
				p_property.name == StringName("fov") ||
				p_property.name == StringName("keep_aspect")
				)
			{
				p_property.usage = PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_EDITOR;
			}
		}
	}

	Node3D::_validate_property(p_property);
}


Transform3D VirtualCam3D::get_camera_transform() const
{
	Transform3D tr = get_global_transform().orthonormalized();
	tr.origin += tr.basis.get_column(1) * v_offset;
	tr.origin += tr.basis.get_column(0) * h_offset;
	return tr;
}


Projection VirtualCam3D::_get_camera_projection(real_t p_near) const
{
	Size2 viewport_size = get_viewport()->get_visible_rect().size;
	Projection cm;

	switch (mode)
	{
	case Camera3D::PROJECTION_PERSPECTIVE: 
		cm.set_perspective(fov, viewport_size.aspect(), p_near, far, keep_aspect == Camera3D::KEEP_WIDTH);
		break;
	case Camera3D::PROJECTION_ORTHOGONAL:
		cm.set_orthogonal(size, viewport_size.aspect(), p_near, far, keep_aspect == Camera3D::KEEP_WIDTH);
		break;
	case Camera3D::PROJECTION_FRUSTUM:
		cm.set_frustum(size, viewport_size.aspect(), frustum_offset, p_near, far);
		break;
	}

	return cm;
}


Projection VirtualCam3D::get_camera_projection() const
{
	ERR_FAIL_COND_V_MSG(!is_inside_tree(), Projection(), "Camera is not inside the scene tree.");
	return _get_camera_projection(near);
}


void VirtualCam3D::set_perspective(real_t p_fovy_degrees, real_t p_z_near, real_t p_z_far)
{
	if (!force_change &&
		fov == p_fovy_degrees &&
		p_z_near == near &&
		p_z_far == far &&
		mode == Camera3D::PROJECTION_PERSPECTIVE) 
	{
		return;
	}

	fov = p_fovy_degrees;
	near = p_z_near;
	far = p_z_far;
	mode = Camera3D::PROJECTION_PERSPECTIVE;

	update_gizmos();
	force_change = false;
}


void VirtualCam3D::set_orthogonal(real_t p_size, real_t p_z_near, real_t p_z_far)
{
	if (
		!force_change &&
		size == p_size &&
		p_z_near == near &&
		p_z_far == far &&
		mode == Camera3D::PROJECTION_ORTHOGONAL
		) 
	{
		return;
	}

	size = p_size;

	near = p_z_near;
	far = p_z_far;
	mode = Camera3D::PROJECTION_ORTHOGONAL;
	force_change = false;

	update_gizmos();
}


void VirtualCam3D::set_frustum(real_t p_size, Vector2 p_offset, real_t p_z_near, real_t p_z_far)
{
	if (!force_change &&
		size == p_size &&
		frustum_offset == p_offset &&
		p_z_near == near &&
		p_z_far == far &&
		mode == Camera3D::PROJECTION_FRUSTUM) 
	{
		return;
	}

	size = p_size;
	frustum_offset = p_offset;

	near = p_z_near;
	far = p_z_far;
	mode = Camera3D::PROJECTION_FRUSTUM;
	force_change = false;

	update_gizmos();
}


void VirtualCam3D::set_projection(Camera3D::ProjectionType p_mode)
{
	if (
		p_mode == Camera3D::PROJECTION_PERSPECTIVE ||
		p_mode == Camera3D::PROJECTION_ORTHOGONAL ||
		p_mode == Camera3D::PROJECTION_FRUSTUM
		)
	{
		mode = p_mode;
		_update_camera_mode();
		notify_property_list_changed();
	}
}


void VirtualCam3D::set_current(bool p_enabled)
{
	current = p_enabled;
}


bool VirtualCam3D::is_current() const
{
	return current;
}


bool VirtualCam3D::is_position_behind(const Vector3& p_pos) const
{
	Transform3D t = get_global_transform();
	Vector3 eyedir = -t.basis.get_column(2).normalized();
	return eyedir.dot(p_pos - t.origin) < near;
}


Vector<Vector3> VirtualCam3D::get_near_plane_points() const
{
	ERR_FAIL_COND_V_MSG(!is_inside_tree(), Vector<Vector3>(), "Camera is not inside scene.");

	Projection cm = _get_camera_projection(near);

	Vector3 endpoints[8];
	cm.get_endpoints(Transform3D(), endpoints);

	Vector<Vector3> points =
	{
		Vector3(),
		endpoints[4],
		endpoints[5],
		endpoints[6],
		endpoints[7]
	};
	return points;
}


void VirtualCam3D::set_environment(const Ref<Environment>& p_environment)
{
	environment = p_environment;
	_update_camera_mode();
}


Ref<Environment> VirtualCam3D::get_environment() const
{
	return environment;
}


void VirtualCam3D::set_attributes(const Ref<CameraAttributes>& p_attributes)
{
	if (attributes.is_valid()) 
	{
		CameraAttributesPhysical* physical_attributes = Object::cast_to<CameraAttributesPhysical>(attributes.ptr());
		if (physical_attributes) 
		{
			attributes->disconnect("attributes_changed", callable_mp(this, &VirtualCam3D::_attributes_changed));
		}
	}

	attributes = p_attributes;

	if (attributes.is_valid())
	{
		CameraAttributesPhysical* physical_attributes = Object::cast_to<CameraAttributesPhysical>(attributes.ptr());
		if (physical_attributes)
		{
			attributes->disconnect("attributes_changed", callable_mp(this, &VirtualCam3D::_attributes_changed));
			_attributes_changed();
		}
	}

	notify_property_list_changed();
}


Ref<CameraAttributes> VirtualCam3D::get_attributes() const
{
	return attributes;
}


void VirtualCam3D::_attributes_changed()
{
	CameraAttributesPhysical* physical_attributes = Object::cast_to<CameraAttributesPhysical>(attributes.ptr());
	ERR_FAIL_NULL(physical_attributes);

	fov = physical_attributes->get_fov();
	near = physical_attributes->get_near();
	far = physical_attributes->get_far();
	keep_aspect = Camera3D::KEEP_HEIGHT;
	_update_camera_mode();
}


void VirtualCam3D::set_keep_aspect_mode(Camera3D::KeepAspect p_aspect)
{
	keep_aspect = p_aspect;
	_update_camera_mode();
	notify_property_list_changed();
}


Camera3D::KeepAspect VirtualCam3D::get_keep_aspect_mode() const
{
	return keep_aspect;
}


void VirtualCam3D::set_doppler_tracking(Camera3D::DopplerTracking p_tracking)
{
	if (doppler_tracking == p_tracking) return;
	doppler_tracking = p_tracking;
	_update_camera_mode();
}


Camera3D::DopplerTracking VirtualCam3D::get_doppler_tracking() const
{
	return doppler_tracking;
}


real_t VirtualCam3D::get_fov() const
{
	return fov;
}


real_t VirtualCam3D::get_size() const
{
	return size;
}

real_t VirtualCam3D::get_near() const
{
	return near;
}


Vector2 VirtualCam3D::get_frustum_offset() const
{
	return frustum_offset;
}


real_t VirtualCam3D::get_far() const
{
	return far;
}


Camera3D::ProjectionType VirtualCam3D::get_projection() const
{
	return mode;
}


void VirtualCam3D::set_fov(real_t p_fov)
{
	ERR_FAIL_COND(p_fov < 1 || p_fov > 179);
	fov = p_fov;
	_update_camera_mode();
}


void VirtualCam3D::set_size(real_t p_size)
{
	ERR_FAIL_COND(p_size <= CMP_EPSILON);
	size = p_size;
	_update_camera_mode();
}


void VirtualCam3D::set_near(real_t p_near)
{
	near = p_near;
	_update_camera_mode();
}


void VirtualCam3D::set_frustum_offset(Vector2 p_offset)
{
	frustum_offset = p_offset;
	_update_camera_mode();
}


void VirtualCam3D::set_far(real_t p_far)
{
	far = p_far;
	_update_camera_mode();
}


void VirtualCam3D::set_cull_mask(uint32_t p_layers)
{
	layers = p_layers;
	_update_camera_mode();
}


uint32_t VirtualCam3D::get_cull_mask() const
{
	return layers;
}


void VirtualCam3D::set_cull_mask_value(int p_layer_number, bool p_value)
{
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Render layer number must be between 1 and 20 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 20, "Render layer number must be between 1 and 20 inclusive.");
	uint32_t mask = get_cull_mask();
	if (p_value) {
		mask |= 1 << (p_layer_number - 1);
	}
	else {
		mask &= ~(1 << (p_layer_number - 1));
	}
	set_cull_mask(mask);
}


bool VirtualCam3D::get_cull_mask_value(int p_layer_number) const
{
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Render layer number must be between 1 and 20 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 20, false, "Render layer number must be between 1 and 20 inclusive.");
	return layers & (1 << (p_layer_number - 1));
}


TypedArray<Plane> VirtualCam3D::get_frustum() const
{
	TypedArray<Plane> result;

	Projection cm = _get_camera_projection(near);
	Array planes = cm.get_projection_planes(get_camera_transform());

	for (int i = 0; i < planes.size(); i++)
	{
		result.append(planes[i]);
	}

	return result;
}


bool VirtualCam3D::is_position_in_frustum(const Vector3& p_position) const
{
	TypedArray<Plane> frustum_arr = get_frustum();
	Vector<Plane> frustum;

	for (int i = 0; i < frustum_arr.size(); i++)
	{
		frustum.append(frustum_arr[i]);
	}

	for (int i = 0; i < frustum.size(); i++) 
	{
		Plane f = frustum[i];
		if (f.is_point_over(p_position)) 
		{
			return false;
		}
	}
	return true;
}


void VirtualCam3D::set_v_offset(real_t p_offset)
{
	v_offset = p_offset;
}


real_t VirtualCam3D::get_v_offset() const
{
	return v_offset;
}


void VirtualCam3D::set_h_offset(real_t p_offset)
{
	h_offset = p_offset;
}


real_t VirtualCam3D::get_h_offset() const
{
	return h_offset;
}


RID VirtualCam3D::get_pyramid_shape_rid()
{
	ERR_FAIL_COND_V_MSG(!is_inside_tree(), RID(), "Camera is not inside scene.");
	if (pyramid_shape == RID()) 
	{
		pyramid_shape_points = get_near_plane_points();
		pyramid_shape = PhysicsServer3D::get_singleton()->convex_polygon_shape_create();
	}
	else 
	{
		//check if points changed
		Vector<Vector3> local_points = get_near_plane_points();

		bool all_equal = true;

		for (int i = 0; i < 5; i++)
		{
			if (local_points[i] != pyramid_shape_points[i]) {
				all_equal = false;
				break;
			}
		}

		if (!all_equal) 
		{
			pyramid_shape_points = local_points;
		}
	}

	return pyramid_shape;
}