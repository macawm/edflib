/** 
 @file EDFAnnotation.cpp
 @brief A model file for an EDF annotation element.
 It is comprised of a starting position (onset), a
 length of time (duration) and a set of descriptive
 strings.
 
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFAnnotation.h"
#include <iomanip>

/**
 Constructor to build new annotation object.
 If onset or duration are negative values they are
 replaced with zero.
 @param onset Time position of the annotation.
 @param duration Time length of the annotation.
 @param strings Descriptive text for the annotation.
*/
EDFAnnotation::EDFAnnotation(double onset,
        double duration, vector<string> strings) {
    this->onset = (onset >= 0) ? onset : 0;
    this->duration = (duration >= 0) ? duration : 0;
    this->strings = strings;
}

/**
 Copy constructor.
 @param orig The object to copy.
*/
EDFAnnotation::EDFAnnotation(const EDFAnnotation& orig) {
    onset = orig.onset;
    duration = orig.duration;
    strings = orig.strings;
}

/**
 Destructor.
*/
EDFAnnotation::~EDFAnnotation() {}

/**
 Operator = overload to make deep copy.
 @param rhs The object to copy.
*/
EDFAnnotation& EDFAnnotation::operator=(const EDFAnnotation& rhs) {
    if (this != &rhs) {
        onset = rhs.onset;
        duration = rhs.duration;
        strings = rhs.strings;
    }

    return *this;
}

/**
 Output operator to make a readable string of this object.
 @param s An output stream reference to place the data into.
 @param ann The annotation to stringify.
 @return The output stream.
*/
std::ostream& operator<<(std::ostream& s, EDFAnnotation& ann) {
    vector<string>::iterator it;
    s << std::fixed << std::setprecision(1);
    for (it = ann.strings.begin(); it != ann.strings.end(); it++)
        s << ann.onset << " " << ann.duration << " " << *it << std::endl;
        
    return s;
}

/**
 Get the floating point value of the onset.
 @return Double >= 0.
*/
double EDFAnnotation::getOnset() { return onset; }

/**
 Get the floating point value of the duration.
 @return Double >= 0.
*/
double EDFAnnotation::getDuration() { return duration; }

/**
 Get the set of descriptive strings.
 @return A vector of strings.
*/
vector<string> EDFAnnotation::getStrings() { return strings; }

/**
 Set the value of the onset time. Ignores negative values and 
 leaves the current value unchanged.
 @param onset New value.
*/
void EDFAnnotation::setOnset(double onset) {
    if (onset >= 0)
	this->onset = onset;
}

/**
 Set the value of the duration time. Ignores negative values and 
 leaves the current value unchanged.
 @param duration New value.
*/
void EDFAnnotation::setDuration(double duration) {
    if (duration >= 0)
	this->duration = duration;
}

/**
 Set the annotations strings to a new set.
 @param strings New vector of descriptive strings.
*/
void EDFAnnotation::setStrings(vector<string> strings) {
    this->strings = strings;
}
