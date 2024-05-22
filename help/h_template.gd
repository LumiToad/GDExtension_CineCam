class_name H_Template
extends Node
#Inherits: Node < Object
#MAIN_DESCRIPTION
## This help document refers to [Template] class! [br]
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


## Test 1 [br]
## Test 2
const CONST_A = 0.0

## Test 1
const CONST_B = 10.0


## The description of a signal. [br]
## Second Line. [br]
## Third Line. [br]
## Fourth Line. [br]
signal signal_a


## The description of a signal. [br]
## Second Line. [br]
## Third Line. [br]
## Fourth Line. [br]
signal signal_b


## Description of the entire enum. [br]
## Description of the entire enum. [br]
## Description of the entire enum. [br]
## Description of the entire enum. [br]
## Description of the entire enum. [br]
enum Enum_A{
	## Entry Description A.ONE [br]
	## second line
	ONE = 1,
	## Entry Description A.TWO
	TWO = 2,
	## Entry Description A.THREE
	THREE = 3,
	## Entry Description A.FOUR
	FOUR = 4
}


## Description of the entire enum.
enum Enum_B{
	## Entry Description A.ONE
	ONE = 1,
	## Entry Description A.TWO
	TWO = 2,
	## Entry Description A.THREE
}


## property_a description
var property_a : float :
	get: return _get_property_a()
	set(value): _set_property_a(value)


## property_b description [br]
## second line
var property_b : float :
	get: return _get_property_b()
	set(value): _set_property_b(value)


## function_a description [br]
## second line
## [codeblock]
## var kekse = 5
## return kekse
## [/codeblock]
func function_a():
	pass


## function_b description [br]
## second line [br]
## third line
func function_b(value_a : String, value_b : float):
	pass


## function_c description [br]
## second line
func function_c(value_a : int) -> Variant:
	return ""


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
