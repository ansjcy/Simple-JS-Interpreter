#ifndef _UTILITY_HPP
#define _UTILITY_HPP
#include <string>

struct token{
	char type;  //'1'£ºint '2':double '3':string
	std::string value;
};
struct Feedback{
	bool succeed;
	struct token rtn_value;
	std::string reason;
	Feedback(struct token return_value) : succeed(true), rtn_value(return_value) {}
	Feedback(std::string fail_reason) : succeed(false), reason(fail_reason) {}
};

#endif