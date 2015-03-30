/** 
 @file EDFAnnotation.cpp 
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFAnnotation.h"
#include <iomanip>

using std::string;
using std::vector;

EDFAnnotation::EDFAnnotation(double onset,
        double duration, vector<string> strings) {
    this->a_onset = (onset >= 0) ? onset : 0;
    this->a_duration = (duration >= 0) ? duration : 0;
    this->a_strings = strings;
}

EDFAnnotation::EDFAnnotation(const EDFAnnotation& orig) {
    a_onset = orig.a_onset;
    a_duration = orig.a_duration;
    a_strings = orig.a_strings;
}

EDFAnnotation& EDFAnnotation::operator=(const EDFAnnotation& rhs) {
    if (this != &rhs) {
        a_onset = rhs.a_onset;
        a_duration = rhs.a_duration;
        a_strings = rhs.a_strings;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFAnnotation& ann) {
    vector<string>::iterator it;
    s << std::fixed << std::setprecision(1);
    for (string str : ann.a_strings)
        s << ann.a_onset << " " << ann.a_duration << " " << str << std::endl;
        
    return s;
}

double EDFAnnotation::onset() const { return a_onset; }

double EDFAnnotation::duration() const { return a_duration; }

vector<string> EDFAnnotation::strings() const { return a_strings; }


void EDFAnnotation::setOnset(double onset) {
    if (onset >= 0)
        this->a_onset = onset;
}

void EDFAnnotation::setDuration(double duration) {
    if (duration >= 0)
        this->a_duration = duration;
}

void EDFAnnotation::setStrings(vector<string> strings) {
    this->a_strings = strings;
}

void EDFAnnotation::addString(string str) {
    a_strings.push_back(str);
}
