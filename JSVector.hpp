//
//  JSVector.hpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#ifndef JSVector_hpp
#define JSVector_hpp

#include <stdio.h>
#include <string>


typedef struct codeSegment
{
    const unsigned int DEFAULTSIZE = 4;
    unsigned int next;
    unsigned int totalSize;
    std::string* code;
    codeSegment(){next = -1; totalSize = DEFAULTSIZE; code = new std::string[DEFAULTSIZE];};
    void push_back(std::string line)
    {
        next++;//next position
        if(next == totalSize)
        {
            //if full enlarge it to twice the original size
            std::string* temp;
            temp = new std::string[totalSize*2];
            for(int i = 0; i < totalSize; i++)
                temp[i] = code[i];
            delete[] code;
            temp[next] = line;
            code = temp;
            totalSize *= 2;
        }
        else
        {
            code[next] = line;
        }
    }
    //void enlarge(int size);
    std::string getCode(int idx)
    {
        if(idx < 0 || idx > next)
        {
            printf("Invalid idx %d.\n", idx);
            return std::string("");
        }
        return code[idx];
    }
}vectorCode;



#endif /* JSVector_hpp */
