#ifndef JShashMap_hpp
#define JShashMap_hpp

#include <stdio.h>
#include <map>
#include <string>
#include "JSfunc.hpp"
using namespace std;
//typedef struct fun{}func;
typedef struct hashMapStruct hashMap;
struct hashMapStruct
{
    map<string, func> hashFunc;
    hashMap* last;
    unsigned int lineOffset;
    unsigned int currentLine;
    
    hashMapStruct(){
        last = NULL;
        lineOffset = 0;
        currentLine = 0;
    }
    func getFunc(string line)
    {
        return hashFunc[line];
    }
    void pushFunc(string name, func fun)
    {
        if(hashFunc.count(name) != 0)
        {
            printf("redefination of function.");
            return;
        }
        hashFunc[name] = fun;
    }
    void insert(hashMap* lastHash)
    {
        last = lastHash;
    }
};

#endif /* JShashMap_hpp */
