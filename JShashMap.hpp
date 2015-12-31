#ifndef JShashMap_hpp
#define JShashMap_hpp

#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include "JSfunc.hpp"
#include "utility.hpp"
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
        if(isExist(line) == true)
        {
            if(hashFunc.count(line) > 0)
                return hashFunc[line];
            else
                return last->getFunc(line);
        }
        cout << "empty.\n";
        return func(); //empty
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
    bool isExist(string line)
    {
        return hashFunc.count(line) > 0 || (last && last->isExist(line));
    }
    void insert(hashMap* lastHash)
    {
        last = lastHash;
    }
    //void assign()
    Feedback getValue(token t)
    {
       if(isExist(t.var_name))
       {
           func fc = getFunc(t.var_name);
           token tk;
           tk.type = fc.type;
          // tk.array_index = 0;
           tk.modifiable = true;
           if(fc.value != "" &&(fc.type == fc.INT || fc.type == fc.DOUBLE))
               tk.number = stof(fc.getValue());
           tk.para = NULL;
           tk.value = fc.getValue();
           tk.var_name = t.var_name;
           return Feedback(token(tk));
       }
       else
       {
           return Feedback("No such var");
       }
    }
    bool modify(token t)
    {
        if(hashFunc.count(t.var_name) > 0)
        {
            hashFunc[t.var_name].type = t.type;
            if(t.type == func::INT || t.type == func::DOUBLE)
            {
                cout << "t.number: " << t.number << endl;
                hashFunc[t.var_name].value = to_string(t.number);
            }
            else
                hashFunc[t.var_name].value = t.value;
            return true;
        }
        else if(last)
        {
            return last->modify(t);
        }
        return false;
    }
    Feedback getFunc(token t)
    {
        return Feedback();
    }
    Feedback modifyValue(token t)
    {
        if(modify(t))
            return Feedback();
        else
            return Feedback("No such variables.\n");
    }
    
};

#endif /* JShashMap_hpp */
