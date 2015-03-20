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

using std::fstream;
using std::string;
using std::vector;
using std::cerr;
using std::endl;


EDFFile::EDFFile(const char* path) {
    filePath = string(path);
    fileStream.open(path, std::ios::in | std::ios::binary);
    if (!fileStream.is_open() || fileStream.fail())
        cerr << "EDFFile: File '" << filePath << "' does not exist or cannot be read." << endl;
    
    header = EDFParser::Instance()->parseHeader(fileStream);
    if (header->hasAnnotations())
        annotation = EDFParser::Instance()->parseAnnotation(fileStream, header);
}

EDFFile::~EDFFile() {
    delete annotation;
    delete header;
    fileStream.close();
}

EDFHeader* EDFFile::getHeader() { return header; }

vector<EDFAnnotation>* EDFFile::getAnnotations() { return annotation; }

EDFSignalData* EDFFile::getSignalData(int channel, double start, double length) {
    if (channel == header->getAnnotationIndex() || !header->signalAvailable(channel))
        return nullptr;

    // fix length arg if it goes beyond end of recording length
    if (start + length > header->getRecordingTime())
        length = header->getRecordingTime() - start;

    return EDFParser::Instance()->parseSignal(fileStream, header, channel, start, length);
}
