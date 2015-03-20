/** 
 * @file EDFAnnotation.h
 * @brief A model file for an EDF annotation element.
 * It is comprised of a starting position (onset), a
 * length of time (duration) and a set of descriptive
 * strings.
 *
 * @author Anthony Magee
 * @date 11/4/2010
 */

#ifndef _EDFANNOTATION_H
#define	_EDFANNOTATION_H

#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

class EDFAnnotation {
public:
    EDFAnnotation() = delete;
    
    /**
     Constructor to build new annotation object.
     If onset or duration are negative values they are
     replaced with zero.
     @param onset Time position of the annotation.
     @param duration Time length of the annotation.
     @param strings Descriptive text for the annotation.
     */
    EDFAnnotation(double, double, vector<string>);
    
    /**
     Copy constructor.
     @param orig The object to copy.
     */
    EDFAnnotation(const EDFAnnotation&);
    
    virtual ~EDFAnnotation() = default;

    /**
     Operator = overload to make deep copy.
     @param rhs The object to copy.
     */
    EDFAnnotation& operator=(const EDFAnnotation&);
    
    /**
     Output operator to make a readable string of this object.
     @param s An output stream reference to place the data into.
     @param ann The annotation to stringify.
     @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream&, EDFAnnotation&);

    /**
     Get the floating point value of the onset.
     @return Double >= 0.
     */
    double getOnset();
    
    /**
     Get the floating point value of the duration.
     @return Double >= 0.
     */
    double getDuration();
    
    /**
     Get the set of descriptive strings.
     @return A vector of strings.
     */
    vector<string> getStrings();

    /**
     Set the value of the onset time. Ignores negative values and
     leaves the current value unchanged.
     @param onset New value.
     */
    void setOnset(double);
    
    /**
     Set the value of the duration time. Ignores negative values and
     leaves the current value unchanged.
     @param duration New value.
     */
    void setDuration(double);
    
    /**
     Set the annotations strings to a new set.
     @param strings New vector of descriptive strings.
     */
    void setStrings(vector<string>);
    
    /**
     Add string to strings list.
     @param string String to add.
     */
    void addString(string);
    
private:
    
    double onset;
    double duration;
    vector<string> strings;

};

#endif	/* _EDFANNOTATION_H */

