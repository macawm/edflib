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
    EDFAnnotation() {};
    EDFAnnotation(double, double, vector<string>);
    EDFAnnotation(const EDFAnnotation&);
    virtual ~EDFAnnotation();

    EDFAnnotation& operator=(const EDFAnnotation&);
    friend std::ostream& operator<<(std::ostream&, EDFAnnotation&);

    double getOnset();
    double getDuration();
    vector<string> getStrings();

    void setOnset(double);
    void setDuration(double);
    void setStrings(vector<string>);
private:
    double onset;
    double duration;
    vector<string> strings;

};

#endif	/* _EDFANNOTATION_H */

