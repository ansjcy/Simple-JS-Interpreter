//
//  interpreter.cpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#include "interpreter.hpp"
//#include "CalExpression.hpp"
#include <iostream>
using namespace std;
//hashMap* currentHsmp;        //current hashmap
interpreter handler;
funcHandler funcHand;       //save func or var...
readHandler readHand;       //read the next line code
ifHandler ifHand;
bool lastIf = false;
bool isbreak = false;
bool isreturn = false;
func retVal;


string readHandler::getNextLineCode(hashMap& hsmp)
{
    string line;
    if(hsmp.currentLine == countLen)
    {
        char buffer[MAXSIZE];
        cin.getline(buffer, MAXSIZE);
        countLen += 1;
        line = buffer;
        inputCode.push_back(line);
        hsmp.currentLine += 1;
        hsmp.lineOffset = 0;
    }
    else
    {
        line = inputCode.getCode(hsmp.currentLine);
        hsmp.currentLine += 1;
        hsmp.lineOffset = 0;
    }
    return line;
}

void interpreter::run()                 //entrance
{
    //currentHsmp = new hashMap;
    //make prepare
    /*
     1. code line , line offset
     2. global variable.
     3.
     */
    hsmp.currentLine = 0;
    hsmp.lineOffset = 0;
    string line;
    while(true)
    {
        line = readHand.getNextLineCode(&hsmp);
        execute(line, hsmp);
    }
}
bool funcHandler::isValidName(string name)
{
    for(int i = 0; i < name.length(); i++)
        if(!isValidNameChar(name.at(i), i)) return false;
    return true;
}
 unsigned int funcHandler::countNum(string& src, char dest)
{
    unsigned int count = 0;
    for(int i = 0; i < src.length(); i++)
    {
        if(src.at(i) == dest)
            count += 1;
    }
    return count;
}

bool funcHandler::isValidNameChar(char ch, unsigned int pos)
{
    if(((ch>='a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') ||
        (ch >= '0' && ch <= '9' && pos != 0))
        return true;
    return false;
}

void funcHandler::saveFunc(string line, hashMap* hsmp )
{
    stringstream in(line.c_str());
    string ins;
    bool spaceIsIgnore = true;
    char ch;
    string funcName = "";
    string expression = "";
    in >> ins;
    //
    if(ins == "var")
    {
        while(in>>ch)
        {
            if(isValidNameChar(ch))
            {
                funcName += ch;
            }
            else
            {
                break;
            }
                
        }
        while(in.read(&ch, 1))
        {
        
            if(ch == '"' || ch == '\'')
            {
                expression += '\\';
                spaceIsIgnore = !spaceIsIgnore;
            }
            if(ch == ' ' && spaceIsIgnore)
                continue;
            expression += ch;
            //cal_expression(expression);
        }
        hsmp->pushFunc(funcName, func());
        cout << ins << " -> " <<  funcName << " -> " << expression << endl;
    }
    else if(ins == "function")
    {
        int count = 0;
        string exp[30]; //= "";
        string funcName = "";
        while(in >> ch)
        {
            if(ch == '(') break;
            funcName += ch;
        }
        while(in >> ch)
        {
            if(ch == ')')
            {
                //if(!isValidName(funcName))
                hsmp->pushFunc(funcName, func(func::FUNCTION, to_string(hsmp->currentLine), exp, count + 1));
                break;
            }
            if(ch == '(')
                continue;
            if(ch == ',' && exp[count] != "" )
            {
                //if(!isValidName(exp[count]))
                count += 1;
                exp[count] = "";
            }
            else if(ch != ',')
            {
                exp[count] += ch;
            }
        }
        count = 0;string line;
        while(true)
        {
            line = readHand.getNextLineCode(hsmp);
            count += countNum(line, '{');
            count -= countNum(line, '}');
            if(count == 0)
                break;
        }
        
    }
}

void interpreter::execute(string& code, hashMap& hsmp)
{
    stringstream in(code.c_str());
    string ins;
    in >> ins;  //get key word or not key word
    if(ins == "var" || ins == "function")
    {
        funcHand.saveFunc(code, &hsmp);
    }
    else if(ins == "if")
    {
        string exp = "";
        char ch;
        while(in >> ch)
            exp += ch;
        ifHand.doIf(hsmp, exp);
        cout << "if" << endl;
    }
    else if(ins == "while")
    {
        cout << "while" << endl;
    }
    else if(ins == "switch")
    {
        cout << "switch" << endl;
    }
    else if(ins == "foreach")
    {
        cout << "foreach" << endl;
    }
    else if(ins == "for")
    {
        cout << "for" << endl;
    }
//    else if(ins == "new")
//    {
//        cout << "new" << endl;
//    }
    else if(ins == "return")
    {
        isreturn = true;
        
        cout << "return" << endl;
    }
    else if(ins == "else")
    {
        this->thisDoIf(hsmp, in);
    }
    else if(ins == "break")
    {
        isbreak = true;
    }
    else
    {
        //cout << "else " << ins << endl;
    }
}

void interpreter::thisDoIf(hashMap& hsmp, stringstream& in)
{
    if(lastIf == true)
    {
        int count = 0;
        while(true)
        {
            string line = readHand.getNextLineCode(hsmp);
            count += funcHandler::countNum(line, '}');
            count -= funcHandler::countNum(line, '{');
            if(count == 0) break;
        }
    }
    else
    {
        string exp = "";
        int count = 0;
        char ch;
        while(in >> ch)
        {
            //check grammar... else xxx??
            if(ch == '(')
            {
                count += 1;
                bool flag = false;
                while(true)
                {
                    in.read(&ch, 1);
                    if(ch == ')') count --;
                    if(count == 0)
                    {
                        flag = true;
                        break;
                    }
                    exp += ch;
                }
                if(flag) break;
            }
        }
        if(exp.size() == 0) exp = "(1==1)";
        ifHand.doIf(hsmp, exp);
        cout << "else if( " << exp << ")" << endl;
    }
}






void ifHandler::doIf(hashMap& lasthsmp, string exp)
{
    hashMap hsmp;
    hsmp.currentLine = lasthsmp.currentLine;
    
    if(/*exp satisfied*/ /* DISABLES CODE */ (false))
    {
        int count = 0;
        string line;
        while(true)
        {
            line = readHand.getNextLineCode(hsmp);
            count += funcHandler::countNum(line, '{');
            count -= funcHandler::countNum(line, '}');
            handler.execute(line, hsmp);
            if(isbreak == true || isreturn) return;
            if(count == 0) break;
        }
        lastIf = true;
    }
    else
    {
        int count = 0;
        while(true)
        {
            string line = readHand.getNextLineCode(hsmp);
            count += funcHandler::countNum(line, '{');
            count -= funcHandler::countNum(line, '}');
            if(count == 0) break;
        }
        lastIf = false;
    }
    lasthsmp.currentLine = hsmp.currentLine;
}





