//
//  parser.hpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include <stdio.h>
#include <string>
using namespace std;

class parser
{
public:
    char type;
    int numPart;
    string* part;
    parser(string& str);
    ~parser();
    void normalize(string& str);
};


parser::parser(string& str)
{
    normalize(str);
    for(int i = 0; i < str.length(); i++)
    {
        
    }
}

void parser::normalize(string& input)
{
    for(int i = 0; i < input.length(); i++)
    {
        char a = input.at(i);
        if(input.at(i) == '(')
        {
            input.replace(i, 1, " ( ");
            i+=2;
        }
        else if(input.at(i) == ')')
        {
            input.replace(i, 1, " ) ");
            i+=2;
        }
        else if(input.at(i) == ',')
        {
            input.replace(i, 1, " , ");
            i+=2;
        }
        else if(input.at(i) == ';')
        {
            input.replace(i, 1, " ; ");
            i+=2;
        }
//        else if(input.at(i) == '\'')
//        {
//            input.replace(i, 1, " ' ");
//            i+=2;
//        }
//        else if(input.at(i) == '=')
//        {
//            input.replace(i, 1, " = ");
//            i+=2;
//        }
        else if(input.at(i) == '[')
        {
            input.replace(i, 1, " [ ");
            i+=2;
        }
        else if(input.at(i) == ']')
        {
            input.replace(i, 1, " ] ");
            i+=2;
        }
        else if(input.at(i) == '{')
        {
            input.replace(i, 1, " { ");
            i+=2;
        }
        else if(input.at(i) == '}')
        {
            input.replace(i, 1, " } ");
            i+=2;
        }
        else if(input.at(i) == '"')
        {
            input.replace(i, 1, " \" ");
            i+=2;
        }
        
    }
    while(input.find("  ") != string::npos)
    {
        input.replace(input.find("  "), 2, " ");
    }
    if(input.back() == ' ')
    {
        input = input.substr(0, input.length()-1);
    }
    //return input;

}

#endif /* parser_hpp */
