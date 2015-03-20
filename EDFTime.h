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
#include <exception>

using std::string;

class EDFTime {
public:
    EDFTime();
    
    /**
     Constructor to parse time from string of required format
     "hh.mm.ss". Any format errors will cause the time to be
     set to 00.00.00.
     @param time String of aforementioned format.
     */
    EDFTime(string);
    
    /**
     Constructor to build new time object from a positive number
     of seconds.
     @param seconds Integer greater than or equal to 0.
     */
    EDFTime(int);
    
    /**
     Constructor to build new time object. Out of
     range values will be adjusted as appropriate.
     @param hour Integer value between 0 and 59.
     @param min Integer value between 0 and 59.
     @param sec Integer value greater than or equal to 0.
     */
    EDFTime(int, int, int);
    
    /**
     Copy constructor.
     @param orig The object to copy.
     */
    EDFTime(const EDFTime&);
    
    /**
     Destructor.
     */
    virtual ~EDFTime() {};

    /**
     Operator = overload to make a deep copy.
     @param rhs The object to copy.
     */
    EDFTime& operator=(const EDFTime&);
    
    /**
     Output operator to make a readable string of this object.
     @param s An output stream reference to place the data into.
     @param time The time to stringify.
     @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream&, EDFTime);

    /**
     Get the integer hour of this object.
     @return Integer in [0, maxInt].
     */
    int getHour();
    
    /**
     Get the integer minute of this object.
     @return Integer in [0, 59].
     */
    int getMin();
    
    /**
     Get the integer second of this object.
     @return Integer in [0, 59].
     */
    int getSec();

    /**
     Set the time's hour value.
     @param day Integer value between 0 and maxInt. (Values below 0 are ignored.)
     */
    void setHour(int);
    
    /**
     Set the time's minute value.
     @param day Integer value between 0 and 59. (Values below 0 are ignored and values over 59 are rectified as hours and minutes.)
     */
    void setMin(int);
    
    /**
     Set the time's second value.
     @param day Integer value between 0 and 59.
     */
    void setSec(int);

    /**
     Get the value of this time object in seconds.
     @return Integer in [0, maxInt]
     */
    int asSeconds();
    
    
private:
    int hour, min, sec;
    
protected:
    void rectifyTime();
};

#endif	/* _EDFTIME_H */

