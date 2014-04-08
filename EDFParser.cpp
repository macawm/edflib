/* 
 * File:   EDFParser.cpp
 * 
 * Created on November 4, 2010, 3:36 PM
 */

#include "EDFParser.h"
#include "EDFUtil.h"
#include "EDFPatient.h"
#include "EDFSignalData.h"
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>

using std::cerr;
using std::endl;

EDFParser* EDFParser::instance = NULL;

EDFParser* EDFParser::Instance() {
    if (!instance)
        instance = new EDFParser;
    return instance;
}

EDFHeader* EDFParser::parseHeader(std::fstream &in) {
    EDFHeader* header = new EDFHeader();

    // rewind file
    in.seekg(0, std::ios::beg);
    // read first 256 characters of file
    char rootHeaderArray[257] = {0};
    if (in.fail() || !in.get(rootHeaderArray, 257)) {
        cerr << "Error trying to read preheader from file. Giving up..." << endl;
        return NULL;
    }
    string rootHeader(rootHeaderArray);

    // verify that header data characters are in valid range
    for (int i = 0; i < 256; i++)
        if (rootHeaderArray[i] < 32 || rootHeaderArray[i] > 126) {
            cerr << "Non-ASCII characters detected in header" << endl;
            return NULL;
        }

    // keep track of where we are in the header bytes
    int headerLoc = 0;

    // extract file version - 8 characters
    string version = rootHeader.substr(headerLoc , 8);
    headerLoc += 8;
    if (version[0] != '0')
        cerr << "Magic number is wrong." << endl <<
                "All current versions of EDF specifications must" <<
                " start with '0' character. Ignoring..." << endl;

    // extract patient info and parse into subparts - 80 characters
    // fields are separated by spaces and fields do not contain spaces
    EDFPatient patient;
    string patientInfo = rootHeader.substr(headerLoc, 80);
    headerLoc += 80;
    size_t fieldLength = 0;
    size_t fieldStart = 0;
    // code
    if ((fieldLength = patientInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
        patient.setCode(patientInfo.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // gender
    if ((fieldLength = patientInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
        string t = patientInfo.substr(fieldStart, fieldLength);
        if (patientInfo.substr(fieldStart, fieldLength).compare("F") == 0)
            patient.setGender(EDFPatient::FEMALE);
        else if (patientInfo.substr(fieldStart, fieldLength).compare("M") == 0)
            patient.setGender(EDFPatient::MALE);
        else
            patient.setGender(EDFPatient::UNKNOWN);
        fieldStart += fieldLength + 1;
    }
    
    // birthdate
    if ((fieldLength = patientInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
        patient.setBirthdate(patientInfo.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // name
    if ((fieldLength = patientInfo.find(' ', fieldStart) - fieldStart) != string::npos)
        patient.setName(patientInfo.substr(fieldStart, fieldLength));

    header->setPatient(patient);
    
    // extract recording info - 80 characters
    // parse this later once we know if this in an EDF or EDF+ type file
    string recordingInfo = rootHeader.substr(headerLoc, 80);
    headerLoc += 80;

    // extract another start date (why are two start dates needed?) - 8 characters
    header->setDate(EDFDate(rootHeader.substr(headerLoc, 8)));
    headerLoc += 8;

    // extract start time - 8 characters
    header->setStartTime(EDFTime(rootHeader.substr(headerLoc, 8)));
    headerLoc += 8;

    // extract header record size - 8 characters
    int recordSize = a2i(rootHeader.substr(headerLoc, 8));
    headerLoc += 8;

    // extract file type - ? characters
    fieldLength = rootHeader.find(' ', headerLoc) - headerLoc;
    string fileType = rootHeader.substr(headerLoc, fieldLength);
    //headerLoc += fileType.length();
    if (fileType.compare("EDF+C") == 0) {
        header->setContinuity(EDFHeader::CONTINUOUS);
        header->setFiletype(EDFHeader::EDFPLUS);
    } else if (fileType.compare("EDF+D") == 0) {
        header->setContinuity(EDFHeader::DISCONTINUOUS);
        header->setFiletype(EDFHeader::EDFPLUS);
    } else {
        header->setContinuity(EDFHeader::CONTINUOUS);
        header->setFiletype(EDFHeader::EDF);
    }
    
    // now deal with the recording info
    fieldStart = 0;
    if (header->getFiletype() == EDFHeader::EDFPLUS) {
        // subheader
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
            string recordingStart = recordingInfo.substr(fieldStart, fieldLength);
            if (recordingStart.compare("Startdate") != 0)
                cerr << "\"Startdate\" text not found in recording information header. Ignoring..." << endl;
            else
                fieldStart += fieldLength + 1;
        }
        
        // start date
        // harder to parse than other version that should be the same
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
            //header.setDate(recordingInfo.substr(fieldStart, spaceLoc));
            fieldStart += fieldLength + 1;
        }
        
        // admin code
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
            header->setAdminCode(recordingInfo.substr(fieldStart, fieldLength));
            fieldStart += fieldLength + 1;
        }
        
        // technician
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos) {
            header->setTechnician(recordingInfo.substr(fieldStart, fieldLength));
            fieldStart += fieldLength + 1;
        }
        
        // equipment
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos)
            header->setEquipment(recordingInfo.substr(fieldStart, fieldLength));
        
    } else { // EDF file
        if ((fieldLength = recordingInfo.find(' ', fieldStart) - fieldStart) != string::npos)
            header->setRecording(recordingInfo.substr(fieldStart, fieldLength));
    }
        

    // skip remainder reserved space
    headerLoc += 44;

    // extract number of data records - 8 characters
    header->setDataRecordCount(a2i(rootHeader.substr(headerLoc, 8)));
    headerLoc += 8;
    
    // extract duration of data records - 8 characters
    double dataDuration = a2f(rootHeader.substr(headerLoc, 8));
    headerLoc += 8;
    header->setDataRecordDuration(dataDuration);

    // extract number of signals - 4 characters
    int signalCount = a2i(rootHeader.substr(headerLoc, 4));
    header->setSignalCount(signalCount);


    // read signal data characters 257 -> signal count * 256
    int signalHeaderLength = signalCount * 256 + 1;
    char* signalHeaderArray = new char[signalHeaderLength];
    if (in.fail() || !in.get(signalHeaderArray, signalHeaderLength)) {
        cerr << "Error reading header from file. Giving up..." << endl;
        return NULL;
    }

    // verify that header data characters are in valid range
    for (int i = 0; i < signalHeaderLength - 1; i++)
        if (signalHeaderArray[i] < 32 || signalHeaderArray[i] > 126) {
            delete [] signalHeaderArray;
            cerr << "Non-ASCII characters detected in header" << endl;
            return NULL;
        }
    
    string signalHeader(signalHeaderArray);
    delete [] signalHeaderArray;
    headerLoc = 0;

    // there can only be one annotation channel. if more than one is found then
    // throw an error based on the index
    int annotationIndex = -1;

    // extract signal labels 16 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setLabel(sigNum, trim(signalHeader.substr(headerLoc, 16)));
        if (header->getLabel(sigNum).compare("EDF Annotations") == 0) {
            if (annotationIndex != -1)
                cerr << "More than one annotation signals defined. Only the first is accessible..." << endl;
            else {
                header->setAnnotationIndex(sigNum);
                if (header->getFiletype() == EDFHeader::EDF)
                    cerr << "Annotations not expected in EDF file. Handling them anyway..." << endl;
            }
        }
        headerLoc += 16;
    }

    // extract transducer types 80 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setTransducer(sigNum, trim(signalHeader.substr(headerLoc, 80)));
        headerLoc += 80;
    }

    // extract physical dimensions 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setPhysicalDimension(sigNum, trim(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }

    // extract physical minimums 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setPhysicalMin(sigNum, a2f(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }
    
    // extract physical maximums 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setPhysicalMax(sigNum, a2f(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }
    
    // extract physical minimums 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setDigitalMin(sigNum, a2i(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }
    
    // extract physical maximums 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setDigitalMax(sigNum, a2i(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }

    // extract prefilters 80 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setPrefilter(sigNum, trim(signalHeader.substr(headerLoc, 80)));
        headerLoc += 80;
    }

    int dataRecordSize = 0;
    int tempOffset = 0;
    // extract the number of samples in each record 8 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setSignalSampleCount(sigNum, a2i(signalHeader.substr(headerLoc, 8)));
        int count = header->getSignalSampleCount(sigNum);
        dataRecordSize += count;
        headerLoc += 8;

        header->setBufferOffset(sigNum, tempOffset);
        // each data point is 2 bytes
        tempOffset += header->getSignalSampleCount(sigNum) * 2;
    }
    header->setDataRecordSize(dataRecordSize * 2);

    // extract reserved field 32 chars at a time
    for (int sigNum = 0; sigNum < signalCount; sigNum++) {
        header->setReserved(sigNum, signalHeader.substr(headerLoc, 32));
        headerLoc += 32;
    }

    // jump to end of file and make sure all data is present and there is no extra data
    in.seekg(0, std::ios::end);
    int last = in.tellg();
    if (last - header->getDataRecordCount() * header->getDataRecordSize() - recordSize < 0) {
        cerr << "Data segement has too much information. Signal data will not be accessible..." << endl;
        return NULL;
    } else if (last - header->getDataRecordCount() * header->getDataRecordSize() - recordSize > 0) {
        cerr << "Data segement has missing information. Signal data will not be accessible..." << endl;
        return NULL;
    }

    return header;
}

