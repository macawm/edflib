/** 
 @file EDFDate.cpp
 @brief A model file for an EDF date.
 EDF specified dates are only valid between
 1/1/1985 through 12/31/2084. They are
 formatted as such dd.mm.yy and must not
 exclude leading zeros.
 
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFDate.h"
#include <iomanip>
#include <cstdlib>
#include <algorithm>

EDFDate::EDFDate() : EDFDate(1, 1, 85) {}

EDFDate::EDFDate(int day, int month, int year) {
    if (day < 1 || day > 31)
	this->day = 1;
    else
        this->day = day;

    if (month < 1 || month > 12)
	this->month = 1;
    else
        this->month = month;

    if (year < 0 || year > 99)
        this->year = 85;
    else
        this->year = year;
}

EDFDate::EDFDate(const string &date) {
    int day, month, year;
    if (date[2] != '.' || date[5] != '.' ||
        date[0] < '0' || date[0] > '9' ||
        date[1] < '0' || date[1] > '9' ||
        date[3] < '0' || date[3] > '9' ||
        date[4] < '0' || date[4] > '9' ||
        date[6] < '0' || date[6] > '9' ||
        date[7] < '0' || date[7] > '9') {
        // formatting error set to first valid date
        day = month = 1;
        year = 85;
    } else {
        day = atoi(date.substr(0, 2).c_str());
        month = atoi(date.substr(3, 2).c_str());
        year = atoi(date.substr(6, 2).c_str());
    }
    
    if (day < 1 || day > 31)
        this->day = 1;
    else
        this->day = day;
    
    if (month < 1 || month > 12)
        this->month = 1;
    else
        this->month = month;
    
    if (year < 0 || year > 99)
        this->year = 85;
    else
        this->year = year;
}


EDFDate::EDFDate(const EDFDate& orig) {
    day = orig.day;
    month = orig.month;
    year = orig.year;
}

/**
 Operator = overload to make a deep copy.
 @param rhs The object to copy.
*/
EDFDate& EDFDate::operator=(const EDFDate& rhs) {
    if (this != &rhs) {
        day = rhs.day;
        month = rhs.month;
        year = rhs.year;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFDate date) {
    s << std::setw(2) << std::setfill('0') << date.day << ".";
    s << std::setw(2) << std::setfill('0') << date.month << ".";
    s << date.getFullYear();
    return s;
}

int EDFDate::getDay() { return day; }
int EDFDate::getMonth() { return month; }
int EDFDate::getYear() { return year; }
int EDFDate::getFullYear() { return (this->year > 84) ? 1900 + this->year : 2000 + this->year; }

void EDFDate::setDay(int day) {
    if (day < 1 || day > 31)
        this->day = 1;
    else
        this->day = day;
}

void EDFDate::setMonth(int month) {
    if (month < 1 || month > 12)
        this->month = 1;
    else
        this->month = month;
}

void EDFDate::setYear(int year) {
    if (year < 0 || year > 99)
        this->year = 85;
    else
        this->year = year;
}


// from testing this method is about 40% faster than a switch
const string MONTHS_STR = "JANFEBMARAPRMAYJUNJLYAUGSEPOCTNOVDEC";
string EDFDate::monthIntToText(int mon) {
    if (mon < 1 || mon > 12)
        return "";
    
    else
        return MONTHS_STR.substr((mon - 1) * 3, 3);
}

int EDFDate::monthTextToInt(string& s) {
    int m = -1;
    
    if (s.length() == 3) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        size_t pos = MONTHS_STR.find(s);
        if (pos != string::npos)
            m = (int)pos / 3 + 1; // relies on integer division
    }
    
    return m;
}
