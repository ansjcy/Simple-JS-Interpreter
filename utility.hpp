#ifndef _UTILITY_HPP
#define _UTILITY_HPP
#include <string>


/*
token:
null: type = '0'
integer: type = '1'  number = integer_value
boolean: type = '5'  number = 0 or 1
double: type = '2'  number = double_value;
string: type = '3'  value = string_value;
function: type = '4'  value = function_name  number = # of parameter;
null: type = '0'
array: type = '7'  var_name = val_name  number = index;
undefined: type = '6'
class: '8'  value = class_name;
class.var: t.type = 18; t.value = class_name; t.var_name = var_name;
class.func: t.type = 19; t.value = calss_name; t.var_name = func_name; t.number = # of para; para = parameters;

operator of one character: type = operator_name;
operator of two character: type =
1:'<='  2:'>='  3:'=='  4:'!='  5:'&&'  
6:'||'  7:'<<'  8:'>>'  9:'++'  10:'--'
11:'==='  12:'!=='  13£º'+='  14:'-='  15:'*='  
16:'/='  17:'%='  18:'keyword' or class.var  19: class.function

keyword:
"typeof"  "undefined"  "null"  "true"  "false"  ".length"  ".get"

*/

struct token{
	char type = '0';  // '0':null '1'£ºint '2':double '3':string '4':function '5':boolean  '6':undefined  '7':array  '8':class
	std::string value = "";
	double number = 0;
	token *para;
	bool modifiable = false; //for assignment operator
	std::string var_name = "";
	int array_index = 0;
};
struct Feedback{
	bool succeed;
	struct token rtn_value;
	std::string reason;
	Feedback() : succeed(true){}
	Feedback(struct token return_value) : succeed(true), rtn_value(return_value) {}
	Feedback(std::string fail_reason) : succeed(false), reason(fail_reason) {}
};

#endif