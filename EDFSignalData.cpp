/**
 @file EDFSignalData.cpp 
 @author Anthony Magee
 @date 11/24/2010
 */

#include "EDFSignalData.h"
#include <iomanip>
#include <iostream>
#include <cmath>
#include <limits>
#include <numeric>

using std::vector;

EDFSignalData::EDFSignalData(double frequency, double channelMax, double channelMin)
    : sMax(std::numeric_limits<double>::infinity())
    , sMin(std::numeric_limits<double>::infinity())
    , m_1(0), m_2(0), m_3(0), m_4(0)
{
    this->cMax = channelMax;
    this->cMin = channelMin;
    this->sFrequency = frequency;
}

EDFSignalData::EDFSignalData(const EDFSignalData& orig) {
    cMax = orig.cMax;
    cMin = orig.cMin;
    sMax = orig.sMax;
    sMin = orig.sMin;
    sFrequency = orig.sFrequency;
    dataPoints = orig.dataPoints;
    
    m_1 = orig.m_1;
    m_2 = orig.m_2;
    m_3 = orig.m_3;
    m_4 = orig.m_4;
}

EDFSignalData& EDFSignalData::operator=(const EDFSignalData& rhs) {
    if (this != &rhs) {
        cMax = rhs.cMax;
        cMin = rhs.cMin;
        sMax = rhs.sMax;
        sMin = rhs.sMin;
        sFrequency = rhs.sFrequency;
        dataPoints = rhs.dataPoints;
        
        m_1 = rhs.m_1;
        m_2 = rhs.m_2;
        m_3 = rhs.m_3;
        m_4 = rhs.m_4;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFSignalData& data) {
    vector<double> raw = data.data();
    s << std::endl << data.time() << " seconds  " << data.size() << " samples" << std::endl;
    
    int width = 0;
    size_t l = data.size();
    while (l > 0) {
        l /= 10;
        width++;
    }
    
    int i = 0;
    for (auto bit : raw) {
        if (i % 20 == 0)
            s << std::endl << std::setw(width) << i << ":  ";
        s << bit << " ";
    }
    s << std::endl << std::endl;
    
    return s;
}

void EDFSignalData::updateSignalMaxMin(double val) {
    // flags for unset min and max
    if (sMax == std::numeric_limits<double>::infinity() && sMin == std::numeric_limits<double>::infinity())
        sMax = sMin = val;
    else if (val > sMax)
        sMax = val;
    else if (val < sMin)
        sMin = val;
}

bool EDFSignalData::valueInRange(double val) const {
    if (val < cMin || val > cMax) {
        std::cerr << "Value is out of range of signal definition: " << val << std::endl;
        return false;
    }
    return true;
}

void EDFSignalData::addDataPoint(double val) {
    valueInRange(val);
    updateSignalMaxMin(val);
    dataPoints.push_back(val);
    
    // update online stats
    // based on John D. Cook's implementation of Knuth & Welford stable algorithm
    // as presented in 'Formulas for Robust, One-Pass Parallel Computation of Covariances and Arbitrary-Order Statistical Moments'
    size_t n = dataPoints.size();
    double delta    = val - m_1;
    double delta_n  = delta / n;
    double delta_n2 = delta_n * delta_n;
    double term1    = delta * delta_n * (n - 1);
    
    m_4 += term1 * delta_n2 * (n*n - 3*n + 3) + (6 * delta_n2 * m_2) - (4 * delta_n * m_3);
    m_3 += term1 * delta_n * (n-2) - (3 * delta_n * m_2);
    m_2 += term1;
    m_1 += delta_n;
}

void EDFSignalData::addDataPoints(const double* vals, size_t length) {
    for (size_t i = 0; i < length; i++)
        addDataPoint(vals[i]);
}

size_t EDFSignalData::size() const { return dataPoints.size(); }

double EDFSignalData::frequency() const { return sFrequency; }

double EDFSignalData::time() const { return dataPoints.size() / sFrequency; }

vector<double> EDFSignalData::data() const { return dataPoints; }

double EDFSignalData::channelMax() const { return cMax; }

double EDFSignalData::channelMin() const { return cMin; }


double EDFSignalData::max() const { return sMax; }

double EDFSignalData::min() const { return sMin; }

vector<double> EDFSignalData::standardizedData() const {
    double mean = this->mean();
    double stddev = this->stddev();
    
    vector<double> newData;
    for (auto val : dataPoints)
        newData.push_back((val - mean) / stddev);
    return newData;
}

double EDFSignalData::mean() const {
    return m_1;
}

double EDFSignalData::stddev() const {
    return sqrt(variance());
}

double EDFSignalData::variance() const {
    return m_2/(dataPoints.size() - 1);
}

double EDFSignalData::skewness() const {
    return sqrt(dataPoints.size()) * m_3 / pow(m_2, 1.5);
}

double EDFSignalData::kurtosis() const {
    return dataPoints.size() * m_4 / (m_2 * m_2); // for coefficient of excess subtract three (3) from the kurtosis
}
