#ifndef _CALEXPPRESSION_HPP
#define _CALEXPPRESSION_HPP
#include "expression.hpp"

/*Feedback cal_expression(std::string exp); (for interpreter)

direction for use:
Feedback fb = cal_expression("1+2*3+5");
if (!fb.succeed) cout << fb.reason;
else if (fb.rtn_value.type == '1') cout << "integer" << fb.rtn_value.number; //integer
else if (fb.rtn_value.type == '2') cout << "double" << fb.rtn_value.number; //double
else if (fb.rtn_value.type == '3') cout << "string" << fb.rtn_value.value; //string
else cout << "null" << endl;

data type support:
integer, double, bool, string, var, function, array

Operation support:
unary operator: '[]', '!', '~', '+', '-', '++', '--'; //'++' and '--' can be put before number or after number, '[]' is for array
binary operator: 
	 arithmetic operation: '+', '-', '*', '/', '%', '<<', '>>';
	 logical operation: '<', '>', '<=', '>=', '==', '!=', '&', '|', '&&', '||';
	 assignment operation: '=', '+=', '-=', '*=', '/=', '%=';
ternary operator: ?!

String's method:
String.length
String.get(Integer) Integer can be an expression  eg: "abc".get(1+1);

class's method:
class.var: t.type = 18; t.value = class_name; t.var_name = var_name;
class.func: t.type = 19; t.value = calss_name; t.var_name = func_name; t.number = # of para; para = parameters;
*/

Feedback cal_expression(std::string exp, hashMap &symbol){
	exp = exp + ";";
	Expression expr(symbol, exp);
	Feedback fb = expr.expression();
	return fb;
}

#endif