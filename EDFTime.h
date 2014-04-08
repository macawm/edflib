/**
 @file EDFTime.h
 @brief A model file for an EDF time.
 EDF specified times are formatted as such hh.mm.ss
 and must not exclude leading zeros.

 @author Anthony Magee
 @date 11/4/2010
*/

#ifndef _EDFTIME_H
#define	_EDFTIME_H

#include <string>
#include <ostream>

using std::string;

class EDFTime {
public:
    EDFTime() {};
    EDFTime(string);
    EDFTime(int);
    EDFTime(int, int, int);
    EDFTime(const EDFTime&);
    virtual ~EDFTime();

    EDFTime& operator=(const EDFTime&);
    friend std::ostream& operator<<(std::ostream&, EDFTime);

    int getHour();
    int getMin();
    int getSec();

    void setHour(int);
    void setMin(int);
    void setSec(int);

    int asSeconds();
private:
    int hour, min, sec;
};

#endif	/* _EDFTIME_H */

