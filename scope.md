# Gorts_C

## Maths

1 + 2 = 1 ☝ 2
1 - 2 = 1 ☟ 2
1 / 2 = 1 ✂ 2
1 * 2 = 1 ☼ 2
1 * 2 / (5 - 3) = 1 ☼ 2 ✂ 👍︎5 ☟ 3👎︎

## Variable assignment:
Normal: new var var_1 = "hello world" 
Gort_c: 🖉 var_1 ✍ "hello world"
Normal: new global var var_2 = "hello globe" 
Gort_c: 🖉 🕮 var_2 ✍ "hello globe"

##Casting:
Casting is implicit in gort_c, not requiring the user to manually specify variable types
string + int = string
int + string = string
int + float = float
float + int = float

## Defining functions
Normal:
def process_string(input_1):
{
	return_value = input_1 + " wowza"
	return return_value
}

Gort_C:
📪︎ process_string👍︎input_1👎︎:
	🖉 return_value ✍ input_1 ☝ " : added_string"
	💣︎ return_value

## If Statements
Normal 
if (foo = bar):
	faz = fin

Gort_c:
✆ 👍︎foo 🖏 faz👎︎:
	faz ✍ fin
