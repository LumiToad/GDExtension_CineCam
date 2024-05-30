class_name H_VirtualCam2D
extends Node2D
#Inherits: Node2D < CanvasItem < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [VirtualCam2D] class! [br]
## 
## Virtual camera for the [H_CineCam2D]. [br]
## Imagine these nodes as dummy cameras for the [H_CineCam2D]. [br]
## [br]
## These dummy cameras will automatically register to the first found [H_CineCam2D]. [br]
## Be careful! At least one virtual camera has to be above a [H_CineCam2D] in the hierarchy! [br]
## Otherwise the game will crash! [br]
## You can setup blends, a camera priority and an ID to find a virtual camera node. [br]
## See [H_CineCam2D] for all the things, that you can do with a [H_VirtualCam2D] node! [br]
## [br]
## They also contain all the properties of a [Camera2D] as dummy values without any use. [br]
## Using [method H_CineCam2D.apply_vcam2d_data], you can copy the data to your currently runing [H_CineCam2D]. [br]
## 
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/VirtualCam2D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(Manual camera blending): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Manual-camera-blending
## @tutorial(Blending by priority): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Camera-blending-by-priority-%E2%80%90-CineCam-wiki
## @tutorial(CamSequence for a series of blends): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Using-CamSequence-for-a-series-of-blends
## @tutorial(Setup virtual camera data for later use): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Setup-virtual-camera-data-for-later-use
#END_OF_DESCRIPTION


## Emitted, when the [member H_VirtualCam2D.priority] has changed. [br]
signal priority_changed(vcam2d: Object, priority: int)


## Getter / Setter for the vcam_id property. [br]
## This [String] is just a name, which you can search, using [method H_CineCam2D.find_vcam_by_id]. [br]
var vcam_id : String:
	get: return get_vcam_id()
	set(value): set_vcam_id(value)


## Getter / Setter for the priority of this virtual camera. [br]
## A [H_CineCam2D] will always know the virtual camera with the highest priority. [br]
## See [enum H_CineCam2D.FollowMode] for this. [br]
## The [H_CineCam2D] will register the priority, either when: [br]
## - [H_VirtualCam2D] nodes are changed (instanced for example) in the scene tree. [br]
## - If their priority changes. [br]
## This also happens, when loading the scene! [br]
## If two or more virtual cameras have the same priority, a [H_CineCam2D] will register the latest registered as the new "highest priority virtual camera". [br]
var priority : int = 0:
	get: return get_priority()
	set(value): set_priority(value)


## The default blending data for this virtual camera. [br]
## Will be used, when a [H_CineCam2D] blends to this virtual camera. [br]
## If this is empty, [member H_CineCam2D.blend_data] will be used as a default blend. [br]
var blend_data : H_BlendData2D :
	get: return _get_blend_data()
	set(value): _set_blend_data(value)


## Getter for vcam_id. [br]
func get_vcam_id() -> String:
	return ""


## Setter for vcam_id. [br]
func set_vcam_id(value : String):
	pass


## Getter for priority. [br]
func get_priority() -> int:
	return 0


## Setter for priority. [br]
func set_priority(value : int):
	pass


## Getter for blend_data. [br]
func _get_blend_data() -> H_BlendData2D:
	return blend_data


## Setter for blend_data. [br]
func _set_blend_data(value : H_BlendData2D):
	pass
