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
unary operator: '[]', '!', '~', '+', '-', '++', '--'; //'++' and '--' can only put before number, '[]' is for array
binary operator: 
	 arithmetic operation: '+', '-', '*', '/', '%', '<<', '>>';
	 logical operation: '<', '>', '<=', '>=', '==', '!=', '&', '|', '&&', '||';
	 assignment operation: '=', '+=', '-=', '*=', '/=', '%=';
ternary operator: ?!
*/

Feedback cal_expression(std::string exp){
	exp = exp + ";";
	Expression expr(exp);
	Feedback fb = expr.expression();
	return fb;
}

#endif