class_name H_CamSequence3D
extends Node3D
#Inherits: Node3D < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CamSequence3D] class! [br]
## 
## This node is only useful in combination with [H_VirtualCam3D] and [H_CineCam3D]. [br]
## Attach [H_VirtualCam3D] nodes to this sequence. Then a [H_CineCam3D] can access it using various methods. [br]
## Will automatically reload the internal array of virtual cameras on [constant Node.NOTIFICATION_CHILD_ORDER_CHANGED]. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/CamSequence3D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(CamSequence for a series of blends): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Using-CamSequence-for-a-series-of-blends
#END_OF_DESCRIPTION


## Will be emitted, when the index of the currently selected virtual camera has changed. [br]
## Returns [H_VirtualCam3D] and the current index. [br]
signal index_changed(vcam3D : Object, index : int)


## Getter / Setter for the current index of the sequence. [br]
var current_idx : int :
	get: return get_current_idx()
	set(value): set_current_idx(value)


## Returns the currently selected [H_VirtualCam3D]. [br]
## That means, that if the sequence is running, the target for the currently running blend is returned. [br]
func current_vcam() -> VirtualCam3D:
	return


## Returns all [H_VirtualCam3D] that are registred to this node. [br]
## This is basically the [H_VirtualCam3D] children of this node. [br]
## When virtual cams are added, removed or changed in order, the internal array will changed automatically. [br]
## You can use this method after such a change to get the new array. [br]
func get_vcam3D_array() -> Array[VirtualCam3D]:
	var array = [VirtualCam3D]
	return array


## Returns the [H_VirtualCam3D] at given index of the internal array. [br]
func vcam3D_at(index: int) -> VirtualCam3D:
	return


## Getter for current_idx. [br]
func get_current_idx() -> int:
	return 0

## Setter for current_idx. [br]
func set_current_idx(value : int):
	pass