vector<EDFAnnotation>* EDFParser::parseAnnotation(std::fstream& in, EDFHeader* header) {
    // tal = time-stamped annotations list
    
    vector<EDFAnnotation>* annotations = new vector<EDFAnnotation>();
    int sig = header->getAnnotationIndex();

    // seek to beginning of data records
    in.seekg(header->getSignalCount() * 256 + 256, std::ios::beg);
    
    char* record = new char[header->getDataRecordSize()];
    int talStart = header->getBufferOffset(sig);
    int talEnd = talStart + header->getSignalSampleCount(sig) * 2;
    char* tal = new char[talEnd - talStart];
    
    for (int recordNum = 0; recordNum < header->getDataRecordCount(); recordNum++) {
        string onset, duration;
        vector<string> annotationStrings;
        int talOffset = 0;

        if(!in.read(record, header->getDataRecordSize())) {
            cerr << "Error reading annotations from file. Giving up..." << endl;
            return NULL;
        }

        // and extract TAL section
        for (int i = 0; i < talEnd - talStart; i++)
            tal[i] = record[talStart + i];

        while (talOffset < talEnd - talStart) {
            // find length of onset by checking for 20 or 21
            int onsetLength = 0;
            while (tal[talOffset + onsetLength] != 20 &&
                   tal[talOffset + onsetLength] != 21 &&
                   talOffset + onsetLength < talEnd - talStart) // check if beyond tal length
                onsetLength++;
            
            if (talOffset + onsetLength < talEnd - talStart) {
                // verify onset character set
                onset = string(tal, talOffset, onsetLength);
                talOffset += onsetLength;
                if (!validOnset(onset)) {
                    cerr << "TAL onset has bad format. Skipping..." << endl;
                    // TODO: need to write proper code to skip this TAL
                }
                
                if (tal[talOffset] == 21) { // duration string expected
                    talOffset++; // skip 21 character
                    int durationLength = 0;
                    // find length of duration by checking for 20
                    while (tal[talOffset + durationLength] != 20)
                        durationLength++;
                    
                    // verify duration character set
                    duration = string(tal, talOffset, durationLength);
                    talOffset += durationLength;
                    if (!validDuration(duration)) {
                        cerr << "TAL duration has bad format. Skipping..." << endl;
                        // TODO: need to write proper code to skip this TAL
                    }
                    
                } else { // skip duration and extract the annotation
                    talOffset++; // skip 20 character
                    int annotLength = 0;
                    // find length of annotation strings by searching for 20 0
                    while (tal[talOffset + annotLength] != 20 && tal[talOffset + annotLength + 1] != 0)
                        annotLength++;
                    
                    string s(tal, talOffset, annotLength);
                    talOffset += annotLength;
                    tal[talOffset + 1] = 20; // this is a hack because the string constructor will not read past a zero for indexing (ref line 309)
                    talOffset += 2; // skip 20 and 0 character
                    if (s.size() > 0)
                        annotationStrings.push_back(trim(s));
                }
            } else
                talOffset += onsetLength;
        }
        
        if (annotationStrings.size() > 0)
            annotations->push_back(EDFAnnotation(a2f(onset), a2f(duration), annotationStrings));

    }

    delete [] tal;
    delete [] record;
    
    return annotations;
}

