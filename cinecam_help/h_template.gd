class_name H_Template
extends Node
#Inherits: Node < Object
#MAIN_DESCRIPTION
## This help document refers to [Template] class!
##
## Description 1. line [br]
## Description 2. line [br]
## Description 3. line [br]
## Description 4. line [br]
## Description 5. line [br]
##
## @tutorial(Link1): https://lnk.bio/LumiToad
## @tutorial(Link2): https://www.linkedin.com/in/LumiToad/
## @tutorial(Link3): https://github.com/LumiToad
#END_OF_DESCRIPTION


#CONSTANT_DESCRIPTION
const CONST_A = 0.0
#END_OF_DESCRIPTION


#CONSTANT_DESCRIPTION
const CONST_B = 10.0
#END_OF_DESCRIPTION


#SIGNAL_DESCRIPTION
## The description of a signal.
signal signal_a
#END_OF_DESCRIPTION


#ENUM_DESCRIPTION
enum Enum_A{
	## Entry Description A.ONE
	ONE = 1,
	## Entry Description A.TWO
	TWO = 2,
	## Entry Description A.THREE
	THREE = 3,
	## Entry Description A.FOUR
	FOUR = 4
}
#END_OF_DESCRIPTION


#PROPERTY_DESCRIPTION
## property_a description
var property_a : float :
	get: return _get_property_a()
	set(value): _set_property_a(value)
#END_OF_DESCRIPTION


#PROPERTY_DESCRIPTION
## property_b description
var property_b : float :
	get: return _get_property_b()
	set(value): _set_property_b(value)
#END_OF_DESCRIPTION


#FUNCTION_DESCRIPTION
## function_a description [br]
## second line
func function_a():
	pass
#END_OF_DESCRIPTION


#FUNCTION_DESCRIPTION	
## function_b description [br]
## second line
func function_b():
	pass
#END_OF_DESCRIPTION


#FUNCTION_DESCRIPTION
## function_c description [br]
## second line
func function_c():
	pass
#END_OF_DESCRIPTION


# Getter
func _get_property_a() -> float: 
	return 0.0


# Setter
func _set_property_a(value : float):
	pass


# Getter 2
func _get_property_b() -> float:
	return 0.0


# Setter 2
func _set_property_b(value : float):
	pass
