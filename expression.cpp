#include <iostream>
#include "expression.hpp"
#define mesg_op_not_match "Error: Operator not match between "
#define mesg_string_not_op "Error: String cannot execute the operator "
#define mesg_divide_by_zero "Error: Cannot divide by zero."
#define mesg_left_cannot_modify "Error: Left expression cannot be modified."
using namespace std;


Feedback cal_function(token t) //****call interpreter
{
	cout << "cal_function: " << t.value << ". Para number: " << (int)t.number << endl;
	for (int i = 0; i < (int)t.number; i++){
		cout << t.para[i].type << " " << t.para[i].number << " " << t.para[i].value << endl;
	}
	token t1;
	t1.type = '1';
	t1.number = 3;
	return t1;
}

string int2string(int n){
	stringstream ss;
	string s;
	ss << n;
	ss >> s;
	return s;
}

string double2string(double n){
	stringstream ss;
	string s;
	ss << n;
	ss >> s;
	return s;
}


string Expression::get_type(token t){
	stringstream ss;
	string s;
	switch (t.type){
	case '1': return "Integer";
	case '2': return "Double";
	case '3': return "String";
	case '4': return "Function";
	case 0: 
	case '0': return "Null";
	case 1: return "<=";
	case 2: return ">=";
	case 3: return "==";
	case 4: return "!=";
	case 5: return "&&";
	case 6: return "||";
	case 7: return "<<";
	case 8: return ">>";
	case 9: return "++";
	case 10: return "--";
	case 11: return "===";
	case 12: return "!==";
	case 13: return "+=";
	case 14: return "-=";
	case 15: return "*=";
	case 16: return "/=";
	case 17: return "%=";
	default: 
		ss << t.type;
		ss >> s;
		return s;
	}
}


int Expression::isNumber(token token1, token token2){
	if (token1.type == '1' && token2.type == '1'){
		return 1; //both are integer
	}
	else if (token1.type == '1' && token2.type == '2' || token1.type == '2' && token2.type == '1' || token1.type == '2' && token2.type == '2'){
		return 2; //both are double or one integer one double
	}
	else if (token1.type == '3' && token2.type == '3'){
		return 3; //both are string
	}
	else {
		return 0; //operator not match
	}
}

Feedback Expression::add(token left, token right){
	int match = isNumber(left, right);
	if (match == 1){
		left.number += right.number;
	}
	else if (match == 2){
		left.type = '2';
		left.number += right.number;
	}
	else if (match == 3){
		left.value += right.value; //string
	}
	else //string + number -> string + string -> string
	{
		if (left.type == '3'){
			if (right.type == '1') left.value += int2string((int)right.number);
			else left.value += double2string(right.number);
		}
		else if (right.type == '3'){
			if (left.type == '1') left.value = int2string((int)left.number) + right.value;
			else left.value = double2string(left.number) + right.value;
			left.type = '3';
		}
	}
	Feedback fb(left);
	return fb;
}

Feedback Expression::minus(token left, token right){
	int match = isNumber(left, right);
	Feedback fail_fb;
	if (match == 3){
		fail_fb.reason = mesg_string_not_op;
		fail_fb.reason += "-.";
		return fail_fb;
	}
	if (match == 1){
		left.number -= right.number;
	}
	else{
		left.type = '2';
		left.number -= right.number;
	}
	Feedback fb(left);
	return fb;
}

Feedback Expression::multi(token left, token right){
	int match = isNumber(left, right);
	Feedback fail_fb;
	if (match == 3){
		fail_fb.reason = mesg_string_not_op;
		fail_fb.reason += "*.";
		return fail_fb;
	}
	if (match == 1){
		left.number *= right.number;
	}
	else if (match == 2){
		left.type = '2';
		left.number *= right.number;
	}
	Feedback fb(left);
	return fb;
}

Feedback Expression::divide(token left, token right){
	int match = isNumber(left, right);
	Feedback fail_fb;
	if (match == 3){
		fail_fb.reason = mesg_string_not_op;
		fail_fb.reason += "/.";
		return fail_fb;
	}
	token d = right;
	if (d.number == 0){
		fail_fb.reason = mesg_divide_by_zero;
		return fail_fb;
	}
	if (match == 1){
		int result = (int)left.number / (int)d.number;
		left.number = result;
	}
	else{
		left.type = '2';
		left.number /= d.number;
	}
	Feedback fb(left);
	return fb;
}

