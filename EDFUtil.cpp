/**
 @file EDFUtil.cpp
 @author Anthony Magee
 @date 11/6/2010
 */

#include "EDFUtil.h"
#include <string>
#include <sstream>
#include <iostream>
#include <limits>

using std::cerr;
using std::endl;
using std::string;

string convertSpaces(const string& str) {
    string s = str;
    size_t spaceLoc = s.find(" ");
    while (spaceLoc != string::npos) {
        s.replace(spaceLoc, 1, "_");
        spaceLoc = s.find(" ", spaceLoc + 1);
    }
    return s;
}

string convertUnderscores(const string& str) {
    string s = str;
    size_t underLoc = s.find("_");
    while (underLoc != string::npos) {
        s.replace(underLoc, 1, " ");
        underLoc = s.find("_", underLoc + 1);
    }
    return s;
}

//trim spaces from string
string trim(string str) {
    // operate on copy, not on reference
    size_t pos = str.find_last_not_of(' ');
    if(pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != string::npos)
            str.erase(0, pos);
    } else
        str.erase(str.begin(), str.end());
    
    return str;
}