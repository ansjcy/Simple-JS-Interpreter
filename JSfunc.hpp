//
//  JSfunc.hpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#ifndef JSfunc_hpp
#define JSfunc_hpp

#include <stdio.h>
#include <string>
typedef struct funcT func;
struct funcT
{
    enum{FUNCTION = 0, INT = 1, STRING = 2, BOOL = 3, DOUBLE = 4, ARRAY = 5};
    char type;
    //function or var { int, string, bool, double, maybe array
    std::string value;     //if function, then it's address else its value in string format.
    std::string* parameters = NULL; //name of parameters
    unsigned int num; //number of parateters, at least 1 (self)
    funcT()
    {
        parameters = NULL;
        num = 0;
        value = "";
        type = -1;
    }
    
    funcT(char type, std::string value, std::string* parameters, unsigned int num):type(type),
    value(value), num(num)
    {
        if(num <= 0)
        {
            parameters = NULL;
        }
        else
        {
            this->parameters = new std::string[num];
            for(int i = 0; i < num; i++)
            {
                this->parameters[i] = parameters[i];
            }
        }
    }
    funcT(const funcT& copy)
    {
        type = copy.type;
        value = copy.value;
        num = copy.num;
        parameters = new std::string[num];
        for(int i = 0; i < num; i++)
            parameters[i] = copy.parameters[i];
       // printf("copy.\n");
    }
    ~funcT()
    {
        //if(value) delete[] value;
        if(parameters)
        {
            delete[] parameters;
            parameters = NULL;
        }
    }
    funcT operator=(const funcT& copy)
    {
        type = copy.type;
        value = copy.value;
        num = copy.num;
        parameters = new std::string[num];
        for(int i = 0; i < num; i++)
            parameters[i] = copy.parameters[i];
        return *this;
    }
    std::string& getValue()
    {
        return value;
    }
    char getType()
    {
        return type;
    }
};

#endif /* JSfunc_hpp */
