//
//  interpreter.hpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#ifndef interpreter_hpp
#define interpreter_hpp

#include <stdio.h>
#include "JSfunc.hpp"
#include "JSVector.hpp"
#include "JShashMap.hpp"
#include <iostream>
#include <iosfwd>
#include <sstream>
#define MAXSIZE 128

class interpreter
{
    hashMap hsmp;
public:
    void run() ;                //entrance
    void handleFeedback();
    void execute(string& code, hashMap& hsmp);
    void doIf();                //in if.
    void doWhile();
    void doFor();
    void doForeach();
    void doSwitch();
    void saveFunc();
    void doFunc();
    void doError();
};
class ifHandler
{
public:
    void doIf(hashMap& lasthsmp, string exp);
};


class readHandler
{
public:
    vectorCode inputCode;       //store the codes
    unsigned int countLen;
    readHandler(){countLen = 0;}
    string getNextLineCode(hashMap* hsmp);
};
class funcHandler
{
public:
    void saveFunc(string line, hashMap* hsmp);
    bool isValidName(string name);
    static unsigned int countNum(string& src, char dest);
    bool isValidNameChar(char ch, unsigned int pos = -1);
};




#endif /* interpreter_hpp */
