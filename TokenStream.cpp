#include "TokenStream.hpp"

using namespace std;

//var
Feedback TokenStream::get_value(token t){
	

	//t.type = '6';
	Feedback fb = symbol.getValue(t);
	fb.rtn_value.var_name = t.var_name;
	fb.rtn_value.modifiable = true;
	return fb;
}

Feedback TokenStream::get_value(string s){
	token var;
	var.type = '6';
	var.var_name = s;
	var.array_index = 0;
	Feedback fb = symbol.getValue(var);
	fb.rtn_value.var_name = s;
	fb.rtn_value.modifiable = true;
	return fb;
}

Feedback TokenStream::get_Func(string s){
	token t;
	t.type = '4';
	t.value = s;
	Feedback fb = symbol.getFunc(t);
	fb.rtn_value.value = t.value;
	return fb;
}

Feedback TokenStream::get_classFunc(string class_name, string func_name){
	token t;
	t.type = 19;
	t.value = class_name;
	t.var_name = func_name;
	Feedback fb = symbol.getFunc(t);
	fb.rtn_value.value = t.value;
	return fb;
}

Feedback TokenStream::modify_value(token t){
	cout << "modify_value " << t.var_name << "[" << t.array_index << "]" << " type:" << t.type << endl;
	Feedback fb = symbol.modifyValue(t);
	return fb;
}

/*
Feedback get_value(std::string s)//****call symbol table
{
	token var;
	var.type = '3';
	var.value = "abc";
	var.number = 1;
	var.modifiable = true;
	Feedback fb(var);
	return fb;
}


Feedback get_function(std::string s)//****call symbol table
{
	token func;
	func.type = '4';
	func.value = s;
	func.number = 2;
	Feedback fb(func);
	return fb;
}
*/


TokenStream::TokenStream(hashMap& symbol1): symbol(symbol1){
	full = false;
	buffer.type = '0';  // no token in buffer
}

TokenStream::~TokenStream(){

}

void TokenStream::putstream(std::string exp){
	sstream << exp;
}