Feedback Expression::mod(token left, token right){
	int match = isNumber(left, right);
	Feedback fail_fb;
	if (match != 1){
		fail_fb.reason = "Error: only integer can execute '%'!";
		return fail_fb;
	}
	else{
		int result = (int)left.number % (int)right.number;
		left.number = result;
	}
	Feedback fb(left);
	return fb;
}


Expression::Expression(hashMap &symbol, string exp): ts(symbol){
	ts.putstream(exp);
}


bool Expression::number_string(token token1, token token2){
	if ((token1.type == '1' || token1.type == '2') && token2.type == '3' || (token2.type == '1' || token2.type == '2') && token1.type == '3'){
		return true; //one is number and one is string
	}
	else return false;
}

Feedback Expression::expression(){
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;
	string s;
	int in;

	rtn_fb = condition();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;


	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	if (true){
		if (t.type == '=' || t.type == 13 || t.type == 14 || t.type == 15 || t.type == 16 || t.type == 17){
			if (left.modifiable == false){
				fail_fb.reason = mesg_left_cannot_modify;
				return fail_fb;
			}
			else{
				rtn_fb = expression();
				if (!rtn_fb.succeed) return rtn_fb;
				else right = rtn_fb.rtn_value;

				match = isNumber(left, right);
				if (match == 0 && t.type != '=' && !(t.type == 13 && number_string(left, right))){
					fail_fb.reason = mesg_op_not_match;
					fail_fb.reason += get_type(left) + " and " + get_type(right) + ".(Operator: " + get_type(t) + ")";
					return fail_fb;
				}
			}
		}
		switch (t.type){
		case '=':
			if (left.var_name == ""){
				fail_fb.reason = "Error: left of expression is not a valid var!";
			}
			s = left.var_name;
			in = left.array_index;
			left = right;
			left.modifiable = false;
			left.var_name = s;
			left.array_index = in;
			ts.modify_value(left);
			break;
		case 13:
			rtn_fb = add(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			left.modifiable = false;
			ts.modify_value(left);

			break;
		case 14:
			rtn_fb = minus(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			left.modifiable = false;
			ts.modify_value(left);

			break;
		case 15:
			rtn_fb = multi(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			left.modifiable = false;
			ts.modify_value(left);

			break;
		case 16:
			rtn_fb = divide(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			left.modifiable = false;
			ts.modify_value(left);

			break;
		case 17:
			rtn_fb = mod(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			left.modifiable = false;
			ts.modify_value(left);

			break;
		default:
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
		if (t.type == '=' || t.type == 13 || t.type == 14 || t.type == 15 || t.type == 16 || t.type == 17){
			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			ts.putback(t);
			if (t.type != ';'){
				fail_fb.reason = "Error: a ';' is exprected!";
				return fail_fb;
			}
			else{
				Feedback fb(left);
				return fb;
			}
		}
	}
}

Feedback Expression::condition() //'? !'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, t2, right, most_right;

	rtn_fb = logi_or();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	if (t.type == '?'){
		left.modifiable = false;
		if (left.type != '1' && left.type != '2'){
			fail_fb.reason = "Error: string cannot be executed before '?' in the '? !' operator!";
			return fail_fb;
		}

		rtn_fb = logi_or();
		if (!rtn_fb.succeed) return rtn_fb;
		else right = rtn_fb.rtn_value;

		rtn_fb = ts.get();
		if (!rtn_fb.succeed) return rtn_fb;
		else t2 = rtn_fb.rtn_value;

		if (t2.type != ':'){
			fail_fb.reason = "Error: a '!' is expected!";
			return fail_fb;
		}

		rtn_fb = logi_or();
		if (!rtn_fb.succeed) return rtn_fb;
		else most_right = rtn_fb.rtn_value;

		if (left.number){
			left = right;
		}
		else left = most_right;
		return left;
	}
	else{
		ts.putback(t);
		Feedback fb(left);
		return fb;
	}
}

Feedback Expression::logi_or() //'||'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = logi_and();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	while (true){
		if (t.type == 6){
			rtn_fb = logi_and();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);

			if (match == 1 || match == 2){
				left.type == '1';
				left.number = left.number || right.number;

				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}
			else{
				fail_fb.reason = "Error: '||' operation only can be executed between two Number!";
				return fail_fb;
			}
			left.modifiable = false;
		}
		else{
			/*char c = t.type;
			if (c != '+' && c != '-' && c != '*' && c != '/' && c != ')' && c != ';' && c != ',' && c != '%' && c != '|' && c != '&' && c != '<' && c != '>' && '^' && c != '?' && c != '!' && c != '=' && c != ':'){
				fail_fb.reason = "Error: bad token between an expression and " + get_type(rtn_fb.rtn_value) + ".";
				return fail_fb;
			}
			*/
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::logi_and() // '&&'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = bit_or();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	while (true){
		if (t.type == 5){
			rtn_fb = bit_or();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);

			if (match == 1 || match == 2){
				left.type == '1';
				left.number = left.number && right.number;

				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}
			else{
				fail_fb.reason = "Error: '&&' operation only can be executed between two Number!";
				return fail_fb;
			}
			left.modifiable = false;
		}
		else{
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::bit_or() // '|'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = bit_xor();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	while (true){
		if (t.type == '|'){
			rtn_fb = bit_xor();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			if (left.type == '1' && right.type == '1'){
				left.number = (int)left.number | (int)right.number;
				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}

			else{
				fail_fb.reason = "Error: bit_or operation only can be executed between two Integer!";
				return fail_fb;
			}
			left.modifiable = false;
		}
		else{
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::bit_xor() // '^'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = bit_and();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	while (true){
		if (t.type == '^'){
			rtn_fb = bit_and();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			if (left.type == '1' && right.type == '1'){
				left.number = (int)left.number ^ (int)right.number;
				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}

			else{
				fail_fb.reason = "Error: bit_xor operation only can be executed between two Integer!";
				return fail_fb;
			}
			left.modifiable = false;
		}
		else{
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::bit_and() // '&'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = compare_equ();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	while (true){
		if (t.type == '&'){
			rtn_fb = compare_equ();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			if (left.type == '1' && right.type == '1'){
					left.number = (int)left.number & (int)right.number;
				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}

			else{
				fail_fb.reason = "Error: bit_and operation only can be executed between two Integer!";
				return fail_fb;
			}
			left.modifiable = false;
		}
		else{
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::compare_equ() // '==' '!='
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = compare_nonequ();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	while (true){
		if (t.type == 3 || t.type == 4){
			rtn_fb = compare_nonequ();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);
			if (match == 0){
				fail_fb.reason = mesg_op_not_match;
				fail_fb.reason += get_type(left) + " and " + get_type(right) + ".(Operator: " + get_type(t) + ")";
				return fail_fb;
			}
			left.modifiable = false;
		}
		switch (t.type){
		case 3:
			left.type = '1';
			if (match == 3){
				left.number = left.value == right.value;
			}
			else{
				left.number = left.number == right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case 4:
			left.type = '1';
			if (match == 3){
				left.number = left.value != right.value;
			}
			else{
				left.number = left.number != right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		
		default:
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::compare_nonequ() // '>' '<' '>=' '<='
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = shift();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	while (true){
		if (t.type == '<' || t.type == '>' || t.type == 1 || t.type == 2){
			rtn_fb = shift();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);
			if (match == 0){
				fail_fb.reason = mesg_op_not_match;
				fail_fb.reason += get_type(left) + " and " + get_type(right) + ".(Operator: " + get_type(t) + ")";
				return fail_fb;
			}
			left.modifiable = false;
		}
		switch (t.type){
		case '<':
			left.type = '1';
			if (match == 3){				
				left.number = left.value < right.value;
			}
			else{				
				left.number = left.number < right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case '>':
			left.type = '1';
			if (match == 3){
				left.number = left.value > right.value;
			}
			else{
				left.number = left.number > right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case 1:
			left.type = '1';
			if (match == 3){
				left.number = left.value <= right.value;
			}
			else{
				left.number = left.number <= right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case 2:
			left.type = '1';
			if (match == 3){
				left.number = left.value >= right.value;
			}
			else{
				left.number = left.number >= right.number;
			}

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		default:
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::shift() // '<<' '>>'
{
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = add_minus();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	while (true){
		if (t.type == 7 || t.type == 8){
			rtn_fb = add_minus();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;
			left.modifiable = false;

			if (left.type == '1' && right.type == '1'){
				if (t.type == 7){
					left.number = (int)left.number << (int)right.number;
				}
				else{
					left.number = (int)left.number >> (int)right.number;
				}
				rtn_fb = ts.get();
				if (!rtn_fb.succeed) return rtn_fb;
				else t = rtn_fb.rtn_value;
			}

			else{
				fail_fb.reason = "Error: shift operation only can be executed between two Integer!";
				return fail_fb;
			}
		}
		else{
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::add_minus(){
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = term();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	int match;
	while (true){
		if (t.type == '+' || t.type == '-'){
			rtn_fb = term();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);
			if (match == 0 && !(t.type == '+' && number_string(left, right))){
				fail_fb.reason = mesg_op_not_match;
				fail_fb.reason += get_type(left) + " and " + get_type(right) + ".(Operator: " + get_type(t) + ")";
				return fail_fb;
			}
			left.modifiable = false;
		}
		switch (t.type){
		case '+':
			rtn_fb = add(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;
			
			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case '-':
			rtn_fb = minus(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		default:
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::term(){
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	token left, t, right;

	rtn_fb = primary();
	if (!rtn_fb.succeed) return rtn_fb;
	else left = rtn_fb.rtn_value;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;
	
	struct token d;
	int match;
	while (true){	
		if (t.type == '*' || t.type == '/' || t.type == '%'){
			rtn_fb = primary();
			if (!rtn_fb.succeed) return rtn_fb;
			else right = rtn_fb.rtn_value;

			match = isNumber(left, right);
			if (match == 0){
				fail_fb.reason = mesg_op_not_match;
				fail_fb.reason += get_type(left) + " and " + get_type(right) + ".(Operator: " + get_type(t) + ")";
				return fail_fb;
			}
			left.modifiable = false;
		}
		switch (t.type){
		case '*':
			rtn_fb = multi(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case '/':
			rtn_fb = divide(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;
		case '%':
			rtn_fb = mod(left, right);
			if (!rtn_fb.succeed) return rtn_fb;
			else left = rtn_fb.rtn_value;

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t = rtn_fb.rtn_value;

			break;			
		default:
			ts.putback(t);
			Feedback fb(left);
			return fb;
		}
	}
}

Feedback Expression::primary(){
	token t, t_temp;
	struct token d;
	Feedback rtn_fb;
	Feedback fail_fb("Error£¡");
	Feedback success_fb;

	rtn_fb = ts.get();
	if (!rtn_fb.succeed) return rtn_fb;
	else t = rtn_fb.rtn_value;

	switch (t.type){
	case '(': //handle '(' expression ')'
	{
		rtn_fb = expression();
		if (!rtn_fb.succeed) return rtn_fb;
		else d = rtn_fb.rtn_value;

		rtn_fb = ts.get();
		if (!rtn_fb.succeed) return rtn_fb;
		else t = rtn_fb.rtn_value;

		if (t.type != ')'){
			fail_fb.reason = "Error: ')' expected after an expression.";
			return fail_fb;
		}
		success_fb.rtn_value = d;
		return success_fb;
	}
	case '1':
	case '2':
	case '3':
		success_fb.rtn_value = t;
		return success_fb;
	case '4': //function
		t.para = new token[(int)t.number];
		for (int i = 0; i < (int)t.number; i++){
			rtn_fb = expression();
			if (!rtn_fb.succeed) return rtn_fb;
			else d = rtn_fb.rtn_value;
			t.para[i] = d;

			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t_temp = rtn_fb.rtn_value;

			if (t_temp.type == ')' && i < (int)t.number - 1 || t_temp.type != ')' && i == (int)t.number - 1){
				fail_fb.reason = "Error: the number of parameter in funtion " + t.value + " is not correct! The right number: " + int2string((int)t.number);
				return fail_fb;
			}
			else if (t_temp.type != ',' && i < (int)t.number - 1){
				fail_fb.reason = "Error: ',' between parameters in a funtion is expected!";
				return fail_fb;
			}
		}
		if ((int)t.number == 0){
			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t_temp = rtn_fb.rtn_value;

			if (t_temp.type != ')'){
				fail_fb.reason = "Error: a ')' is expected in the end of function";
				return fail_fb;
			}
		}
		rtn_fb = cal_function(t); 
		if (!rtn_fb.succeed){
			rtn_fb.reason += "(In function " + t.value + ")";
			return rtn_fb;
		}
		else {
			rtn_fb.rtn_value.modifiable = false;
			return rtn_fb;
		}
	case '7': //array
		rtn_fb = expression();
		if (!rtn_fb.succeed) return rtn_fb;
		else d = rtn_fb.rtn_value;

		if (d.type != '1'){
			fail_fb.reason = "Error: array can only be indexed by integer! This index: " + get_type(d) + ".";
			return fail_fb;
		}
		else{
			rtn_fb = ts.get();
			if (!rtn_fb.succeed) return rtn_fb;
			else t_temp = rtn_fb.rtn_value;

			if (t_temp.type != ']'){
				fail_fb.reason = "Error: a ']' is expected in the array!";
				return fail_fb;
			}
			t.array_index = (int)d.number;
			rtn_fb = ts.get_value(t);
			if (!rtn_fb.succeed) return rtn_fb;
			else{
				rtn_fb.rtn_value.modifiable = true;
				rtn_fb.rtn_value.var_name = t.var_name;
				rtn_fb.rtn_value.array_index = t.array_index;
				return rtn_fb;
			}
		}
		rtn_fb = ts.get();
		if (!rtn_fb.succeed) return rtn_fb;
		else t_temp = rtn_fb.rtn_value;
	case '-':
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type == '3'){
			fail_fb.reason = mesg_string_not_op;
			fail_fb.reason += "-(Unary Minus Operator).";
			return fail_fb;
		}
		else{
			rtn_fb.rtn_value.number = -rtn_fb.rtn_value.number;
			rtn_fb.rtn_value.modifiable = false;
			return rtn_fb;
		}
	case '+':
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type == '3'){
			fail_fb.reason = mesg_string_not_op;
			fail_fb.reason += "+(Unary Plus Operator).";
			return fail_fb;
		}
		else{
			rtn_fb.rtn_value.modifiable = false;
			return rtn_fb;
		}
	case '!': //logical negation
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type == '3'){
			fail_fb.reason = mesg_string_not_op;
			fail_fb.reason += "!(logical negation).";
			return fail_fb;
		}
		else{
			success_fb.rtn_value.type = '1';
			success_fb.rtn_value.number = rtn_fb.rtn_value.number ? 0 : 1;
			rtn_fb.rtn_value.modifiable = false;
			return success_fb;
		}
	case '~': //bit negation
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type != '1'){
			fail_fb.reason = "Error: Only integer can execute the operator ";
			fail_fb.reason += "~(bit negation).";
			return fail_fb;
		}
		else{
			success_fb.rtn_value.type = '1';
			success_fb.rtn_value.number = ~(int)rtn_fb.rtn_value.number;
			rtn_fb.rtn_value.modifiable = false;
			return success_fb;
		}
	case 9: //'++' before number
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type != '1' && rtn_fb.rtn_value.type != '2'){
			fail_fb.reason = "Error: Only number can execute the operator: ++.";
			return fail_fb;
		}
		else{
			success_fb.rtn_value.number++;
			rtn_fb.rtn_value.modifiable = false;
			return success_fb;
		}
	case 10: //'--' before number
		rtn_fb = primary();
		if (!rtn_fb.succeed) return rtn_fb;
		else if (rtn_fb.rtn_value.type != '1' && rtn_fb.rtn_value.type != '2'){
			fail_fb.reason = "Error: Only number can execute the operator: --.";
			return fail_fb;
		}
		else{
			success_fb.rtn_value.number--;
			rtn_fb.rtn_value.modifiable = false;
			return success_fb;
		}
	default:
		fail_fb.reason = "Error: primary expected.";
		return fail_fb;
	}
}

Expression::~Expression(){

}