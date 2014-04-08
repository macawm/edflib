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

#include "EDFFile.h"
#include <iostream>
#include "EDFUtil.h"
#include "EDFParser.h"

using std::cerr;
using std::endl;

/**
 Contructor to build new EDF file object. Access
 to the data of an EDF file should start by instantiating
 this class.
 @param path Path to the EDF file on disk.
*/
EDFFile::EDFFile(const char* path) {
    filePath = string(path);
    fileStream.open(path, std::ios::in | std::ios::binary);
    if (!fileStream.is_open())
        cerr << "EDFFile: File '" << filePath << "' does not exist or cannot be read." << endl;
    
    header = EDFParser::Instance()->parseHeader(fileStream);
    if (header->hasAnnotations())
        annotation = EDFParser::Instance()->parseAnnotation(fileStream, header);
}

/**
 Destructor.
*/
EDFFile::~EDFFile() {
    delete annotation;
    delete header;
    fileStream.close();
}

/**
 Get the header object associated with this file.
 @return EDF header object.
*/
EDFHeader* EDFFile::getHeader() { return header; }

/**
 Get the number of real channels available for signal data access.
 @return Real channel count.
*/
int EDFFile::getRealChannelCount() { return realChannelCount; }

/**
 Get the annotations channel information vector.
 @return EDF Annotation vector or NULL if channel does not exist.
*/
vector<EDFAnnotation>* EDFFile::getAnnotations() { return annotation; }

/**
 Get a portion of a channel's signal information.
 @param channel The channel to extract information from.
 @param start The starting time in fractional seconds to
 begin signal data extraction.
 @param length The length of time in fractional seconds of
 signal information to extract. If the length is greater than the
 signal data, the time will be truncated to the max data length.
 @return An EDFSignalData object containing the data extracted from file.
 The following issues will cause a NULL value to be returned:
  Attempting to access the annotation channel.
  Attempting to extract data from an nonexistent channel.
*/
EDFSignalData* EDFFile::getSignalData(int channel, double start, double length) {
    if (channel == header->getAnnotationIndex()) return NULL;
    if (channel < 0 && channel > header->getSignalCount()) return NULL;

    if (start + length > header->getDataRecordCount() * header->getDataRecordDuration())
        length = header->getDataRecordCount() * header->getDataRecordDuration() - start;

    return EDFParser::Instance()->parseSignal(fileStream, header, channel, start, length);
}