Feedback TokenStream::get(){
	Feedback fb;
	std::string s;
	token var;
	Feedback fail_fb("Error: Bad token!");
	if (full){
		full = false;
		fb.rtn_value = buffer;
		return fb;
	}
	char ch, c_temp;
	sstream >> ch;
	while (ch == ' ' || ch == '\t' || ch == '\n') //skip space: " " or "\t" or "\n"
	{
		sstream >> ch;
	}
	switch (ch){
	case ';': //for "return"
	case '(':
	case ')':
	case '^':
	case ',':
	case '?':
	case ':':
	case '~':
	case '[':
	case ']':
		fb.rtn_value.type = ch;
		return fb;
	case '+':
		sstream >> ch;
		if (ch == '+'){
			fb.rtn_value.type = 9;
		}
		else if (ch == '='){
			fb.rtn_value.type = 13;
		}
		else {
			sstream.putback(ch);
			fb.rtn_value.type = '+';
		}
		return fb;
	case '-':
		sstream >> ch;
		if (ch == '-'){
			fb.rtn_value.type = 10;
		}
		else if (ch == '='){
			fb.rtn_value.type = 14;
		}
		else {
			sstream.putback(ch);
			fb.rtn_value.type = '-';
		}
		return fb;
	case '*':
		sstream >> ch;
		if (ch != '='){
			sstream.putback(ch);
			fb.rtn_value.type = '*';
		}
		else fb.rtn_value.type = 15;
		return fb;
	case '/':
		sstream >> ch;
		if (ch != '='){
			sstream.putback(ch);
			fb.rtn_value.type = '/';
		}
		else fb.rtn_value.type = 16;
		return fb;
	case '%':
		sstream >> ch;
		if (ch != '='){
			sstream.putback(ch);
			fb.rtn_value.type = '%';
		}
		else fb.rtn_value.type = 17;
		return fb;
	case '!':
		sstream >> ch;
		if (ch != '='){
			sstream.putback(ch);
			fb.rtn_value.type = '!';
		}
		else fb.rtn_value.type = 4;
		return fb;
	case '<':
		sstream >> ch;
		if (ch == '<')
			fb.rtn_value.type = 7;
		else if (ch == '=')
			fb.rtn_value.type = 1;
		else{
			sstream.putback(ch);
			fb.rtn_value.type = '<';
		}
		return fb;
	case '>':
		sstream >> ch;
		if (ch == '>')
			fb.rtn_value.type = 8;
		else if (ch == '=')
			fb.rtn_value.type = 2;
		else{
			sstream.putback(ch);
			fb.rtn_value.type = '>';
		}
		return fb;
	case '=':
		sstream >> ch;
		if (ch != '='){
			sstream.putback(ch);
			fb.rtn_value.type = '=';
		}
		else fb.rtn_value.type = 3;
		return fb;
	case '&':
		sstream >> ch;
		if (ch != '&'){
			sstream.putback(ch);
			fb.rtn_value.type = '&';
		}
		else fb.rtn_value.type = 5;
		return fb;
	case '|':
		sstream >> ch;
		if (ch != '|'){
			sstream.putback(ch);
			fb.rtn_value.type = '|';
		}
		else fb.rtn_value.type = 6;
		return fb;
	case '.':
		sstream >> c_temp;  //detect keyword
		if (!isdigit(c_temp)){
			sstream.putback(c_temp);
			while (sstream.get(c_temp) && isalpha(c_temp)){
				s += c_temp;
			}
			if (c_temp == '(') //class.function
			{
				fb.rtn_value.type = 19;
				fb.rtn_value.value = s;
				return fb;
			}
			//class.var
			sstream.putback(c_temp);
			fb.rtn_value.type = 18;
			fb.rtn_value.value = s;
			return fb;
		}
		sstream.putback(c_temp);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		sstream.putback(ch); //put a digit back into the stream
		double val; //read a floating-point or an integer (number of characters > 1)
		sstream >> val;
		int val_int = (int)val;
		if ((double)val_int == val){
			fb.rtn_value.type = '1';
		}
		else fb.rtn_value.type = '2';
		fb.rtn_value.number = val;
		return fb;
	}
	case '\"':
	case '\'':
		while (sstream.get(ch) && ch != '\'' && ch != '\"'){
			s += ch;
			if (ch == ';'){
				fail_fb.reason = "Error: a '\'' or '\"' expected!";
				return fail_fb;
			}
		}
		fb.rtn_value.type = '3';
		fb.rtn_value.value = s;
		return fb;
	default:
		if (isalpha(ch) || ch == '_' || ch == '&')  //detect var & function
		{
			s += ch;
			while (sstream.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '&'))
				s += ch;
			if (ch == '(') //function
			{
				fb = get_Func(s); 
				if (!fb.succeed){
					fail_fb.reason = "Error: Cannot find the function: " + s + ".";
					return fail_fb;
				}
				else{
					fb.rtn_value.type = '4';
					fb.rtn_value.value = s;
					return fb;
				}
			}
			else if (ch == '[') //array
			{
				fb.rtn_value.modifiable = true;
				fb.rtn_value.type = '7';
				fb.rtn_value.var_name = s;
				return fb;
			}
			else  //var
			{
				sstream.putback(ch);
				fb = get_value(s); 
				if (!fb.succeed){
					fail_fb.reason = "Error: Cannot find the var: " + s + ".";
					return fail_fb;
				}
				else{
					if (fb.rtn_value.type != '4') //simple var
					{
						fb.rtn_value.modifiable = true;
						fb.rtn_value.var_name = s;
					}
					else{
						fb.rtn_value.modifiable = false;
						fb.rtn_value.type = 'f';
					}

					return fb;
				}		
			}
		}
		return fail_fb;
	}
}

Feedback TokenStream::putback(token t){
	if (full){
		Feedback fail_fb("Error: putback() into a full buffer.");
		return fail_fb;
	}
	buffer = t;
	full = true;
	Feedback fb;
	return fb;
}