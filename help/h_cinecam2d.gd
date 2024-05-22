class_name H_CineCam2D
extends  Node3D
#Inherits: Camera2D < Node2D < CanvasItem < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CineCam2D] class! [br]
## 
## Main Camera for the [CineCam2D]. [br]
## Features include camera shaking, camera blending to [H_VirtualCam2D], targeting [H_CamTarget2D] and more. [br]
##
## @tutorial(Link1): https://lnk.bio/LumiToad
## @tutorial(Link2): https://www.linkedin.com/in/LumiToad/
## @tutorial(Link3): https://github.com/LumiToad
#END_OF_DESCRIPTION


## Emitted when a blend is completed. [br]
## Will not be emitted for [enum H_CineCam2D.FollowMode] [constant H_CineCam2D.TARGET_BLEND]. [br]
signal blend_completed


## Emitted when a blend is paused. [br]
## Will not be emitted for [enum H_CineCam2D.FollowMode] [constant H_CineCam2D.TARGET_BLEND]. [br]
signal blend_paused


## Emitted when a blend is resumed. [br]
## Will not be emitted for [enum H_CineCam2D.FollowMode] [constant H_CineCam2D.TARGET_BLEND]. [br]
signal blend_resumed


## Emitted when a blend is completed. [br]
## Will not be emitted for [enum H_CineCam2D.FollowMode] [constant H_CineCam2D.TARGET_BLEND]. [br] 
signal blend_started


## Emitted when any registered [H_VirtualCam2D] priority became higher than the previously highest priority [H_VirtualCam2D]. [br] 
## Returns the [H_VirtualCam2D] with the now highest priority as an integer. [br]
## See [member H_VirtualCam2D.priority]. [br]
## [br]
## It doesn't matter, if this [CineCam2D] acts on it. [br] 
signal prioritized_vcam2d_changed(vcam2d: Object, priority: int)


## Emitted when a sequence of blends, using [CamSequence2D] was completed. [br]
## [br]
signal sequence_completed


## Emitted when a sequence of blends, using [CamSequence2D] was paused. [br]
## See [method H_CineCam2D.seq_pause]. [br]
signal sequence_paused


## Emitted when a sequence of blends, using [CamSequence2D] was resumed. [br]
## See [method H_CineCam2D.seq_resume]. [br]
signal sequence_resumed


## Emitted when a sequence of blends, using [CamSequence2D] was started. See: [br]
## - [method H_CineCam2D.start_sequence] [br]
## - [method H_CineCam2D.start_sequence_at] [br]
## - [method H_CineCam2D.seq_blend_to] [br]
## - [method H_CineCam2D.seq_blend_prev] [br]
## - [method H_CineCam2D.seq_blend_prev] [br]
signal sequence_started


## Emitted when a sequence of blends, using [CamSequence2D] was stopped. [br]
## See [method H_CineCam2D.seq_stop]. [br]
signal sequence_stopped


## Emitted when the camera shake, using its offset, has ended. [br]
## See [method H_CineCam2D.shake_offset]. [br]
signal shake_offset_ended


## Emitted when the camera shake, using its offset, is started. [br]
## See [method H_CineCam2D.shake_offset]. [br]
signal shake_offset_started


## Emitted when the camera shake, using its rotation, has ended. [br]
## See [method H_CineCam2D.shake_rotation]. [br]
signal shake_rotation_ended


## Emitted when the camera shake, using its rotation, has started. [br]
## See [method H_CineCam2D.shake_rotation]. [br]
signal shake_rotation_started


## Emitted when the camera shake, using its zoom, has ended. [br]
## See [method H_CineCam2D.shake_zoom]. [br]
signal shake_zoom_ended


## Emitted when the camera shake, using its zoom, has started. [br]
## See [method H_CineCam2D.shake_zoom]. [br]
signal shake_zoom_started


## Setings for [member H_CineCam2D.follow_mode]. [br]
## PRIO refers to the [H_VirtualCam2D] with the highest priority in the [SceneTree] only! [br]
## TARGET is for [H_CamTarget2D] if it is set in [member H_CineCam2D.target]! [br]
enum  FollowMode{
	OFF = 0,
	## Will constantly follow the it. [br]
	PRIO = 1,
	## Will follow it once. [br]
	PRIO_ONESHOT = 2,
	## Will blend towards it once, using the data from [member H_VirtualCam2D.blend_data]. [br]
	## If none are given, [member H_CineCam2D.blend_data] will be used. [br]
	PRIO_BLEND = 3,
	## Will constantly follow the target. [br]
	TARGET = 4,
	## Will constantly blend towards the target. [br]
	TARGET_BLEND = 5
}


var blend_data : BlendData2D :
	get: return _get_blend_data()
	set(value): _set_blend_data(value)


