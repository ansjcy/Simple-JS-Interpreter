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
#include "utility.hpp"
#define MAXSIZE 128
#define STRFALSE ("0.000000")
class interpreter
{
    hashMap hsmp;
public:
    void run() ;                //entrance
    void handleFeedback();
    void execute(string& code, hashMap& hsmp);
    void thisDoIf(hashMap& hsmp, stringstream& in);                //in if.
    void thisDoWhile();
    void thisDoFor();
    void thisDoForeach();
    void thisDoSwitch();
    void saveFunc();
    void doFunc(string& code, hashMap& lasthsmp);
    void doError();
    string getExp(stringstream& in);
    string convert2str(Feedback& fb);
};

class ifHandler
{
public:
    void doIf(hashMap& lasthsmp, string exp);
};

class whileHandler
{
public:
    void doWhile(hashMap& lasthsmp, string exp);
};

class forHandler
{
public:
    void doFor(hashMap& lasthsmp, string exp);
};

class funcHandler
{
public:
    void saveFunc(string line, hashMap& hsmp);
    bool isValidName(string name);
    static unsigned int countNum(string& src, char dest);
    bool isValidNameChar(char ch, unsigned int pos = -1);
};

class readHandler
{
public:
    vectorCode inputCode;       //store the codes
    unsigned int countLen;
    readHandler(){countLen = 0;}
    string getNextLineCode(hashMap& hsmp);
};



#endif /* interpreter_hpp */
