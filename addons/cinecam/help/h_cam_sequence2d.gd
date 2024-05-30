class_name H_CamSequence2D
extends Node2D
#Inherits: Node2D < CanvasItem < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CamSequence2D] class! [br]
## 
## This node is only useful in combination with [H_VirtualCam2D] and [H_CineCam2D]. [br]
## Attach [H_VirtualCam2D] nodes to this sequence. Then a [H_CineCam2D] can access it using various methods. [br]
## Will automatically reload the internal array of virtual cameras on [constant Node.NOTIFICATION_CHILD_ORDER_CHANGED]. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/CamSequence2D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(CamSequence for a series of blends): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Using-CamSequence-for-a-series-of-blends
#END_OF_DESCRIPTION


## Will be emitted, when the index of the currently selected virtual camera has changed. [br]
## Returns [H_VirtualCam2D] and the current index. [br]
signal index_changed(vcam2d : Object, index : int)


## Getter / Setter for the current index of the sequence. [br]
var current_idx : int = 0 :
	get: return get_current_idx()
	set(value): set_current_idx(value)


## Getter / Setter for _draw_debug_lines property. [br]
## Will draw debug lines between [H_VirtualCam2D] nodes, in order of sequence execution. [br]
var _draw_debug_lines : bool:
	get: return get_is_draw_debug_lines()
	set(value): set_is_draw_debug_lines(value)


## Getter / Setter for _is_draw_debug_color property. [br]
## Color for debug lines. [br]
var _debug_lines_color : Color:
	get: return get_debug_lines_color()
	set(value): set_debug_lines_color(value)


## Getter / Setter for _is_draw_debug_width property. [br]
## Width for debug lines. [br]
var _debug_lines_width : float:
	get: return get_debug_lines_width()
	set(value): set_debug_lines_width(value)


## Returns the currently selected [H_VirtualCam2D]. [br]
## That means, that if the sequence is running, the target for the currently running blend is returned. [br]
func current_vcam() -> VirtualCam2D:
	return


## Returns all [H_VirtualCam2D] that are registred to this node. [br]
## This is basically the [H_VirtualCam2D] children of this node. [br]
## When virtual cams are added, removed or changed in order, the internal array will changed automatically. [br]
## You can use this method after such a change to get the new array. [br]
func get_vcam2d_array() -> Array[VirtualCam2D]:
	var array = [VirtualCam2D]
	return array


## Returns the [H_VirtualCam2D] at given index of the internal array. [br]
func vcam2d_at(index: int) -> VirtualCam2D:
	return


## Getter for current_idx. [br]
func get_current_idx() -> int:
	return 0

## Setter for current_idx. [br]
func set_current_idx(value : int):
	pass


## Getter for is_draw_debug_lines.
func get_is_draw_debug_lines() -> bool:
	return false


## Setter for is_draw_debug_lines.
func set_is_draw_debug_lines(value : bool):
	pass


## Getter for _draw_debug_color.
func get_debug_lines_color() -> Color:
	return Color()


## Setter for _draw_debug_color.
func set_debug_lines_color(value : Color):
	pass


## Getter for _draw_debug_width.
func get_debug_lines_width() -> float:
	return 0.0


## Setter for _draw_debug_width.
func set_debug_lines_width(value : float):
	pass
