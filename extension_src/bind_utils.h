// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// bind_utils.h

#ifndef BINDUTILS_H
#define BINDUTILS_H

#define VA_LIST(...) __VA_ARGS__

#define ADD_GETSET_BINDING(GETTER_NAME, SETTER_NAME, VAR, P_VAR, CLASS_NAME, VARIANT_TYPE) ClassDB::bind_method(D_METHOD(#GETTER_NAME), &CLASS_NAME::GETTER_NAME); \
ClassDB::bind_method(D_METHOD(#SETTER_NAME, #P_VAR), &CLASS_NAME::SETTER_NAME); \
ClassDB::add_property(#CLASS_NAME, PropertyInfo(Variant::VARIANT_TYPE, #VAR), #SETTER_NAME, #GETTER_NAME) \

#define ADD_GETSET_HINT_BINDING(GETTER_NAME, SETTER_NAME, VAR, P_VAR, CLASS_NAME, VARIANT_TYPE, PROPERTY_HINT_TYPE, HINT_STRING) ClassDB::bind_method(D_METHOD(#GETTER_NAME), &CLASS_NAME::GETTER_NAME); \
ClassDB::bind_method(D_METHOD(#SETTER_NAME, #P_VAR), &CLASS_NAME::SETTER_NAME); \
ClassDB::add_property(#CLASS_NAME, PropertyInfo(Variant::VARIANT_TYPE, #VAR, PROPERTY_HINT_TYPE, HINT_STRING), #SETTER_NAME, #GETTER_NAME) \

#define ADD_METHOD_BINDING(METHOD_NAME, CLASS_NAME) ClassDB::bind_method(D_METHOD(#METHOD_NAME), &CLASS_NAME::METHOD_NAME)
#define ADD_METHOD_ARGS_BINDING(METHOD_NAME, CLASS_NAME, ARGS) ClassDB::bind_method(D_METHOD(#METHOD_NAME, ARGS), &CLASS_NAME::METHOD_NAME)
#define ADD_METHOD_DEFAULTARGS_BINDING(METHOD_NAME, CLASS_NAME, ARGS, DEFAULT_ARGS) ClassDB::bind_method(D_METHOD(#METHOD_NAME, ARGS), &CLASS_NAME::METHOD_NAME, DEFAULT_ARGS)

// kinda "backported" from future godot versions???
//#define ADD_PROPERTYI(m_property, m_setter, m_getter, m_index) ClassDB::add_property(get_class_static(), m_property, m_setter, m_getter, m_index)

#endif // BINDUTILS_H