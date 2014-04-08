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

/**
 Constructor to build new date object. Out of
 range values will default to 1.
 @param day Integer value between 1 and 31.
 @param month Integer value between 1 and 12.
 @param year Integer value between 0 and 99.
*/
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
        this->year = 1;
    else
        this->year = year;
    
    fullYear = 1900 + this->year;
    if (this->year <= 84)
        fullYear += 100;
    
    textMonth = monthIntToText(month);
}

/**
 Constructor to parse date from string of required
 format "dd.mm.yy". Any format errors will cause the date
 to be set to 01.01.85.
 @param date String of aforementioned format.
*/
EDFDate::EDFDate(string date) {
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
        this->year = 1;
    else
        this->year = year;
    
    fullYear = 1900 + this->year;
    if (this->year <= 84)
        fullYear += 100;
    
    textMonth = monthIntToText(month);
}

/**
 Copy constructor.
 @param orig The object to copy.
*/
EDFDate::EDFDate(const EDFDate& orig) {
    day = orig.day;
    month = orig.month;
    year = orig.year;
    fullYear = orig.fullYear;
    textMonth = orig.textMonth;
}

/**
 Destructor.
*/
EDFDate::~EDFDate() {
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
        fullYear = rhs.fullYear;
        textMonth = rhs.textMonth;
    }

    return *this;
}

/**
 Output operator to make a readable string of this object.
 @param s An output stream reference to place the data into.
 @param date The date to stringify.
 @return The output stream.
*/
std::ostream& operator<<(std::ostream& s, EDFDate date) {
    s << std::setw(2) << std::setfill('0') << date.day << ".";
    s << std::setw(2) << std::setfill('0') << date.month << ".";
    s << date.fullYear;
    return s;
}

/**
 Get the integer day of month value.
 @return Integer in [1, 31].
*/
int EDFDate::getDay() { return day; }

/**
 Get the integer month of year value.
 @return Integer in [1, 12].
*/
int EDFDate::getMonth() { return month; }

/**
 Get the integer year value.
 @return Integer in [0, 99].
*/
int EDFDate::getYear() { return year; }

/**
 Get the integer four digit year value.
 @return Integer in [1985, 2084].
*/
int EDFDate::getFullYear() { return fullYear; }

/**
 Set the date's day value. Out of range values
 will default to 1.
 @param day Integer value between 1 and 31.
*/
void EDFDate::setDay(int day) {
    if (day < 1 || day > 31)
        this->day = 1;
    else
        this->day = day;
}

/**
 Set the date's month value. Out of range values
 will default to 1.
 @param month Integer value between 1 and 12.
*/
void EDFDate::setMonth(int month) {
    if (month < 1 || month > 12)
        this->month = 1;
    else
        this->month = month;
}

/**
 Set the date's year value. Out of range values
 will default to 1.
 @param year Integer value between 0 and 99.
*/
void EDFDate::setYear(int year) {
    if (year < 0 || year > 99)
        this->year = 1;
    else
	this->year = year;

    fullYear = 1900 + this->year;
    if (this->year <= 84)
        fullYear += 100;
}

/**
 Translate month digit to three letter month.
 Invalid digits will return empty string.
 @param mon Month digit between 1 and 12.
 @return String containing three letter month
 representation or empty string.
*/
string EDFDate::monthIntToText(int mon) {
    string val;
    switch (mon) {
        case 1:
            val = "JAN"; break;
        case 2:
            val = "FEB"; break;
        case 3:
            val = "MAR"; break;
        case 4:
            val = "APR"; break;
        case 5:
            val = "MAY"; break;
        case 6:
            val = "JUN"; break;
        case 7:
            val = "JLY"; break;
        case 8:
            val = "AUG"; break;
        case 9:
            val = "SEP"; break;
        case 10:
            val = "OCT"; break;
        case 11:
            val = "NOV"; break;
        case 12:
            val = "DEC"; break;
        default:
	    val = "";
    }
    return val;
}
