/**
 @file EDFTime.cpp
 @brief A model file for an EDF time.
 EDF specified times are formatted as such hh.mm.ss
 and must not exclude leading zeros and must be positive
 in each dimension.

 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFTime.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

using std::cerr;
using std::endl;

/**
 Constructor to build new time object. Out of
 range values will be adjusted as appropriate.
 @param hour Integer value between 0 and 59.
 @param min Integer value between 0 and 59.
 @param sec Integer value greater than or equal to 0.
*/
EDFTime::EDFTime(int hour, int min, int sec) {
    // validate sec
    if (sec > 59) {
        min += sec / 60;
        this->sec = sec % 60;
    } else if (sec < 0)
        this->sec = 0;
    else
        this->sec = sec;

    // validate min
    if (min > 59) {
        hour += min / 60;
        this->min = min % 60;
    } else if (min < 0)
        this->min = 0;
    else
        this->min = min;

    // validate hour
    if (hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

/**
 Constructor to build new time object from a positive number
 of seconds.
 @param seconds Integer greater than or equal to 0.
*/
EDFTime::EDFTime(int seconds) {
    this->hour = seconds / 3600;
    seconds %= 3600;
    this->min = seconds / 60;
    this->sec = seconds % 60;
}

/**
 Constructor to parse time from string of required format
 "hh.mm.ss". Any format errors will cause the time to be
 set to 00.00.00.
 @param time String of aforementioned format.
*/
EDFTime::EDFTime(string time) {
    int hour, min, sec;
    if (time[2] != '.' || time[5] != '.'  ||
        time[0] < '0' || time[0] > '9' ||
        time[1] < '0' || time[1] > '9' ||
        time[3] < '0' || time[3] > '9' ||
        time[4] < '0' || time[4] > '9' ||
        time[6] < '0' || time[6] > '9' ||
        time[7] < '0' || time[7] > '9')
        hour = min = sec = 0;
    else {
        hour = atoi(time.substr(0, 2).c_str());
        min = atoi(time.substr(3, 2).c_str());
        sec = atoi(time.substr(6, 2).c_str());
    }

    // validate sec
    if (sec > 59) {
        min += sec / 60;
        this->sec = sec % 60;
    } else if (sec < 0)
        this->sec = 0;
    else
        this->sec = sec;

    // validate min
    if (min > 59) {
        hour += min / 60;
        this->min = min % 60;
    } else if (min < 0)
        this->min = 0;
    else
        this->min = min;

    // validate hour
    if (hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

/**
 Copy constructor.
 @param orig The object to copy.
*/
EDFTime::EDFTime(const EDFTime& orig) {
    hour = orig.hour;
    min = orig.min;
    sec = orig.sec;
}

/**
 Destructor.
*/
EDFTime::~EDFTime() {
}

/**
 Operator = overload to make a deep copy.
 @param rhs The object to copy.
*/
EDFTime& EDFTime::operator=(const EDFTime& rhs) {
    if (this != &rhs) {
        hour = rhs.hour;
        min = rhs.min;
        sec = rhs.sec;
    }

    return *this;
}

/**
 Output operator to make a readable string of this object.
 @param s An output stream reference to place the data into.
 @param time The time to stringify.
 @return The output stream.
*/
std::ostream& operator<<(std::ostream& s, EDFTime time) {
    s << std::setw(2) << std::setfill('0') << time.hour << ".";
    s << std::setw(2) << std::setfill('0') << time.min << ".";
    s << std::setw(2) << std::setfill('0') << time.sec;
    return s;
}

/**
 Get the integer hour of this object.
 @return Integer in [0, maxInt].
*/
int EDFTime::getHour() { return hour; }

/**
 Get the integer minute of this object.
 @return Integer in [0, 59].
*/
int EDFTime::getMin() { return min; }

/**
 Get the integer second of this object.
 @return Integer in [0, 59].
*/
int EDFTime::getSec() { return sec; }

/**
 Set the time's hour value. Out of range values
 will default to 0.
 @param day Integer value between 0 and maxInt.
*/
void EDFTime::setHour(int hour) {
    // validate hour
    if (hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

/**
 Set the time's minute value. Out of range values
 will default to 0.
 @param day Integer value between 0 and 59.
*/
void EDFTime::setMin(int min) {
    // validate min
    if (min < 0)
        this->min = 0;
    else if (min > 59) {
        this->hour += min / 60;
        this->min = min % 60;
    } else
        this->min = min;
}

/**
 Set the time's second value. Out of range values
 will default to 0.
 @param day Integer value between 0 and 59.
*/
void EDFTime::setSec(int sec) {
    // validate sec
    if (sec < 0)
        this->sec = 0;
    else if (sec > 59) {
        this->min += sec / 60;
        this->sec = sec % 60;
    } else
        this->sec = sec;

    if (min > 59) {
        this->hour += this->min / 60;
        this->min %= 60;
    }
}

/**
 Get the value of this time object in seconds.
 @return Integer in [0, maxInt]
*/
int EDFTime::asSeconds() {
    return hour * 3600 + min * 60 + sec;
}