class_name H_CineCam3D
extends Camera3D
#Inherits: Camera3D < Node3D < Node < Object
#MAIN_DESCRIPTION
## This help document refers to [CineCam3D] class! [br]
## 
## Main Camera for the [CineCam3D]. [br]
## Features include camera shaking, camera blending to [H_VirtualCam3D], targeting [H_CamTarget3D] and more. [br]
##
## @tutorial(Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki
## @tutorial(This page in Wiki on GitHub): https://github.com/LumiToad/GDExtension_CineCam/wiki/CineCam3D-class-reference-%E2%80%90-CineCam-wiki
## @tutorial(Camera shaking): https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Easy-camera-shaking-%E2%80%90-CineCam-wiki
#END_OF_DESCRIPTION


## Emitted when a blend is completed. [br]
## Will not be emitted for [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.TARGET_BLEND]. [br]
signal blend_completed


## Emitted when a blend is paused. [br]
## Will not be emitted for [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.TARGET_BLEND]. [br]
signal blend_paused


## Emitted when a blend is resumed. [br]
## Will not be emitted for [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.TARGET_BLEND]. [br]
signal blend_resumed


## Emitted when a blend is completed. [br]
## Will not be emitted for [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.TARGET_BLEND]. [br] 
signal blend_started


## Emitted when any registered [H_VirtualCam3D] priority became higher than the previously highest priority [H_VirtualCam3D]. [br] 
## Returns the [H_VirtualCam3D] with the now highest priority as an integer. [br]
## See [member H_VirtualCam3D.priority]. [br]
## [br]
## It doesn't matter, if this [H_CineCam3D] acts on it. [br] 
signal prioritized_vcam3d_changed(vcam3d: Object, priority: int)


## Emitted when a sequence of blends, using [CamSequence3D] was completed. [br]
## [br]
signal sequence_completed


## Emitted when a sequence of blends, using [CamSequence3D] was paused. [br]
## See [method H_CineCam3D.seq_pause]. [br]
signal sequence_paused


## Emitted when a sequence of blends, using [CamSequence3D] was resumed. [br]
## See [method H_CineCam3D.seq_resume]. [br]
signal sequence_resumed


## Emitted when a sequence of blends, using [CamSequence3D] was started. See: [br]
## - [method H_CineCam3D.start_sequence] [br]
## - [method H_CineCam3D.start_sequence_at] [br]
## - [method H_CineCam3D.seq_blend_to] [br]
## - [method H_CineCam3D.seq_blend_prev] [br]
## - [method H_CineCam3D.seq_blend_next] [br]
signal sequence_started


## Emitted when a sequence of blends, using [CamSequence3D] was stopped. [br]
## See [method H_CineCam3D.seq_stop]. [br]
signal sequence_stopped


## Emitted when the camera shake, using its offset, has ended. [br]
## See [method H_CineCam3D.shake_offset]. [br]
signal shake_offset_ended


## Emitted when the camera shake, using its offset, is started. [br]
## See [method H_CineCam3D.shake_offset]. [br]
signal shake_offset_started


## Emitted when the camera shake, using its rotation, has ended. [br]
## See [method H_CineCam3D.shake_rotation]. [br]
signal shake_rotation_ended


## Emitted when the camera shake, using its rotation, has started. [br]
## See [method H_CineCam3D.shake_rotation]. [br]
signal shake_rotation_started


## Emitted when the camera shake, using its fov, has ended. [br]
## See [method H_CineCam3D.shake_fov]. [br]
signal shake_fov_ended


## Emitted when the camera shake, using its fov, has started. [br]
## See [method H_CineCam3D.shake_fov]. [br]
signal shake_fov_started


## Setings for [member H_CineCam3D.follow_mode]. [br]
## PRIO refers to the [H_VirtualCam3D] with the highest priority in the [SceneTree] only! [br]
## TARGET is for [H_CamTarget3D] if it is set in [member H_CineCam3D.target]! [br]
enum  FollowMode{
	## Won't Follow anything. [br]
	OFF = 0,
	## Will constantly follow the it. [br]
	PRIO = 1,
	## Will follow it once. [br]
	PRIO_ONESHOT = 2,
	## Will blend towards it once, using the data from [member H_VirtualCam3D.blend_data]. [br]
	## If none are given, [member H_CineCam3D.blend_data] will be used. [br]
	PRIO_BLEND = 3,
	## Will constantly follow the target. [br]
	TARGET = 4,
	## Will constantly blend towards the target. [br]
	TARGET_BLEND = 5
}


