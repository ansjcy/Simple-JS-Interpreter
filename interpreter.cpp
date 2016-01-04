//
//  interpreter.cpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#include "interpreter.hpp"
#include "CalExpression.hpp"
#include "doIf.hpp"
#include "extern.h"
//#include "CalExpression.hpp"
#include <iostream>
using namespace std;
//hashMap* currentHsmp;        //current hashmap
extern whileHandler whileHand;
extern forHandler forHand;




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
        line = readHand.getNextLineCode(hsmp);
        execute(line, hsmp);
    }
}



string  interpreter::getExp(stringstream& in)
{
    string exp = "";
    char ch;
    bool spaceIsIgnore = true;
    while(in.read(&ch, 1))
    {
        if(ch == '"' || ch == '\'')
        {
            //exp += '\\';
            spaceIsIgnore = !spaceIsIgnore;
        }
        if(ch == ' ' && spaceIsIgnore)
            continue;
        exp += ch;
    }
    return exp;
}


void interpreter::execute(string& code, hashMap& hsmp)
{
    stringstream in(code.c_str());
    string ins;
    in >> ins;  //get key word or not key word
    if(ins == "var" || ins == "function")
    {
        funcHand.saveFunc(code, hsmp);
    }
    else if(ins == "if")
    {
        string exp = "";
        char ch;
        while(in >> ch)
            exp += ch;
        ifHand.doIf(hsmp, exp);
       // cout << "if" << endl;
    }
    else if(ins == "while")
    {
        string exp = "";
        char ch;
        while(in >> ch)
            exp += ch;
        whileHand.doWhile(hsmp, exp);
      //  cout << "while" << endl;
    }
    else if(ins == "switch")
    {
        cout << "switch" << endl;
    }
//    else if(ins == "foreach")
//    {
//        cout << "foreach" << endl;
//    }
    else if(ins == "for")
    {
        string exp = "";
        string s;
        while(in >> s)
            exp += s + " ";
        forHand.doFor(hsmp, exp);
        cout << "------for-------" << endl;
    }
//    else if(ins == "new")
//    {
//        cout << "new" << endl;
//    }
    else if(ins == "return")
    {
        isreturn = true;
        string exp = getExp(in);
       // cout << exp << endl;
        Feedback fb = cal_expression(exp, hsmp);
        if(fb.succeed == true)
        {
            string val = convert2str(fb);//fb.rtn_value.value;
            retVal.value = val;
           // cout << fb.rtn_value.type<< endl;
           // cout << "Val: " << val << endl;
            retVal.type = fb.rtn_value.type;
            retVal.num = 0;
        }
        else
        {
            cout << fb.reason << endl;
        }
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
    else //if()
    {
        if(hsmp.isExist(ins) && hsmp.getFunc(ins).type == func::FUNCTION)
        {
            doFunc(code, hsmp);
        }
        else if(hsmp.isExist(ins) && hsmp.getFunc(ins).type != func::FUNCTION)
        {
            Feedback fb = cal_expression(code, hsmp);
            if(fb.succeed == false)
            {
                cout << fb.reason << endl;
            }
            else
            {
                cout << "Val: " <<  convert2str(fb) << endl;
            }
            
        }
        else
        {
            Feedback fb = cal_expression(code, hsmp);
            if(fb.succeed == false)
            {
                cout << fb.reason << endl;
            }
            else
            {
                cout << "Val: " <<  convert2str(fb) << endl;
            }
        }
    }
}

void interpreter::doFunc(string& code, hashMap& lasthsmp)
{
    hashMap hsmp;
    hsmp.last = &lasthsmp;
    stringstream ss(code.c_str());
    string funcName;
    ss >> funcName;
    hsmp.currentLine = atoi(lasthsmp.getFunc(funcName).getValue().c_str());
    int count = 0;
    while(true)
    {
        string line;
        line = readHand.getNextLineCode(hsmp);
        cout << "Line: " << line << endl;
        count += funcHand.countNum(line, '{');
        count -= funcHand.countNum(line, '}');
        handler.execute(line, hsmp);
        if(isreturn == true || count == 0)
        {
            cout << "lallalala.\n";
            return;
        }
    }
    
}


string interpreter::convert2str(Feedback& fb)
{
    // '0':null '1'£∫int '2':double '3':string '4':function '5':boolean  '6':array  '7':undefined
    switch (fb.rtn_value.type)
    {
        case  '0': return "NULL";
        case  '1':case '2': return to_string(fb.rtn_value.number);
        case  '3':return fb.rtn_value.value;
       // case  '5':return fb.rtn_value.number == 0? "false":"true";
       // case  '1': return fb.rtn_value.number
    }
    return "UNDEFINED";
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
       // cout << "else if( " << exp << ")" << endl;
    }
}