var blend_pause : bool = false :
	get: return _is_blend_paused()
	set(value): _set_blend_paused(value)


var current_sequence : CamSequence2D :
	get: return get_current_sequence()
	set(value): set_current_sequence(value)


var follow_mode : FollowMode = 0 :
	get: return get_follow_mode()
	set(value): set_follow_mode(value)


var follow_prio_pause : bool = false :
	get: return is_follow_prio_paused()
	set(value): set_follow_prio_paused(value)


var follow_target_pause : bool = false :
	get: return is_follow_target_paused()
	set(value): set_follow_target_paused(value)


var sequence_pause : bool = false :
	get: return _is_seq_paused()
	set(value): _set_seq_paused(value)


var shake_offset_duration : float = 0.0 :
	get: return _get_shake_offset_duration()
	set(value): _set_shake_offset_duration(value)


var shake_offset_intensity : Vector2 = Vector2() :
	get: return _get_shake_offset_intensity()
	set(value): _set_shake_offset_intensity(value)


var shake_rotation_duration : float = 0.0 :
	get: return _get_shake_rotation_duration()
	set(value): _set_shake_rotation_duration(value)


var shake_rotation_intensity : float = 0.0 :
	get: return _get_shake_rotation_intensity()
	set(value): _set_shake_rotation_intensity(value)


var shake_zoom_duration : float = 0.0 :
	get: return _get_shake_zoom_duration()
	set(value): _set_shake_zoom_duration(value)


var shake_zoom_intensity : Vector2 = Vector2() :
	get: return _get_shake_zoom_intensity()
	set(value): _set_shake_zoom_intensity(value)


var target : CamTarget2D :
	get: return get_target()
	set(value): set_target(value)


func apply_vcam2d_data(vcam2d: VirtualCam2D) -> String :
	return ""


func pause_blend():
	pass


func pause_follow_prio():
	pass


func pause_follow_target():
	pass


func prioritized_vcam():
	pass


func reposition_to_vcam(vcam : VirtualCam2D):
	pass


func resume_blend():
	pass


func resume_follow_prio():
	pass


func resume_follow_target():
	pass


func toggle_blend():
	pass


func seq_blend_next():
	pass


func seq_blend_prev():
	pass


func seq_blend_to(idx: int):
	pass


func seq_pause():
	pass


func seq_resume():
	pass


func seq_stop():
	pass


## Shakes the cam. [br]
## Takes a strength and a duration in seconds. [br]
func shake_offset(intensity : Vector2, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


## Shakes the cam. [br]
## Takes a strength and a duration in seconds. [br]
func shake_rotation(intensity : float, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


## Shakes the cam zoom. [br]
## Takes a strength and a duration in seconds. [br]
func shake_zoom(intensity : Vector2, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


func start_sequence(backwards : bool = false):
	pass


func start_sequence_at(backwards : bool = false, index : int = 0):
	pass


func toggle_follow_prio():
	pass


func toggle_follow_target():
	pass


func _get_blend_data() -> BlendData2D:
	return blend_data


func _set_blend_data(value : BlendData2D):
	pass


func _is_blend_paused() -> bool:
	return blend_pause


func _set_blend_paused(value : bool):
	pass



func get_current_sequence() -> CamSequence2D:
	return current_sequence


func set_current_sequence(value : CamSequence2D):
	pass


func get_follow_mode() -> FollowMode:
	return follow_mode


func set_follow_mode(value : FollowMode):
	pass


func is_follow_prio_paused() -> bool:
	return follow_prio_pause


func set_follow_prio_paused(value : bool):
	pass


func is_follow_target_paused() -> bool:
	return follow_target_pause


func set_follow_target_paused(value : bool):
	pass


func _is_seq_paused() -> bool:
	return sequence_pause


func _set_seq_paused(value : bool):
	pass


func _get_shake_offset_duration() -> float:
	return shake_offset_duration


func _set_shake_offset_duration(value : float):
	pass


func _get_shake_offset_intensity() -> Vector2:
	return shake_offset_intensity


func _set_shake_offset_intensity(value : Vector2):
	pass


func _get_shake_rotation_duration() -> float:
	return shake_rotation_duration


func _set_shake_rotation_duration(value : float):
	pass


func _get_shake_rotation_intensity() -> float:
	return shake_rotation_intensity


func _set_shake_rotation_intensity(value : float):
	pass


func _get_shake_zoom_duration() -> float:
	return shake_zoom_duration


func _set_shake_zoom_duration(value : float):
	pass


func _get_shake_zoom_intensity() -> Vector2:
	return shake_zoom_intensity


func _set_shake_zoom_intensity(value : Vector2):
	pass


func get_target() -> CamTarget2D:
	return target


func set_target(value : CamTarget2D):
	pass
