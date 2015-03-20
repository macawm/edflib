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
    EDFDate();
    
    /**
     Constructor to parse date from string of required
     format "dd.mm.yy". Any format errors will cause the date
     to be set to 01.01.85.
     @param date String of aforementioned format.
     */
    EDFDate(const string&);
    
    /**
     Constructor to build new date object. Out of
     range values will default to 1.
     @param day Integer value between 1 and 31.
     @param month Integer value between 1 and 12.
     @param year Integer value between 0 and 99.
     */
    EDFDate(int, int, int);
    
    /**
     Copy constructor.
     @param orig The object to copy.
     */
    EDFDate(const EDFDate&);
    
    virtual ~EDFDate() {};

    /**
     Operator = overload to make a deep copy.
     @param rhs The object to copy.
     */
    EDFDate& operator=(const EDFDate&);
    
    /**
     Output operator to make a readable string of this object.
     @param s An output stream reference to place the data into.
     @param date The date to stringify.
     @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream&, EDFDate);

    /**
     Get the integer day of month value.
     @return Integer in [1, 31].
     */
    int getDay();
    
    /**
     Get the integer month of year value.
     @return Integer in [1, 12].
     */
    int getMonth();
    
    /**
     Get the integer year value.
     @return Integer in [0, 99].
     */
    int getYear();
    
    /**
     Get the integer four digit year value.
     @return Integer in [1985, 2084].
     */
    int getFullYear();

    /**
     Set the date's day value. Out of range values
     will default to 1.
     @param day Integer value between 1 and 31.
     */
    void setDay(int);
    
    /**
     Set the date's month value. Out of range values
     will default to 1.
     @param month Integer value between 1 and 12.
     */
    void setMonth(int);
    
    /**
     Set the date's year value. Out of range values
     will default to 1.
     @param year Integer value between 0 and 99.
     */
    void setYear(int);
    
    /**
     Translate month digit to three letter month.
     Invalid digits will return empty string.
     @param mon Month digit between 1 and 12.
     @return String containing three letter month
     representation or empty string.
     */
    static string monthIntToText(int);
    
    /**
     Translate a three letter month string to a month digit.
     Invalid months will return -1.
     @param mon Three letter month string, JAN, FEB, ...
     @return int of the month [1, 12]
     */
    static int monthTextToInt(string&);
    
    
private:
    int day, month, year;
};

#endif	/* _EDFDATE_H */