## The default blending data for this CineCam3D. [br]
## Only used as default value for blending to [H_VirtualCam3D] and only if it has no [H_BlendData3D] set. [br]
var blend_data : BlendData3D :
	get: return _get_blend_data()
	set(value): _set_blend_data(value)


## Getter / Setter for pausing the tween that is used for camera blends. [br]
## See [method H_CineCam3D.pause_blend]. [br]
var blend_pause : bool = false :
	get: return _is_blend_paused()
	set(value): _set_blend_paused(value)


## Getter / Setter for the current [H_CamSequence3D]. [br]
var current_sequence : CamSequence3D :
	get: return get_current_sequence()
	set(value): set_current_sequence(value)


## Getter / Setter for the current follow mode property. [br]
## See [enum H_CineCam3D.FollowMode] [br]
var follow_mode : FollowMode = 0 :
	get: return get_follow_mode()
	set(value): set_follow_mode(value)


var follow_prio_pause : bool = false :
	get: return is_follow_prio_paused()
	set(value): set_follow_prio_paused(value)


## Getter / Setter for pausing the follow target feature. [br]
## See [constant H_CineCam3D.TARGET], [constant H_CineCam3D.TARGET_BLEND] and [method H_CineCam3D.pause_follow_target]. [br]
var follow_target_pause : bool = false :
	get: return is_follow_target_paused()
	set(value): set_follow_target_paused(value)


## Getter / Setter for the look at target feature. [br]
## If this is set, the camera will look towards the given [H_CamTarget3D], regardless of other rotations. [br]
## Can be used with rotation shaking. [br]
var look_at_target : CamTarget3D :
	get: return get_look_at_target()
	set(value): set_look_at_target(value)


## Getter / Setter for pausing the camera sequence feature. [br]
## See [H_CamSequence3D] and [method H_CineCam3D.start_sequence]. [br]
var sequence_pause : bool = false :
	get: return _is_seq_paused()
	set(value): _set_seq_paused(value)


## Getter / Setter for the shake offset duration. [br]
## Can be increased during a shake to increase its duration. [br]
var shake_offset_duration : float = 0.0 :
	get: return _get_shake_offset_duration()
	set(value): _set_shake_offset_duration(value)


## Getter / Setter for the shake offset intensity. [br]
## Can be increased during a shake to increase its intensity. [br]
var shake_offset_intensity : Vector3 = Vector3() :
	get: return _get_shake_offset_intensity()
	set(value): _set_shake_offset_intensity(value)


## Getter / Setter for the shake rotation duration. [br]
## Can be increased during a shake to increase its duration. [br]
var shake_rotation_duration : float = 0.0 :
	get: return _get_shake_rotation_duration()
	set(value): _set_shake_rotation_duration(value)


## Getter / Setter for the shake rotation intensity. [br]
## Can be increased during a shake to increase its intensity. [br]
var shake_rotation_intensity : float = 0.0 :
	get: return _get_shake_rotation_intensity()
	set(value): _set_shake_rotation_intensity(value)


## Getter / Setter for the shake fov duration. [br]
## Can be increased during a shake to increase its duration. [br]
var shake_fov_duration : float = 0.0 :
	get: return _get_shake_fov_duration()
	set(value): _set_shake_fov_duration(value)


## Getter / Setter for the shake fov intensity. [br]
## Can be increased during a shake to increase its intensity. [br]
var shake_fov_intensity : Vector3 = Vector3() :
	get: return _get_shake_fov_intensity()
	set(value): _set_shake_fov_intensity(value)


## Getter / Setter for the current [H_CamTarget3D]. [br]
var target : CamTarget3D :
	get: return get_target()
	set(value): set_target(value)


