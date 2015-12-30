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
typedef struct interpreterT interpreter;
struct interpreterT
{
    hashMap* currentHsmp; //current hashmap
    vectorCode inputCode;       //store the codes
    
    void run();                 //entrance
    void doIf();
    void doWhile();
    void doFor();
    void doForeach();
    void doSwitch();
    void saveFunc();
    void doFunc();
    void doError();
};


#endif /* interpreter_hpp */
