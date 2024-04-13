class_name H_CineCam2D
extends Camera2D
## This help document refers to [CineCam2D]!
##
## This is the GDExtension Main camera for the 2D Space
## BLA BLA
##
## @tutorial:            https://www.youtube.com
## @tutorial(Tutorial2): https://www.youtube.com

## strength description
var shake_strength : float : 
	get : return _get_shake_strength()
	set (value): _set_shake_strength(value)

## duration description
var shake_duration : float :
	get : return _get_shake_duration()
	set (value): _set_shake_duration(value)


## The description of a signal.
signal shake_ended


## As the following function is documented, even though its name starts with
## an underscore, it will appear in the help window.
func shake_cam():
	pass

# Getter
func _get_shake_strength() -> float: 
	return 0.0

# Setter
func _set_shake_strength(value : float):
	pass

# Getter 2
func _get_shake_duration() -> float:
	return 0.0

# Setter 2
func _set_shake_duration(value : float):
	pass