## Applies all data from the given [H_VirtualCam3D] under Camera3D Section to this [H_CineCam3D]. [br]
func apply_vcam3d_data(vcam3d: VirtualCam3D) -> String :
	return ""


## Blends towards the given [H_VirtualCam3D], using the given [H_BlendData3D]. [br]
## You can use [member H_CineCam3D.blend_data], [member H_VirtualCam3D.blend_data] or data from any other source. [br]
func blend_to(vcam3d : VirtualCam3D, blend_data : BlendData3D):
	pass


## Returns the first [H_VirtualCam3D] with the given id. [br]
## Will only search registered [H_VirtualCam3D]. [br]
## See [member H_VirtualCam3D.vcam_id]. [br]
func find_vcam_by_id(id : String) -> VirtualCam3D:
	return


## Returns the full duration of the current blend. [br]
## This is either [member H_BlendData3D.blend_by_value] if [member H_BlendData3D.blend_by] is set to [constant H_BlendData3D.DURATION] [br]
## or a calculated duration, if set to [constant H_BlendData3D.SPEED]. [br]
## Will return -1 if the blend has not started yet. [br]
func full_blend_duration() -> float:
	return 0.0


## Sets [member H_CineCam3D.blend_data] to the default data. [br]
func init_default_blend_data():
	pass


## This will pause the currently running blend. [br]
## Will only affect [method H_CineCam3D.blend_to], [constant H_CineCam3D.PRIO_BLEND] or anything from [H_CamSequence3D]. [br] 
func pause_blend():
	pass


## This will pause the follow priority feature. [br]
## See [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.PRIO]. [br] 
func pause_follow_prio():
	pass


## This will pause the follow target and target blend feature. [br]
## See [enum H_CineCam3D.FollowMode] [constant H_CineCam3D.TARGET] and [constant H_CineCam3D.TARGET_BLEND]. [br] 
func pause_follow_target():
	pass


## Returns the prioritized [H_VirtualCam3D]. [br]
func prioritized_vcam():
	pass


## Will teleport this Camera instantly to the given [H_VirtualCam3D]. [br]
func reposition_to_vcam(vcam : VirtualCam3D):
	pass


## Resumes the currently running blend, regardless of the current state. [br]
## Will only affect [method H_CineCam3D.blend_to], [constant H_CineCam3D.PRIO_BLEND] or anything from [H_CamSequence3D]. [br] 
func resume_blend():
	pass


## Resumes the currently running priority blend, regardless of the current state. [br]
## See [enum H_CineCam3D.FollowMode], [constant H_CineCam3D.PRIO]. [br]
func resume_follow_prio():
	pass


## Resumes the currently running target follow, regardless of the current state. [br]
## See [enum H_CineCam3D.FollowMode], [constant H_CineCam3D.TARGET] and [constant H_CineCam3D.TARGET_BLEND]. [br]
func resume_follow_target():
	pass


## Blends to the next [H_VirtualCam3D] in the current [member H_CineCam3D.current_sequence]. [br]
func seq_blend_next():
	pass


## Blends to the previous [H_VirtualCam3D] in the current [member H_CineCam3D.current_sequence]. [br]
func seq_blend_prev():
	pass


## Blends to the [H_VirtualCam3D] at the given index, in the current [member H_CineCam3D.current_sequence]. [br]
func seq_blend_to(idx: int):
	pass


## Pauses the current running sequence. Will not pause the blend, just the execution of the sequence. [br]
func seq_pause():
	pass


## Resumes the current running sequence, regardless of the current state. [br]
func seq_resume():
	pass


## Stops the current running sequence and resets its [member H_CamSequence3D.current_idx]. [br]
func seq_stop():
	pass


