class_name H_CamTarget2D
extends Node2D
#Inherits: Node2D < CanvasItem < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CamTarget2D] class! [br]
## 
## A Target for the [H_CineCam2D]. [br]
## This node is used as a "carrot on a stick" for a [H_CineCam2D]. [br]
## See [member H_CineCam2D.target] and [enum H_CineCam2D.FollowMode] for more infos. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/CamTarget2D-class-reference-%E2%80%90-CineCam-wiki
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
## If a [H_CineCam2D] is following this target, it will add this offest to the equation. [br]
## Example: Useful if you want to pan the camera upwards, when a character looks up or down, like in some platformers. [br]
var offset : Vector2 = Vector2():
	get: return get_target_offset()
	set(value): set_target_offset(value)


## Getter / setter for the horizontal follow speed. [br]
## Horizontal blend speed, if a [H_CineCam2D] is following this target, using [constant H_CineCam2D.TARGET_BLEND]. [br]
## This is set in percentage from 0.0% to 100.0%. Cannot exceed this limit! [br]
var speed_x : float = 0.0:
	get: return get_speed_x()
	set(value): set_speed_x(value)


## Getter / setter for the vertical follow speed. [br]
## Vertical blend speed, if a [H_CineCam2D] is following this target, using [constant H_CineCam2D.TARGET_BLEND]. [br]
## This is set in percentage from 0.0% to 100.0%. Cannot exceed this limit! [br]
var speed_y : float = 0.0:
	get: return get_speed_y()
	set(value): set_speed_y(value)


## Returns speed_x and speed_y as a Vector2, but scaled down to represent the actual blending speed. [br]
## From 0.00 to 1.00
func scaled_speed() -> Vector2:
	return Vector2()


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
func get_target_offset() -> Vector2:
	return Vector2()


## Setter for offset. [br]
func set_target_offset(value : Vector2):
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
