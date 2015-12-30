//
//  main.cpp
//  intepreter
//
//  Created by Ah on 12/30/15.
//  Copyright © 2015 Ah. All rights reserved.
//

#include <iostream>
using namespace std;
#include "JSVector.hpp"
#include "JShashMap.hpp"
#include "JSfunc.hpp"
int main(void)
{
    hashMap test;
    string  s[] = {"str1","str2"};
    string s1 = "123";
    string s2 = "333";
    func temp1((char)(func::INT), s1, s, 2);
    func temp2((char)(func::INT), s2, s, 2);
    cout << "----.\n";
    test.pushFunc("123", temp1);
    test.pushFunc("234", temp2);
    cout << "+++++.\n";
    func t = test.getFunc("123");
    cout <<"******.\n";
     func sb = test.getFunc("234");
    cout << t.getType() << " " << t.getValue() << endl;
    cout << sb.getType() << " " << sb.getValue() << endl;
    cout << test.getFunc("234").getType() << " " << test.getFunc("234").getValue() << endl;
    
}