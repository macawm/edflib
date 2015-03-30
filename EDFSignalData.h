/**
 @file EDFSignalData.h
 @brief A model object for EDF data. The channel range is not enforced when signal data is added,
 an error message indicates out of range values.
 Additional statical methods for inspecting the data are added here irregularly:
  signal minimum
  singal maximum
  signal average
 
 @author Anthony Magee
 @date 11/24/2010
 */

#ifndef _EDFSIGNALDATA_H
#define	_EDFSIGNALDATA_H

#include <vector>

class EDFSignalData {
public:
    EDFSignalData() = delete;
    
    /**
     Constructor to build new signal data object.
     Signal data should be added via one of the addElement methods.
     @param frequency The channel frequency.
     @param channelMax Maximum value of signal values.
     @param channelMin Minimum value of signal values.
     */
    EDFSignalData(double, double, double);
    
    /**
     Copy constructor.
     @param orig The object to copy.
     */
    EDFSignalData(const EDFSignalData&);
    
    virtual ~EDFSignalData() = default;
    
    /**
     Operator = overload to make deep copy.
     @param rhs The object to copy.
     */
    EDFSignalData& operator=(const EDFSignalData&);
    friend std::ostream& operator<<(std::ostream&, EDFSignalData&);
    
    /**
     Add single element to end signal data.
     @param val double value to add to signal data.
     */
    void addDataPoint(double);
    
    /**
     Add set of elements to end signal data.
     @param vals double values to add to signal data.
     @param length length of vals array.
     */
    void addDataPoints(const double*, size_t);
    
    /**
     Get length of object's data.
     @return Number of data points stored
     */
    size_t size() const;
    
    /**
     Get the frequency of the data stored in hertz.
     @return Frequency of the signal data points.
     */
    double frequency() const;
    
    /**
     Get the length of time the data represents in seconds.
     @return Time length of data.
     */
    double time() const;
    
    /**
     Get the maximum value the data's channel should handle.
     @return Maximum channel value.
     */
    double channelMax() const;
    
    /**
     Get the minimum value the data's channel should handle.
     @return Minimum channel value.
     */
    double channelMin() const;
    
    /**
     Get the maximum value present in the data.
     @return Maximum data point.
     */
    double max() const;
    
    /**
     Get the minimum value present in the data.
     @return Minimum data point.
     */
    double min() const;
    
    /**
     Get the raw data stored.
     @return List of data stored for object.
     */
    std::vector<double> data() const;
    
    /**
     Get the statistically standardized version of the data stored. Mean should be 0 and stddev should be 1.
     @return Standardized data stored for object.
     */
    std::vector<double> standardizedData() const;
    
    /* Statistics and lies */
    double mean() const;
    
    /**
     Sample variance of data.
     @return Statistically stable calculated second central moment.
     */
    double variance() const;
    
    /**
     Standard variation of data.
     @return Statistically stable calculated standard deviation.
     */
    double stddev() const;
    
    /**
     Sample skewness of data.
     @return Statistically stable calculated third central moment.
     */
    double skewness() const;
    
    /**
     Sample kurtosis of data.
     @return Statistically stable calculated fourth central moment.
     */
    double kurtosis() const;
    
private:
    std::vector<double> dataPoints;
    
    double sFrequency; // in hertz
    double cMax, cMin;
    double sMax, sMin;
    
    // running totals of these values make for better statistical accuracy in IEEE754
    double m_1, m_2, m_3, m_4;
    
    bool valueInRange(double) const;
    void updateSignalMaxMin(double);
};

#endif	/* _EDFSignalData_H */