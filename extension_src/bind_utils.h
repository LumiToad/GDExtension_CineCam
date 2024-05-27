// Copyright(c) 2014 - present Godot Engine contributors(see AUTHORS.md), Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// CineCam GDExtension made by Lukas Schmidt (https://github.com/lumitoad/)
   
// bind_utils.h

#ifndef BINDUTILS_H
#define BINDUTILS_H

#define VA_LIST(...) __VA_ARGS__

#define ADD_METHOD_BINDING(METHOD_NAME, CLASS_NAME) ClassDB::bind_method(D_METHOD(#METHOD_NAME), &CLASS_NAME::METHOD_NAME)
#define ADD_METHOD_ARGS_BINDING(METHOD_NAME, CLASS_NAME, ARGS) ClassDB::bind_method(D_METHOD(#METHOD_NAME, ARGS), &CLASS_NAME::METHOD_NAME)
#define ADD_METHOD_DEFAULTARGS_BINDING(METHOD_NAME, CLASS_NAME, ARGS, DEFAULT_ARGS) ClassDB::bind_method(D_METHOD(#METHOD_NAME, ARGS), &CLASS_NAME::METHOD_NAME, DEFAULT_ARGS)

#endif // BINDUTILS_H