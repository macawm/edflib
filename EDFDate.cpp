/** 
 @file EDFDate.cpp 
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFDate.h"
#include "EDFUtil.h"

#include <iomanip>
#include <cstdlib>
#include <algorithm>

using std::string;

EDFDate::EDFDate() : EDFDate(1, 1, 85) {}

EDFDate::EDFDate(int day, int month, int year) {
    if (day < 1 || day > 31)
	this->d_day = 1;
    else
        this->d_day = day;

    if (month < 1 || month > 12)
	this->d_month = 1;
    else
        this->d_month = month;

    if (year < 0 || year > 99)
        this->d_year = 85;
    else
        this->d_year = year;
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
        this->d_day = 1;
    else
        this->d_day = day;
    
    if (month < 1 || month > 12)
        this->d_month = 1;
    else
        this->d_month = month;
    
    if (year < 0 || year > 99)
        this->d_year = 85;
    else
        this->d_year = year;
}


EDFDate::EDFDate(const EDFDate& orig) {
    d_day = orig.d_day;
    d_month = orig.d_month;
    d_year = orig.d_year;
}

/**
 Operator = overload to make a deep copy.
 @param rhs The object to copy.
*/
EDFDate& EDFDate::operator=(const EDFDate& rhs) {
    if (this != &rhs) {
        d_day = rhs.d_day;
        d_month = rhs.d_month;
        d_year = rhs.d_year;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFDate date) {
    s << std::setw(2) << std::setfill('0') << date.d_day << ".";
    s << std::setw(2) << std::setfill('0') << date.d_month << ".";
    s << date.fullYear();
    return s;
}

bool EDFDate::operator==(const EDFDate& rhs) {
    return this->d_day == rhs.d_day &&
           this->d_month == rhs.d_month &&
           this->d_year == rhs.d_year;
}

int EDFDate::day() const { return d_day; }
int EDFDate::month() const { return d_month; }
int EDFDate::year() const { return d_year; }
int EDFDate::fullYear() const { return (this->d_year > 84) ? 1900 + this->d_year : 2000 + this->d_year; }

void EDFDate::setDay(int day) {
    if (day < 1 || day > 31)
        this->d_day = 1;
    else
        this->d_day = day;
}

void EDFDate::setMonth(int month) {
    if (month < 1 || month > 12)
        this->d_month = 1;
    else
        this->d_month = month;
}

void EDFDate::setYear(int year) {
    if (year < 0 || year > 99)
        this->d_year = 85;
    else
        this->d_year = year;
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
