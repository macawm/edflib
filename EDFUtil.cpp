/* 
 * File:   EDFUtil.cpp
 *
 * Created on November 6, 2010, 11:55 PM
 */

#include "EDFParser.h"
#include <sstream>
#include <iostream>
#include <limits>

using std::cerr;
using std::endl;

double a2f(std::string str) {
    double val;
    if (str.compare("") == 0)
        return std::numeric_limits<double>::infinity();

    std::istringstream iss(str);
    iss >> val;

    if (iss.fail()) {
        cerr << "Could not convert string \"" << str << "\" to double" << endl;
        return std::numeric_limits<double>::infinity();
    }

    return val;
}

double a2i(std::string str) {
    int val;
    if (str.compare("") == 0)
        return std::numeric_limits<int>::infinity();

    std::istringstream iss(str);
    iss >> val;

    if (iss.fail()) {
        cerr << "Could not convert string \"" << str << "\" to integer" << endl;
        return std::numeric_limits<int>::infinity();
    }

    return val;
}

string convertSpaces(const std::string& str) {
    string s = str;
    size_t spaceLoc = s.find(" ");
    while (spaceLoc != string::npos) {
        s.replace(spaceLoc, 1, "_");
        spaceLoc = s.find(" ", spaceLoc + 1);
    }
    return s;
}

string convertUnderscores(const std::string& str) {
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
    size_t pos = str.find_last_not_of(' ');
    if(pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != string::npos) str.erase(0, pos);
    } else
        str.erase(str.begin(), str.end());
    
    return str;
}