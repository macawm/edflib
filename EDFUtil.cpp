/* 
 * File:   EDFUtil.cpp
 *
 * Created on November 6, 2010, 11:55 PM
 */

#include "EDFUtil.h"
#include <string>
#include <sstream>
#include <iostream>
#include <limits>

using std::cerr;
using std::endl;
using std::string;

double a2f(string str) {
    double val;
    if (str.compare("") == 0)
        return 0;

    std::istringstream iss(str);
    iss >> val;

    if (iss.fail()) {
        DI(cerr << "* RT  Could not convert string \"" << str << "\" to double" << endl)
        return std::numeric_limits<double>::infinity();
    }

    return val;
}

int a2i(string str) {
    int val;
    if (str.compare("") == 0)
        return 0;

    std::istringstream iss(str);
    iss >> val;

    if (iss.fail()) {
        DI(cerr << "* RT  Could not convert string \"" << str << "\" to integer" << endl)
        return std::numeric_limits<int>::infinity();
    }

    return val;
}

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