void ifHandler::doIf(hashMap& lasthsmp, string exp)
{
    hashMap hsmp;
    hsmp.currentLine = lasthsmp.currentLine; //next line
    hsmp.last = &lasthsmp; //link
    Feedback fb = cal_expression(exp, hsmp);
    
    if(handler.convert2str(fb) != STRFALSE) //????
    {
       // cout << "True.\n";
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


void forHandler::doFor(hashMap& lasthsmp, string exp)
{
    /*for ( int i = 0, int j = 0; i < 2, j < 3; j++, i++ )
     {
     a++;
     b++;
     c++;
     }
     */
    hashMap hsmp;
    hsmp.last = &lasthsmp;
    hsmp.currentLine = lasthsmp.currentLine;
    string initial = "";
    string tmpCondition = "";
    string tmpOperation = "";
    string* operation;
    string* condition;
    int countSemi = 0;
    int countComma = 0;
    for(int i = 1; i < exp.length(); i++)
    {
        if(exp[i] == ';')
        {
            countSemi++;
            i++;
        }
        if(countSemi == 0)
            initial += exp[i];
        else if(countSemi == 1)
            tmpCondition += exp[i];
        else if(countSemi == 2)
            tmpOperation += exp[i];
    }
    int totalCondition = funcHandler::countNum(tmpCondition, ',') + 1;
    condition = new string[totalCondition];
    for(int i = 0; i < tmpCondition.length(); i++)
    {
        if(tmpCondition[i] == ';')
            break;
        if(tmpCondition[i] == ',')
        {
            i++;
            countComma++;
        }
        condition[countComma]+= tmpCondition[i];
    }
    
    countComma = 0;
    int totalOperation = funcHandler::countNum(tmpOperation, ',') + 1;
    operation = new string[totalOperation];
    for(int i = 0; i < tmpOperation.length(); i++)
    {
        if(tmpOperation[i] == ')')
            break;
        if(tmpOperation[i] == ',')
        {
            i++;
            countComma++;
        }
        operation[countComma]+= tmpOperation[i];
    }
    
    handler.execute(initial, hsmp);
    
    //    cout<<operation[0]<<endl;
    //    cout<<operation[1]<<endl;
    //    cout<<condition[0]<<endl;
    //    cout<<condition[1]<<endl;
    bool checkCond = true;
    for(int i = 0; i < totalCondition; i++)
    {
        //exp satisfied all
        Feedback fb = cal_expression(condition[i], hsmp);
        if(fb.succeed == true && handler.convert2str(fb) != STRFALSE)
        {
            checkCond = false;
            break;
        }
        else if(fb.succeed == false)
        {
            checkCond = false;
            cout << fb.reason << endl;
            break;
        }
        
    }
    if(checkCond)
    {
        int count = 0;
        string line;
        while(true)
        {
            line = readHand.getNextLineCode(hsmp);
            count += funcHandler::countNum(line, '{');
            count -= funcHandler::countNum(line, '}');
            handler.execute(line, hsmp);
            if(isreturn) return;
            if(isbreak)
            {
                while(true)
                {
                    string line = readHand.getNextLineCode(hsmp);
                    count += funcHandler::countNum(line, '{');
                    count -= funcHandler::countNum(line, '}');
                    if(count == 0) break;
                }
                break;
            }
            
            if(count == 0)
            {
                for(int i = 0; i < totalCondition; i++)
                {
                    //exp satisfied all
                    Feedback fb = cal_expression(condition[i], hsmp);
                    if(fb.succeed == true && handler.convert2str(fb) != STRFALSE)
                    {
                        checkCond = false;
                        break;
                    }
                    
                }
                if(!checkCond)
                    break;
                for(int i = 0; i < totalOperation; i++)
                {
                    handler.execute(operation[i], hsmp);
                }
                hsmp.currentLine = lasthsmp.currentLine;
            }
        }
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
    }
    lasthsmp.currentLine = hsmp.currentLine;
}

void whileHandler::doWhile(hashMap& lasthsmp, string exp)
{
    hashMap hsmp;
    hsmp.last = &lasthsmp;
    hsmp.currentLine = lasthsmp.currentLine;
    Feedback fb = cal_expression(exp, hsmp);
    
    if(fb.succeed && handler.convert2str(fb) != STRFALSE)
    {
        int count = 0;
        string line;
        while(true)
        {
            line = readHand.getNextLineCode(hsmp);
            count += funcHandler::countNum(line, '{');
            count -= funcHandler::countNum(line, '}');
            if(line!="{"&&line!="}")
                handler.execute(line, hsmp);
            if(isreturn) return;
            if(isbreak)
            {
                while(true)
                {
                    string line = readHand.getNextLineCode(hsmp);
                    count += funcHandler::countNum(line, '{');
                    count -= funcHandler::countNum(line, '}');
                    if(count == 0) break;
                }
                break;
            }
            fb = cal_expression(exp, hsmp);
            if(count == 0 && handler.convert2str(fb) != STRFALSE)
            {
                hsmp.currentLine = lasthsmp.currentLine;
            }
        }
    }
    else if(!fb.succeed)
    {
        cout<<fb.reason<<endl;
        //call error handler..
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
    }
    lasthsmp.currentLine = hsmp.currentLine;
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
void funcHandler::saveFunc(string line, hashMap& hsmp )
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
        while(in.read(&ch,1))
        {
            if(isValidNameChar(ch) )
            {
                funcName += ch;
            }
            else if(funcName != "")
            {
                break;
            }
            
        }
        expression = funcName;
        while(in.read(&ch, 1))
        {
            
            if(ch == '"' || ch == '\'')
            {
                // expression += '\\';
                spaceIsIgnore = !spaceIsIgnore;
            }
            if(ch == ' ' && spaceIsIgnore)
                continue;
            expression += ch;
        }
        func var;
        var.type = var.INT;
        
        hsmp.pushFunc(funcName, var);
        cout << "expression: " << expression << endl;
        if(expression != funcName)
        {   Feedback fb = cal_expression(expression, hsmp);
            if(fb.succeed == false) { cout << fb.reason;}
        }
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
                hsmp.pushFunc(funcName, func(func::FUNCTION, to_string(hsmp.currentLine), exp, count + 1));
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


