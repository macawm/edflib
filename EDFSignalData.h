/* 
 * File:   EDFSignalData.h
 *
 * Created on November 24, 2010, 3:55 PM
 */

#ifndef _EDFSIGNALDATA_H
#define	_EDFSIGNALDATA_H

#include <vector>
#include <cstdlib>

using std::vector;

class EDFSignalData {
public:
    EDFSignalData(double, double, double);
    EDFSignalData(const EDFSignalData&);
    virtual ~EDFSignalData();
    
    EDFSignalData& operator=(const EDFSignalData&);
    void addElement(int16_t);
    void addElements(vector<int16_t>);
    void addElements(int16_t*, int);
    
    double getLength(); // in seconds
    double getFrequency(); // in hertz
    vector<int16_t> getData();
    double getMax();
    double getMin();
    
private:
    double frequency; // in hertz
    double max;
    double min;
    vector<int16_t> data;
};

#endif	/* _EDFSignalData_H */