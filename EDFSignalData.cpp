/* 
 * File:   EDFSignalData.cpp
 *
 * Created on November 24, 2010, 3:55 PM
 */

#include "EDFSignalData.h"
#include <iomanip>
#include <cmath>

EDFSignalData::EDFSignalData(double newFrequency, double max, double min) {
    this->max = max;
    this->min = min;
    frequency = newFrequency;
}

EDFSignalData::EDFSignalData(const EDFSignalData& orig) {
    max = orig.max;
    min = orig.min;
    frequency = orig.frequency;
    data = orig.data;
}

EDFSignalData::~EDFSignalData() {}

EDFSignalData& EDFSignalData::operator=(const EDFSignalData& rhs) {
    max = rhs.max;
    min = rhs.min;
    frequency = rhs.frequency;
    data = rhs.data;
    
    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFSignalData& data) {
    vector<int16_t> raw = data.getData();
    s << std::endl << data.getLength() / data.getFrequency() << " seconds  " << data.getLength() << " samples" << std::endl;
    
    int width = 0;
    int l = data.getLength();
    while (l > 0) {
        l /= 10;
        width++;
    }
    
    int i = 0;
    for (vector<int16_t>::iterator it = raw.begin(); it != raw.end(); it++, i++) {
        if (i % 20 == 0)
            s << std::endl << std::setw(width) << i << ":  ";
        s << *it << " ";
    }
    s << std::endl << std::endl;
    
    return s;
}

void EDFSignalData::addElement(int16_t val) {
    data.push_back(val);
}

void EDFSignalData::addElements(vector<int16_t> vals) {
    for (vector<int16_t>::iterator it = vals.begin(); it != vals.end(); it++) 
        data.push_back(*it);
}

void EDFSignalData::addElements(int16_t* vals, int length) {
    for (int i = 0; i < length; i++)
        data.push_back(vals[i]);
}

double EDFSignalData::getLength() { return data.size(); }

double EDFSignalData::getFrequency() { return frequency; }

vector<int16_t> EDFSignalData::getData() { return data; }

double EDFSignalData::getMax() { return max; }

double EDFSignalData::getMin() { return min; }