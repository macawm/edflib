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


EDFAnnotation::EDFAnnotation(double onset,
        double duration, vector<string> strings) {
    this->onset = (onset >= 0) ? onset : 0;
    this->duration = (duration >= 0) ? duration : 0;
    this->strings = strings;
}

EDFAnnotation::EDFAnnotation(const EDFAnnotation& orig) {
    onset = orig.onset;
    duration = orig.duration;
    strings = orig.strings;
}

EDFAnnotation& EDFAnnotation::operator=(const EDFAnnotation& rhs) {
    if (this != &rhs) {
        onset = rhs.onset;
        duration = rhs.duration;
        strings = rhs.strings;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFAnnotation& ann) {
    vector<string>::iterator it;
    s << std::fixed << std::setprecision(1);
    for (it = ann.strings.begin(); it != ann.strings.end(); it++)
        s << ann.onset << " " << ann.duration << " " << *it << std::endl;
        
    return s;
}

double EDFAnnotation::getOnset() { return onset; }

double EDFAnnotation::getDuration() { return duration; }

vector<string> EDFAnnotation::getStrings() { return strings; }


void EDFAnnotation::setOnset(double onset) {
    if (onset >= 0)
	this->onset = onset;
}

void EDFAnnotation::setDuration(double duration) {
    if (duration >= 0)
	this->duration = duration;
}

void EDFAnnotation::setStrings(vector<string> strings) {
    this->strings = strings;
}

void EDFAnnotation::addString(string str) {
    strings.push_back(str);
}