EDFSignalData* EDFParser::parseSignal(std::fstream& in, EDFHeader* header, int signal) {
    return parseSignal(in, header, signal, 0, header->getTotalRecordingTime());
}

EDFSignalData* EDFParser::parseSignal(std::fstream& in, EDFHeader* header, int signal, double startTime, double length) {
    // you should check that the signal value is in range before calling this method
    if (startTime < 0 || startTime > header->getTotalRecordingTime()) {
        cerr << "Signal start time out of range. Giving up..." << endl;
        return NULL;
    }
    
    double freq = header->getSignalSampleCount(signal) / header->getDataRecordDuration();
    EDFSignalData* data = new EDFSignalData(freq, header->getPhysicalMax(signal), header->getPhysicalMin(signal));
    
    // seek to beginning of data records
    //in.seekg(header.getSignalCount() * 256 + 256, std::ios::beg);
    
    char* record = new char[header->getDataRecordSize()]; // each record is the same size
    int startInRecordBuffer = header->getBufferOffset(signal); // this is the byte where the channel starts in each record
    int endInRecordBuffer = startInRecordBuffer + header->getSignalSampleCount(signal) * 2; // this is the byte where the channel ends in each record
    int16_t* convertedSignal;
    int convertedSignalLength;
    
    // convert startTime in seconds to a starting record location
    int startRecord = floor(startTime / header->getDataRecordDuration()); // as record index
    // convert length in seconds to an ending record location
    int endRecord = startRecord + ceil(length / header->getDataRecordDuration()); // as record index
    // truncate if beyond length of file
    if (endRecord > header->getDataRecordCount())
        endRecord = header->getDataRecordCount();

    // seek to beginning of first record to read
    in.seekg(header->getSignalCount() * 256 + 256 + header->getDataRecordSize() * startRecord);
    
    int numberOfSamples = static_cast<int>(floor((startTime + length) * freq));
    for (int recordNum = startRecord; recordNum < endRecord; recordNum++) {
        if(!in.read(record, header->getDataRecordSize())) {
            cerr << "Error reading signal records from file. Giving up..." << endl;
            return NULL;
        }

        // reading possibly partial first record
        if (recordNum == startRecord) {
            // convert from 2's comp to integers
            int start = static_cast<int>(floor(startTime * freq)) %
                header->getSignalSampleCount(signal); // in samples
            int end = std::min((endInRecordBuffer - startInRecordBuffer) / 2, numberOfSamples); // in samples
            convertedSignalLength = end - start;
            convertedSignal = new int16_t[convertedSignalLength];
            for (int i = start; i < end; i++)
                convertedSignal[i - start] = record[startInRecordBuffer + 2 * i] + 256 * record[startInRecordBuffer + 2 * i + 1];
            
        // read the rest of the records from 0 to end
        } else {
            // convert from 2's comp to integers
            int end = std::min((endInRecordBuffer - startInRecordBuffer) / 2, numberOfSamples); // in bytes
            convertedSignalLength = end;
            convertedSignal = new int16_t[convertedSignalLength];
            for (int i = 0; i < end; i++)
                convertedSignal[i] = record[startInRecordBuffer + 2 * i] + 256 * record[startInRecordBuffer + 2 * i + 1];
        }
        
        data->addElements(convertedSignal, convertedSignalLength);
        numberOfSamples -= header->getSignalSampleCount(signal); // trim for 'end' calculations
        
        delete [] convertedSignal;
    }
    
    delete [] record;
    
    return data;
}

bool EDFParser::validOnset(string& s) {
    // onset must start with '+' or '-'
    if (s[0] != '+' && s[0] != '-')
        return false;
    
    // can contain at most one '.'
    // and only numerals after first dot character
    int dotIndex = -1;
    for (size_t i = 1; i < s.length(); i++) {
        if (s[i] == '.' && dotIndex < 0) {// is it a dot
            dotIndex = i;
            if (dotIndex == (int)s.length() - 1)
                return false; // cannot specify the dot without the fractional second part i.e. "+1." is bad
        } else if (s[i] < '0' || s[i] > '9') // not a numeral
            return false;
    }
    
    return true;
}

bool EDFParser::validDuration(string& s) {
    // can contain at most one '.'
    // and only numerals after first dot character
    int dotIndex = -1;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '.' && dotIndex < 0) // is it a dot
            dotIndex = i;
        else if (s[i] < '0' || s[i] > '9') // not a numeral
            return false;
    }
    
    return true;
}