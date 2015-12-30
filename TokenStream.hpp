#ifndef _TOKENSTREAM_HPP
#define _TOKENSTREAM_HPP
#include "utility.hpp"
#include "JShashMap.hpp"
#include <sstream>

class TokenStream{
private:
	std::stringstream sstream;
	bool full; // is there a token in the buffer
	token buffer; //here is where we keep a token put back using putback()
	hashMap &symbol;
public:
	TokenStream();
	TokenStream(hashMap& symbol);
	~TokenStream();
	void putstream(std::string exp);

	Feedback get();

	Feedback putback(token t);
};

#endif