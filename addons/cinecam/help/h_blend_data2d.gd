class_name H_BlendData2D
extends Resource
#Inherits: Resource < RefCounted < Object
#MAIN_DESCRIPTION
## This help document refers to [BlendData2D] class! [br]
## 
## Blending data for the [H_CineCam2D] / [H_VirtualCam2D]. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/BlendData2D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(Godot tweening cheat sheet): https://www.reddit.com/r/godot/comments/frqzup/godot_tweening_cheat_sheet/
#END_OF_DESCRIPTION


## Controls how the [member H_BlendData2D.blend_by_value] is being used. [br]
## You can retrieve the full duration of the blend with [method H_CineCam2D.full_blend_duration]. [br]
enum BlendByType{
	## The value will be used as time in seconds. [br]
	DURATION = 0,
	## The value will be a relative speed value. [br]
	## The time in seconds will be calculated. [br]
	SPEED = 1
}


## Getter / Setter for the current blend_by property. [br]
## See [enum H_BlendData2D.BlendByType] [br]
var blend_by : BlendByType = BlendByType.DURATION:
	get: return get_blend_by()
	set(value): set_blend_by(value)


## Getter / Setter for the blend by value. [br]
## This if [member H_BlendData2D.blend_by] is set to [constant H_BlendData2D.DURATION], this will be used as seconds. [br]
## If set to [constant H_BlendData2D.SPEED], it will be used as a relative speed value. [br]
## See [enum H_BlendData2D.BlendByType] for more information. [br]
## In both cases, these values will be used by an internal tween, so there is no need to delta time them. [br]
var blend_by_value : float = 0.0:
	get: return get_blend_by_value()
	set(value): set_blend_by_value(value)


## Getter / Setter for the name of the blend. [br]
## This is used as an ID for [method H_CineCam2D.find_vcam_by_id]. [br]
var blend_name : String:
	get: return get_blend_name()
	set(value): set_blend_name(value)


## Getter / Setter for blend rotation property. [br]
## If set to true, the blend will also rotate the [H_CineCam2D] to match the rotation of the given [H_VirtualCam2D]. [br]
var blend_rotation : bool = true:
	get: return is_blend_rotation()
	set(value): set_blend_rotation(value)


## Getter / Setter for the optional callable. [br]
## You can add a [Callable] to the blend_data, which will be called by the [H_CineCam2D], [br]
## either on the start of the blend of when its complete. [br]
## See [member H_BlendData2D.on_start] / [member H_BlendData2D.on_complete] for this. [br]
var callable : Callable:
	get: return get_callable()
	set(value): set_callable(value)


## Getter / Setter for the ease type of the blend. [br]
## See [enum Tween.EaseType] for more info. [br]
var ease : Tween.EaseType:
	get: return get_ease()
	set(value): set_ease(value)


## Getter / Setter for the transition type of the blend. [br]
## See [enum Tween.TransitionType] for more info. [br]
var trans : Tween.TransitionType:
	get: return get_trans()
	set(value): set_trans(value)


## Getter / Setter for the on start property. [br]
## If this is set to true, [member H_BlendData2D.callable] will be called on the start of the blend. [br]
var on_start : bool = false:
	get: return get_callable_on_start()
	set(value): set_callable_on_start(value)


## Getter / Setter for the on complete property. [br]
## If this is set to true, [member H_BlendData2D.callable] will be called when the blend is complete. [br]
var on_complete : bool = false:
	get: return get_callable_on_complete()
	set(value): set_callable_on_complete(value)


## Getter for blend_by. [br]
func get_blend_by() -> BlendByType:
	return BlendByType.DURATION


## Setter for blend_by. [br]
func set_blend_by(blend_by : BlendByType):
	pass


## Getter for blend_by_value. [br]
func get_blend_by_value() -> float:
	return 0.0


## Setter for blend_by_value. [br]
func set_blend_by_value(value : float):
	pass


## Getter for blend_name. [br]
func get_blend_name() -> String:
	return ""


## Setter for blend_name. [br]
func set_blend_name(name : String):
	pass


## Getter for blend_rotation. [br]
func is_blend_rotation() -> bool:
	return true


## Setter for blend_rotation. [br]
func set_blend_rotation(value : bool):
	pass


## Getter for callable. [br]
func get_callable() -> Callable:
	return Callable()


## Setter for callable. [br]
func set_callable(callable : Callable):
	pass


## Getter for ease. [br]
func get_ease() -> Tween.EaseType:
	return Tween.EaseType.EASE_IN


## Setter for ease. [br]
func set_ease(ease : Tween.EaseType):
	pass


## Getter for trans. [br]
func get_trans() -> Tween.TransitionType:
	return Tween.TransitionType.TRANS_LINEAR


## Setter for trans. [br]
func set_trans(trans : Tween.TransitionType):
	pass


## Getter for on_start. [br]
func get_callable_on_start() -> bool:
	return false


## Setter for on_start. [br]
func set_callable_on_start(value : bool):
	pass


## Getter for on_complete. [br]
func get_callable_on_complete() -> bool:
	return false


## Setter for on_complete. [br]
func set_callable_on_complete(value : bool):
	pass
