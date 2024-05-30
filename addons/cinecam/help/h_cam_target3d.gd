class_name H_CamTarget3D
extends Node3D
#Inherits: Node3D < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CamTarget3D] class! [br]
## 
## A Target for the [H_CineCam3D]. [br]
## This node is used as a "carrot on a stick" for a [H_CineCam3D]. [br]
## See [member H_CineCam3D.target], [member H_CineCam3D.look_at_target] and [enum H_CineCam3D.FollowMode] for more infos. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/CamTarget3D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(Following a target): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Following-a-target
#END_OF_DESCRIPTION

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


## Getter / Setter for the target offset. [br]
## If a [H_CineCam3D] is following this target, it will add this offest to the equation. [br]
## Example: Useful if you want to pan the camera upwards, when a character looks up or down, like in some platformers. [br]
var offset : Vector3 = Vector3():
	get: return get_target_offset()
	set(value): set_target_offset(value)


## Getter / setter for the X-Axis follow speed. [br]
## X-Axis blend speed, if a [H_CineCam3D] is following or looking at this target, using [constant H_CineCam3D.TARGET_BLEND]. [br]
## This is set in percentage from 0.0% to 100.0%. Cannot exceed this limit! [br]
var speed_x : float = 0.0:
	get: return get_speed_x()
	set(value): set_speed_x(value)


## Getter / setter for the Y_Axis speed. [br]
## Y_Axis blend speed, if a [H_CineCam3D] is following or looking at this target, using [constant H_CineCam3D.TARGET_BLEND]. [br]
## This is set in percentage from 0.0% to 100.0%. Cannot exceed this limit! [br]
var speed_y : float = 0.0:
	get: return get_speed_y()
	set(value): set_speed_y(value)


## Getter / setter for the Z_Axis follow speed. [br]
## Z_Axis blend speed, if a [H_CineCam3D] is following or looking at this target, using [constant H_CineCam3D.TARGET_BLEND]. [br]
## This is set in percentage from 0.0% to 100.0%. Cannot exceed this limit! [br]
var speed_z : float = 0.0:
	get: return get_speed_z()
	set(value): set_speed_z(value)


## Returns speed_x, speed_y and speed_z as a Vector3, but scaled down to represent the actual blending speed. [br]
## From 0.00 to 1.00
func scaled_speed() -> Vector3:
	return Vector3()


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


## Getter for offset. [br]
func get_target_offset() -> Vector3:
	return Vector3()


## Setter for offset. [br]
func set_target_offset(value : Vector3):
	pass


## Getter for speed_x. [br]
func get_speed_x() -> float:
	return 0.0


## Setter for speed_x. [br]
func set_speed_x(value : float):
	pass


## Getter for speed_y. [br]
func get_speed_y() -> float:
	return 0.0


## Setter for speed_y. [br]
func set_speed_y(value : float):
	pass


## Getter for speed_z. [br]
func get_speed_z() -> float:
	return 0.0


## Setter for speed_z. [br]
func set_speed_z(value : float):
	pass
