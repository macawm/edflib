/** 
 @file EDFDate.h
 @brief A model file for an EDF annotation element.
 It is comprised of a starting position (onset), a
 length of time (duration) and a set of descriptive
 strings.
 
 @author Anthony Magee
 @date 11/4/2010
*/

#ifndef _EDFDATE_H
#define	_EDFDATE_H

#include <string>
#include <ostream>

using std::string;

class EDFDate {
public:
    EDFDate() {};
    EDFDate(string);
    EDFDate(int, int, int);
    EDFDate(const EDFDate&);
    virtual ~EDFDate();

    EDFDate& operator=(const EDFDate&);
    friend std::ostream& operator<<(std::ostream&, EDFDate);

    int getDay();
    int getMonth();
    int getYear();
    int getFullYear();

    void setDay(int);
    void setMonth(int);
    void setYear(int);
private:
    int day, month, year;
    int fullYear;
    string textMonth;

    string monthIntToText(int);
};

#endif	/* _EDFDATE_H */

