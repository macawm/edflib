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


class EDFFile {
public:
    /**
     Contructor to build new EDF file object. Access
     to the data of an EDF file should start by instantiating
     this class.
     @param path Path to the EDF file on disk.
     */
    EDFFile(const char*);
    
    /**
     Destructor.
     */
    virtual ~EDFFile();

    /**
     Get the header object associated with this file.
     @return EDF header object.
     */
    EDFHeader* getHeader();
    
    /**
     Get the annotations channel information vector.
     @return EDF Annotation vector or nullptr if channel does not exist.
     */
    std::vector<EDFAnnotation>* getAnnotations();
    
    /**
     Get a portion of a channel's signal information.
     @param channel The channel to extract information from.
     @param start The starting time in fractional seconds to
     begin signal data extraction.
     @param length The length of time in fractional seconds of
     signal information to extract. If the length is greater than the
     signal data, the time will be truncated to the max data length.
     @return An EDFSignalData object containing the data extracted from file.
     The following issues will cause a nullptr value to be returned:
     Attempting to access the annotation channel.
     Attempting to extract data from an nonexistent channel.
     */
    EDFSignalData* getSignalData(int, double, double);

private:
    std::string filePath;
    std::fstream fileStream;
    EDFHeader* header;
    std::vector<EDFAnnotation>* annotation;
};

#endif	/* _EDFFILE_H */

