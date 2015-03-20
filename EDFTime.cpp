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
#include "EDFUtil.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

using std::cerr;
using std::endl;

void EDFTime::rectifyTime() {
    if (sec > 59) {
        min += sec / 60;
        sec = sec % 60;
    }
    
    if (min > 59) {
        hour += min / 60;
        min = min % 60;
    }
}

EDFTime::EDFTime() : EDFTime(0, 0, 0) {}

EDFTime::EDFTime(int hour, int min, int sec) {
    if (sec >= 0) {
        this->sec = sec;
    }
    
    else {
        this->sec = 0;
        DI(cerr << "* RT  Could not set seconds to negative value: " << sec << endl)
    }
    
    if (min >= 0) {
        this->min = min;
    }
    
    else {
        this->min = 0;
        DI(cerr << "* RT  Could not set minutes to negative value: " << min << endl)
    }
    
    if (hour >= 0) {
        this->hour = hour;
    }
    
    else {
        this->hour = 0;
        DI(cerr << "* RT  Could not set hour to negative value: " << hour << endl)
    }
    
    rectifyTime();
}


EDFTime::EDFTime(int seconds) {
    if (seconds >= 0) {
        this->hour = seconds / 3600;
        seconds %= 3600;
        this->min = seconds / 60;
        this->sec = seconds % 60;
    }
    
    else {
        this->sec = this->min = this->hour = 0;
        DI(cerr << "* RT  Could not set time to negative value: " << seconds << endl)
    }
}


EDFTime::EDFTime(string time) {
    if (time[2] != '.' || time[5] != '.'  ||
        time[0] < '0' || time[0] > '9' ||
        time[1] < '0' || time[1] > '9' ||
        time[3] < '0' || time[3] > '9' ||
        time[4] < '0' || time[4] > '9' ||
        time[6] < '0' || time[6] > '9' ||
        time[7] < '0' || time[7] > '9')
    {
        this->sec = this->min = this->hour = 0;
        DI(cerr << "* RT  Could not parse string: " << time << endl)
    }
    
    else {
        hour = a2i(time.substr(0, 2));
        min  = a2i(time.substr(3, 2));
        sec  = a2i(time.substr(6, 2));
    }

    rectifyTime();
}


EDFTime::EDFTime(const EDFTime& orig) {
    hour = orig.hour;
    min = orig.min;
    sec = orig.sec;
}


EDFTime& EDFTime::operator=(const EDFTime& rhs) {
    if (this != &rhs) {
        hour = rhs.hour;
        min = rhs.min;
        sec = rhs.sec;
    }

    return *this;
}


std::ostream& operator<<(std::ostream& s, EDFTime time) {
    s << std::setw(2) << std::setfill('0') << time.hour << ".";
    s << std::setw(2) << std::setfill('0') << time.min << ".";
    s << std::setw(2) << std::setfill('0') << time.sec;
    return s;
}


int EDFTime::getHour() { return hour; }

int EDFTime::getMin() { return min; }

int EDFTime::getSec() { return sec; }


void EDFTime::setHour(int hour) {
    if (hour >= 0) {
        this->hour = hour;
        // no need to rectify here
    }

    else {
        DI(cerr << "* RT  Could not set hours to negative value: " << hour << endl)
    }
}


void EDFTime::setMin(int min) {
    if (min >= 0) {
        this->min = min;
        rectifyTime();
    }

    else {
        DI(cerr << "* RT  Could not set minutes to negative value: " << min << endl)
    }
}


void EDFTime::setSec(int sec) {
    if (sec >= 0) {
        this->sec = sec;
        rectifyTime();
    }
    
    else {
        DI(cerr << "* RT  Could not set seconds to negative value: " << sec << endl)
    }
}


int EDFTime::asSeconds() {
    return hour * 3600 + min * 60 + sec;
}
