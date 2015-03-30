/** 
 @file EDFDate.h 
 @brief A model object for an EDF date.
 EDF specified dates are only valid between
 1/1/1985 through 12/31/2084. They are
 formatted as such dd.mm.yy and must not
 exclude leading zeros.
 
 @author Anthony Magee
 @date 11/4/2010
*/

#ifndef _EDFDATE_H
#define	_EDFDATE_H

#include <string>
#include <ostream>

class EDFDate {
public:
    EDFDate();
    
    /**
     Constructor to parse date from string of required
     format "dd.mm.yy". Any format errors will cause the date
     to be set to 01.01.85.
     @param date String of aforementioned format.
     */
    EDFDate(const std::string&);
    
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
     Test deep equality.
     @return true if all properties are equal, otherwise false.
     */
    bool operator==(const EDFDate&);

    /**
     Get the integer day of month value.
     @return Integer in [1, 31].
     */
    int day() const;
    
    /**
     Get the integer month of year value.
     @return Integer in [1, 12].
     */
    int month() const;
    
    /**
     Get the integer year value.
     @return Integer in [0, 99].
     */
    int year() const;
    
    /**
     Get the integer four digit year value.
     @return Integer in [1985, 2084].
     */
    int fullYear() const;

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
    static std::string monthIntToText(int);
    
    /**
     Translate a three letter month string to a month digit.
     Invalid months will return -1.
     @param mon Three letter month string, JAN, FEB, ...
     @return int of the month [1, 12]
     */
    static int monthTextToInt(std::string&);
    
    
private:
    int d_day, d_month, d_year;
};

#endif	/* _EDFDATE_H */