## Shakes the cam. [br]
## Takes a strength and a duration in seconds. [br]
func shake_offset(intensity : Vector3, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


## Shakes the cam. [br]
## Takes a strength and a duration in seconds. [br]
func shake_rotation(intensity : float, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


## Shakes the cam fov. [br]
## Takes a strength and a duration in seconds. [br]
func shake_fov(intensity : Vector3, duration : float, ease : Tween.EaseType, trans : Tween.TransitionType):
	pass


## Starts the sequence in [member H_CineCam3D.current_sequence]. [br]
## You can set the optional bool to true, to play the sequence in reverse. [br]
func start_sequence(backwards : bool = false):
	pass


## Starts the sequence in [member H_CineCam3D.current_sequence] at the given index. [br]
## You can set the optional bool to true, to play the sequence in reverse. [br]
func start_sequence_at(backwards : bool = false, index : int = 0):
	pass


## Toggles the currently running priority blend, considering its current state. [br]
## See [enum H_CineCam3D.FollowMode], [constant H_CineCam3D.PRIO]. [br]
func toggle_follow_prio():
	pass


## Toggles the currently running target follow,  considering its current state. [br]
## See [enum H_CineCam3D.FollowMode], [constant H_CineCam3D.TARGET] and [constant H_CineCam3D.TARGET_BLEND]. [br]
func toggle_follow_target():
	pass


## Getter for blend_data. [br]
func _get_blend_data() -> BlendData3D:
	return blend_data


## Setter for blend_data. [br]
func _set_blend_data(value : BlendData3D):
	pass


## Getter for blend_pause. [br]
func _is_blend_paused() -> bool:
	return blend_pause


## Setter for blend_pause. [br]
func _set_blend_paused(value : bool):
	pass


## Getter for current_sequence. [br]
func get_current_sequence() -> CamSequence3D:
	return current_sequence


## Setter for current_sequence. [br]
func set_current_sequence(value : CamSequence3D):
	pass


## Getter for follow_mode. [br]
func get_follow_mode() -> FollowMode:
	return follow_mode


## Setter for follow_mode. [br]
func set_follow_mode(value : FollowMode):
	pass


## Getter for follow_prio_paused. [br]
func is_follow_prio_paused() -> bool:
	return follow_prio_pause


## Setter for follow_prio_paused. [br]
func set_follow_prio_paused(value : bool):
	pass


## Getter for follow_target_paused. [br]
func is_follow_target_paused() -> bool:
	return follow_target_pause


## Setter for follow_target_paused. [br]
func set_follow_target_paused(value : bool):
	pass


## Getter for sequence_paused. [br]
func _is_seq_paused() -> bool:
	return sequence_pause


## Setter for sequence_paused. [br]
func _set_seq_paused(value : bool):
	pass


## Getter for shake_offset_duration. [br]
func _get_shake_offset_duration() -> float:
	return shake_offset_duration


## Setter for shake_offset_duration. [br]
func _set_shake_offset_duration(value : float):
	pass


## Getter for shake_offset_intensity. [br]
func _get_shake_offset_intensity() -> Vector3:
	return shake_offset_intensity


## Setter for shake_offset_intensity. [br]
func _set_shake_offset_intensity(value : Vector3):
	pass


## Getter for shake_rotation_duration. [br]
func _get_shake_rotation_duration() -> float:
	return shake_rotation_duration


## Setter for shake_rotation_duration. [br]
func _set_shake_rotation_duration(value : float):
	pass


## Getter for shake_rotation_intensity. [br]
func _get_shake_rotation_intensity() -> float:
	return shake_rotation_intensity


## Setter for shake_rotation_intensity. [br]
func _set_shake_rotation_intensity(value : float):
	pass


## Getter for shake_fov_duration. [br]
func _get_shake_fov_duration() -> float:
	return shake_fov_duration


## Setter for shake_fov_duration. [br]
func _set_shake_fov_duration(value : float):
	pass


## Getter for shake_fov_intensity. [br]
func _get_shake_fov_intensity() -> Vector3:
	return shake_fov_intensity


## Setter for shake_fov_intensity. [br]
func _set_shake_fov_intensity(value : Vector3):
	pass


## Getter for target. [br]
func get_target() -> CamTarget3D:
	return target


## Setter for target. [br]
func set_target(value : CamTarget3D):
	pass


## Getter for look_at_target. [br]
func get_look_at_target() -> CamTarget3D :
	return


## Setter for look_at_target. [br]
func set_look_at_target(value : CamTarget3D):
	pass
