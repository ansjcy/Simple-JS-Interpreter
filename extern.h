//
//  extern.h
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#ifndef extern_h
#define extern_h
#include "interpreter.hpp"
#include "doIf.hpp"
#include "funcHand.hpp"
#include "readHand.hpp"
extern interpreter handler;
extern funcHandler funcHand;       //save func or var...
extern readHandler readHand;       //read the next line code
extern ifHandler ifHand;
extern bool lastIf;
extern bool isbreak;
extern bool isreturn;
extern func retVal;
extern whileHandler whileHand;
extern forHandler forHand;
#endif /* extern_h */
