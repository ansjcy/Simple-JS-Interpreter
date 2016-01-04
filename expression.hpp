#ifndef _EXPRESSION_HPP
#define _EXPRESSION_HPP
#include <string>
#include "TokenStream.hpp"

class Expression{
private:
	TokenStream ts;	
	Feedback condition(); //'? !'
	Feedback logi_or(); //'||'
	Feedback logi_and(); // '&&'
	Feedback bit_or(); // '|'
	Feedback bit_xor(); // '^'
	Feedback bit_and(); // '&'
	Feedback compare_equ(); // '==' '!='
	Feedback compare_nonequ(); // '>' '<' '>=' '<='
	Feedback shift(); // '<<' '>>'
	Feedback add_minus(); // '+' '-'
	Feedback term(); // '*' '/' '%'
	Feedback primary(); // () number string function var
	Feedback get_token(); 

private:
	int isNumber(token token1, token token2);
	bool number_string(token token1, token token2);
	std::string get_type(token t);
	Feedback add(token left, token right);
	Feedback minus(token left, token right);
	Feedback multi(token left, token right);
	Feedback divide(token left, token right);
	Feedback mod(token left, token right);
	Feedback construct_func(token func);

public:
	Expression(hashMap &symbol, std::string exp);
	~Expression();	
	Feedback expression();
};
#endif