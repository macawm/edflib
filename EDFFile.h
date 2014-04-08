/** 
 @file EDFFile.cpp
 @brief A model and access file for EDF data.
 Holds information about the file stream, headers
 and signal data.

 This is the primary access class for extracting
 data from an EDF file.
 
 @author Anthony Magee
 @date 11/4/2010
*/

#ifndef _EDFFILE_H
#define	_EDFFILE_H

#include <string>
#include <fstream>
#include <vector>
#include "EDFHeader.h"
#include "EDFAnnotation.h"
#include "EDFSignalData.h"

using std::fstream;
using std::string;
using std::vector;

class EDFFile {
public:
    EDFFile(const char*);
    virtual ~EDFFile();

    EDFHeader* getHeader();
    vector<EDFAnnotation>* getAnnotations();
    EDFSignalData* getSignalData(int, double, double);
    int getRealChannelCount();

private:
    string filePath;
    fstream fileStream;
    EDFHeader* header;
    vector<EDFAnnotation>* annotation;

    int realChannelCount;
};

#endif	/* _EDFFILE_H */

