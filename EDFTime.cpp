/**
 @file EDFTime.cpp
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFTime.h"
#include "EDFUtil.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

void EDFTime::rectifyTime() {
    if (t_sec > 59) {
        t_min += t_sec / 60;
        t_sec = t_sec % 60;
    }
    
    if (t_min > 59) {
        t_hour += t_min / 60;
        t_min = t_min % 60;
    }
}

EDFTime::EDFTime()
    : t_hour(0)
    , t_min(0)
    , t_sec(0)
{}

EDFTime::EDFTime(int hour, int min, int sec) {
    if (sec >= 0) {
        this->t_sec = sec;
    }
    
    else {
        this->t_sec = 0;
        DI(cerr << "* RT  Could not set seconds to negative value: " << sec << endl)
    }
    
    if (min >= 0) {
        this->t_min = min;
    }
    
    else {
        this->t_min = 0;
        DI(cerr << "* RT  Could not set minutes to negative value: " << min << endl)
    }
    
    if (hour >= 0) {
        this->t_hour = hour;
    }
    
    else {
        this->t_hour = 0;
        DI(cerr << "* RT  Could not set hour to negative value: " << hour << endl)
    }
    
    rectifyTime();
}


EDFTime::EDFTime(int seconds) {
    if (seconds >= 0) {
        this->t_hour = seconds / 3600;
        seconds %= 3600;
        this->t_min = seconds / 60;
        this->t_sec = seconds % 60;
    }
    
    else {
        this->t_sec = this->t_min = this->t_hour = 0;
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
        this->t_sec = this->t_min = this->t_hour = 0;
        DI(cerr << "* RT  Could not parse string: " << time << endl)
    }
    
    else {
        t_hour = atoi(time.substr(0, 2).c_str());
        t_min  = atoi(time.substr(3, 2).c_str());
        t_sec  = atoi(time.substr(6, 2).c_str());
    }

    rectifyTime();
}


EDFTime::EDFTime(const EDFTime& orig) {
    t_hour = orig.t_hour;
    t_min = orig.t_min;
    t_sec = orig.t_sec;
}


EDFTime& EDFTime::operator=(const EDFTime& rhs) {
    if (this != &rhs) {
        t_hour = rhs.t_hour;
        t_min = rhs.t_min;
        t_sec = rhs.t_sec;
    }

    return *this;
}


std::ostream& operator<<(std::ostream& s, EDFTime time) {
    s << std::setw(2) << std::setfill('0') << time.t_hour << ".";
    s << std::setw(2) << std::setfill('0') << time.t_min << ".";
    s << std::setw(2) << std::setfill('0') << time.t_sec;
    return s;
}


int EDFTime::hour() const { return t_hour; }

int EDFTime::minute() const { return t_min; }

int EDFTime::second() const { return t_sec; }


void EDFTime::setHour(int hour) {
    if (hour >= 0) {
        this->t_hour = hour;
        // no need to rectify here
    }

    else {
        DI(cerr << "* RT  Could not set hours to negative value: " << hour << endl)
    }
}


void EDFTime::setMinute(int min) {
    if (min >= 0) {
        this->t_min = min;
        rectifyTime();
    }

    else {
        DI(cerr << "* RT  Could not set minutes to negative value: " << min << endl)
    }
}


void EDFTime::setSecond(int sec) {
    if (sec >= 0) {
        this->t_sec = sec;
        rectifyTime();
    }
    
    else {
        DI(cerr << "* RT  Could not set seconds to negative value: " << sec << endl)
    }
}


int EDFTime::asSeconds() const {
    return 60 * (60 * t_hour + t_min) + t_sec;
}
