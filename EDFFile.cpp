/** 
 @file EDFFile.cpp
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFFile.h"
#include "EDFUtil.h"
#include <iostream>
#include <cmath>

using std::fstream;
using std::string;
using std::vector;
using std::cerr;
using std::endl;

/* Parsing operations prototypes */
EDFHeader* parseHeader(std::fstream&);
std::vector<EDFAnnotation>* parseAnnotations(std::fstream&, EDFHeader*);
EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int);
EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int, double, double);

bool validOnset(string&);
bool validDuration(string&);
bool charactersValid(char*, int);

void parsePatientInfo(const string&, EDFHeader*);
void parseFileType(const string&, EDFHeader*);
void parseStdRecordInfo(const string&, EDFHeader*);
void parsePlusRecordInfo(const string&, EDFHeader*);
bool parseSignalHeaders(std::fstream&, EDFHeader*);
bool validFileLength(std::fstream&, EDFHeader*, int);
string parseOnset(char* const &, int&, int);
string parseDuration(char* const &, int&, int);
string parseAnnotation(char* const &, int&);

/* EDFFile class */

EDFFile::EDFFile(const char* path)
    : fileHeader(nullptr)
    , annotation(nullptr)
{
    filePath = string(path);
    fileStream.open(path, std::ios::in | std::ios::binary);
    if (!fileStream.is_open() || fileStream.fail())
        cerr << "EDFFile: File '" << filePath << "' does not exist or cannot be read." << endl;
    
    fileHeader = parseHeader(fileStream);
    if (fileHeader != nullptr && fileHeader->hasAnnotations())
        annotation = parseAnnotations(fileStream, fileHeader);
}

EDFFile::~EDFFile() {
    delete annotation;
    delete fileHeader;
    fileStream.close();
}

EDFHeader* EDFFile::header() const { return fileHeader; }

vector<EDFAnnotation>* EDFFile::annotations() const { return annotation; }

EDFSignalData* EDFFile::extractSignalData(int channel, double start, double length) {
    if (channel == fileHeader->annotationIndex() || !fileHeader->signalAvailable(channel))
        return nullptr;

    // fix length arg if it goes beyond end of recording length
    if (start + length > fileHeader->recordingTime())
        length = fileHeader->recordingTime() - start;

    return parseSignal(fileStream, fileHeader, channel, start, length);
}


/* Parsing operations */

EDFHeader* parseHeader(std::fstream &in) {
    EDFHeader* header = new EDFHeader();
    
    // rewind file
    in.seekg(0, std::ios::beg);
    // read first 256 characters of file
    char rootHeaderArray[257] = {0};
    if (in.fail() || !in.get(rootHeaderArray, 257)) {
        cerr << "Error trying to read header from file. Giving up..." << endl;
        return nullptr;
    }
    
    // verify that header data characters are in valid range
    if (!charactersValid(rootHeaderArray, 256))
        return nullptr;
    
    string rootHeader(rootHeaderArray);
    
    int headerLoc = 0;
    string versionStr    = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string patientStr    = rootHeader.substr(headerLoc, 80);
    headerLoc += 80;
    
    string recordStr     = rootHeader.substr(headerLoc, 80);
    headerLoc += 80;
    
    string startDateStr  = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string startTimeStr  = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string recordSizeStr = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string reservedStr   = rootHeader.substr(headerLoc, 44);
    headerLoc += 44;
    
    string recordCntStr  = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string durationStr   = rootHeader.substr(headerLoc, 8);
    headerLoc += 8;
    
    string signalCntStr  = rootHeader.substr(headerLoc, 4);
    
    // extract file version
    if (versionStr.compare("0       ") != 0)
        cerr << "Magic number is wrong." << endl <<
        "All current versions of EDF specifications must" <<
        " start with '0       ' string. Ignoring..." << endl;
    
    parsePatientInfo(patientStr, header);
    
    // extract another start date
    header->setDate(EDFDate(startDateStr));
    
    // extract start time
    header->setStartTime(EDFTime(startTimeStr));
    
    // extract file type
    parseFileType(reservedStr, header);
    
    // now deal with the recording info
    (header->filetype() == FileType::EDFPLUS) ? parsePlusRecordInfo(recordStr, header) : parseStdRecordInfo(recordStr, header);
    
    // extract number of data records
    header->setDataRecordCount(atoi(recordCntStr.c_str()));
    
    // extract duration of data records
    header->setDataRecordDuration(atof(durationStr.c_str()));
    
    // extract number of signals
    int signalCount = atoi(signalCntStr.c_str());
    header->setSignalCount(signalCount);
    
    if (!parseSignalHeaders(in, header))
        return nullptr;
    
    
    if (!validFileLength(in, header, atoi(recordSizeStr.c_str())))
        return nullptr;
    
    return header;
}

vector<EDFAnnotation>* parseAnnotations(std::fstream& in, EDFHeader* header) {
    // tal = time-stamped annotations list
    int annSigIdx = header->annotationIndex();
    if (annSigIdx < 0)
        return nullptr;
    
    vector<EDFAnnotation>* annotations = new vector<EDFAnnotation>();
    
    // seek to beginning of data records
    in.seekg(header->signalCount() * 256 + 256, std::ios::beg);
    
    char* record = new char[header->dataRecordSize()];
    int talStart = header->bufferOffset(annSigIdx);
    int talEnd = talStart + header->signalSampleCount(annSigIdx) * 2;
    int talLength = talEnd - talStart;
    char* tal = new char[talLength];
    
    range_loop(recordNum, 0, header->dataRecordCount(), 1) {
        string onset, duration;
        vector<string> annotationStrings;
        int talOffset = 0;
        
        if(!in.read(record, header->dataRecordSize())) {
            cerr << "Error reading annotations from file. Giving up..." << endl;
            delete annotations;
            delete [] record;
            delete [] tal;
            return nullptr;
        }
        
        // and extract TAL section
        memcpy(tal, record + talStart, talLength);
        
        while (talOffset < talLength) {
            // find length of onset by checking for 20 or 21
            int onsetLength = 0;
            while (tal[talOffset + onsetLength] != 20 &&
                   tal[talOffset + onsetLength] != 21 &&
                   talOffset + onsetLength < talLength) // check if beyond tal length
                onsetLength++;
            
            if (talOffset + onsetLength < talLength) {
                onset = parseOnset(tal, talOffset, onsetLength);
                
                if (tal[talOffset] == 21) { // duration string expected
                    duration = parseDuration(tal, talOffset, onsetLength);
                    
                } else { // skip duration and extract the annotation
                    string ann = parseAnnotation(tal, talOffset);
                    if (ann.size() > 0) // avoid storing empty objects
                        annotationStrings.push_back(trim(ann));
                }
                
            } else {
                talOffset += onsetLength;
            }
        }
        
        if (annotationStrings.size() > 0) // avoid storing empty objects
            annotations->push_back(EDFAnnotation(atof(onset.c_str()), atof(duration.c_str()), annotationStrings));
        
    }
    
    delete [] tal;
    delete [] record;
    
    return annotations;
}

string parseOnset(char* const &tal, int &talOffset, int onsetLength) {
    // verify onset character set
    string onset = string(tal, talOffset, onsetLength);
    talOffset += onsetLength;
    if (!validOnset(onset)) {
        cerr << "TAL onset has bad format. Skipping..." << endl;
        // TODO: need to write code to skip this TAL
        // not sure if anything needs to happen
    }
    return onset;
}

string parseDuration(char* const &tal, int &talOffset, int onsetLength) {
    talOffset++; // skip 21 character
    int durationLength = 0;
    // find length of duration by checking for 20
    while (tal[talOffset + durationLength] != 20)
        durationLength++;
    
    // verify duration character set
    string duration = string(tal, talOffset, durationLength);
    talOffset += durationLength;
    if (!validDuration(duration)) {
        cerr << "TAL duration has bad format. Skipping..." << endl;
        // TODO: need to write code to skip this TAL
        // not sure if anything needs to happen
    }
    
    return duration;
}

string parseAnnotation(char* const &tal, int& talOffset) {
    talOffset++; // skip 20 character
    int annotLength = 0;
    // find length of annotation strings by searching for consecutive bytes {20, 0}
    while (tal[talOffset + annotLength] != 20 && tal[talOffset + annotLength + 1] != 0)
        annotLength++;
    
    string s(tal, talOffset, annotLength);
    talOffset += annotLength;
    tal[talOffset + 1] = 20; // this is needed because the string constructor will not read past a zero for indexing (ref line 309)
    talOffset += 2; // skip 20 and 0 character, we know there is a 0 character because the while loop above specifically checked for it
    
    return s;
}

EDFSignalData* parseSignal(std::fstream& in, EDFHeader* header, int signal) {
    return parseSignal(in, header, signal, 0, header->recordingTime());
}

EDFSignalData* parseSignal(std::fstream& in, EDFHeader* header, int signal, double startTime, double length) {
    // you should check that the signal value is in range before calling this method
    if (startTime < 0 || startTime > header->recordingTime()) {
        cerr << "Signal start time out of range. Giving up..." << endl;
        return nullptr;
    }
    
    double freq = header->signalSampleCount(signal) / header->dataRecordDuration();
    EDFSignalData* data = new EDFSignalData(freq, header->physicalMax(signal), header->physicalMin(signal));
    
    // seek to beginning of data records
    //in.seekg(header.signalCount() * 256 + 256, std::ios::beg);
    
    char* record = new char[header->dataRecordSize()]; // each record is the same size
    int startInRecordBuffer = header->bufferOffset(signal); // this is the byte where the channel starts in each record
    int endInRecordBuffer = startInRecordBuffer + header->signalSampleCount(signal) * 2; // this is the byte where the channel ends in each record
    double* convertedSignal;
    int convertedSignalLength;
    
    // convert startTime in seconds to a starting record location
    int startRecord = floor(startTime / header->dataRecordDuration()); // as record index
    // convert length in seconds to an ending record location
    int endRecord = startRecord + ceil(length / header->dataRecordDuration()); // as record index
    // truncate if beyond length of file
    if (endRecord > header->dataRecordCount())
        endRecord = header->dataRecordCount();
    
    // seek to beginning of first record to read
    in.seekg(header->signalCount() * 256 + 256 + header->dataRecordSize() * startRecord);
    
    int numberOfSamples = static_cast<int>(floor((startTime + length) * freq));
    range_loop(recordNum, startRecord, endRecord, 1) {
        if(!in.read(record, header->dataRecordSize())) {
            cerr << "Error reading signal records from file. Giving up..." << endl;
            delete data;
            return nullptr;
        }
        
        // reading, possibly partial, first record
        if (recordNum == startRecord) {
            // convert from 2's comp to integers
            int start = static_cast<int>(floor(startTime * freq)) % header->signalSampleCount(signal); // in samples
            int end = std::min((endInRecordBuffer - startInRecordBuffer) / 2, numberOfSamples); // in samples
            convertedSignalLength = end - start;
            convertedSignal = new double[convertedSignalLength];
            range_loop(i, start, end, 1)
                convertedSignal[i - start] = record[startInRecordBuffer + 2 * i] + 256 * record[startInRecordBuffer + 2 * i + 1];
            
            // read the rest of the records from 0 to end
        } else {
            // convert from 2's comp to integers
            int end = std::min((endInRecordBuffer - startInRecordBuffer) / 2, numberOfSamples); // in bytes
            convertedSignalLength = end;
            convertedSignal = new double[convertedSignalLength];
            range_loop(i, 0, end, 1)
                convertedSignal[i] = record[startInRecordBuffer + 2 * i] + 256 * record[startInRecordBuffer + 2 * i + 1];
        }
        
        data->addDataPoints(convertedSignal, convertedSignalLength);
        numberOfSamples -= header->signalSampleCount(signal); // trim for 'end' calculations
        
        delete [] convertedSignal;
    }
    
    delete [] record;
    
    return data;
}

void parsePatientInfo(const string &pStr, EDFHeader *header) {
    // extract patient info and parse into subparts - 80 characters
    // fields are separated by spaces and fields do not contain spaces
    
    EDFPatient patient;
    
    size_t fieldLength = 0;
    size_t fieldStart = 0;
    // code
    if ((fieldLength = pStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        patient.setCode(pStr.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // gender
    if ((fieldLength = pStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        string t = pStr.substr(fieldStart, fieldLength);
        string gender = pStr.substr(fieldStart, fieldLength);
        if (gender.compare("F") == 0)
            patient.setGender(Gender::FEMALE);
        else if (gender.compare("M") == 0)
            patient.setGender(Gender::MALE);
        else
            patient.setGender(Gender::UNKNOWN);
        fieldStart += fieldLength + 1;
    }
    
    // birthdate
    if ((fieldLength = pStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        patient.setBirthdate(pStr.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // name
    if ((fieldLength = pStr.find(' ', fieldStart) - fieldStart) != string::npos)
        patient.setName(pStr.substr(fieldStart, fieldLength));
    
    header->setPatient(patient);
}

void parseFileType(const string &rStr, EDFHeader *header) {
    string fileType = rStr.substr(0, rStr.find(' ', 0));
    if (fileType.compare("EDF+C") == 0) {
        header->setContinuity(Continuity::CONTINUOUS);
        header->setFiletype(FileType::EDFPLUS);
    } else if (fileType.compare("EDF+D") == 0) {
        header->setContinuity(Continuity::DISCONTINUOUS);
        header->setFiletype(FileType::EDFPLUS);
    } else {
        header->setContinuity(Continuity::CONTINUOUS);
        header->setFiletype(FileType::EDF);
    }
}

void parseStdRecordInfo(const string &recordStr, EDFHeader *header) {
    size_t fieldStart = 0;
    size_t fieldLength = 0;
    
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos)
        header->setRecording(recordStr.substr(fieldStart, fieldLength));
}

void parsePlusRecordInfo(const string &recordStr, EDFHeader *header) {
    size_t fieldStart = 0;
    size_t fieldLength = 0;
    
    // subheader
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        string recordingStart = recordStr.substr(fieldStart, fieldLength);
        if (recordingStart.compare("Startdate") != 0)
            cerr << "\"Startdate\" text not found in recording information header. Ignoring..." << endl;
        else
            fieldStart += fieldLength + 1;
    }
    
    // start date
    // harder to parse than other version that should be the same
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        //header.setDate(recordingInfo.substr(fieldStart, spaceLoc));
        fieldStart += fieldLength + 1;
    }
    
    // admin code
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        header->setAdminCode(recordStr.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // technician
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos) {
        header->setTechnician(recordStr.substr(fieldStart, fieldLength));
        fieldStart += fieldLength + 1;
    }
    
    // equipment
    if ((fieldLength = recordStr.find(' ', fieldStart) - fieldStart) != string::npos)
        header->setEquipment(recordStr.substr(fieldStart, fieldLength));
}

bool parseSignalHeaders(std::fstream &in, EDFHeader *header) {
    // read signal data characters 257 -> signal count * 256
    int signalHeaderLength = header->signalCount() * 256 + 1;
    char* signalHeaderArray = new char[signalHeaderLength];
    if (in.fail() || !in.get(signalHeaderArray, signalHeaderLength)) {
        cerr << "Error reading header from file. Giving up..." << endl;
        delete [] signalHeaderArray;
        return false;
    }
    
    // verify that header data characters are in valid range
    if (!charactersValid(signalHeaderArray, signalHeaderLength - 1)) {
        delete [] signalHeaderArray;
        return false;
    }
    
    string signalHeader(signalHeaderArray);
    delete [] signalHeaderArray;
    
    size_t headerLoc = 0;
    int signalCount = header->signalCount();
    
    // there can only be one annotation channel. if more than one is found then report an error
    int annotationIndex = -1;
    
    // extract signal labels 16 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setLabel(sigNum, trim(signalHeader.substr(headerLoc, 16)));
        if (header->label(sigNum).compare("EDF Annotations") == 0) {
            if (annotationIndex != -1) {
                cerr << "More than one annotation signals defined. Only the first is accessible..." << endl;
            } else {
                header->setAnnotationIndex(sigNum);
                if (header->filetype() == FileType::EDF)
                    cerr << "Annotations not expected in EDF file. Handling them anyway..." << endl;
            }
        }
        headerLoc += 16;
    }
    
    // extract transducer types 80 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setTransducer(sigNum, trim(signalHeader.substr(headerLoc, 80)));
        headerLoc += 80;
    }
    
    // extract physical dimensions 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setPhysicalDimension(sigNum, trim(signalHeader.substr(headerLoc, 8)));
        headerLoc += 8;
    }
    
    // extract physical minimums 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setPhysicalMin(sigNum, atof(signalHeader.substr(headerLoc, 8).c_str()));
        headerLoc += 8;
    }
    
    // extract physical maximums 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setPhysicalMax(sigNum, atof(signalHeader.substr(headerLoc, 8).c_str()));
        headerLoc += 8;
    }
    
    // extract physical minimums 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setDigitalMin(sigNum, atoi(signalHeader.substr(headerLoc, 8).c_str()));
        headerLoc += 8;
    }
    
    // extract physical maximums 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setDigitalMax(sigNum, atoi(signalHeader.substr(headerLoc, 8).c_str()));
        headerLoc += 8;
    }
    
    // extract prefilters 80 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setPrefilter(sigNum, trim(signalHeader.substr(headerLoc, 80)));
        headerLoc += 80;
    }
    
    int dataRecordSize = 0;
    int tempOffset = 0;
    // extract the number of samples in each record 8 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setSignalSampleCount(sigNum, atoi(signalHeader.substr(headerLoc, 8).c_str()));
        int count = header->signalSampleCount(sigNum);
        dataRecordSize += count;
        headerLoc += 8;
        
        header->setBufferOffset(sigNum, tempOffset);
        // each data point is 2 bytes
        tempOffset += header->signalSampleCount(sigNum) * 2;
    }
    header->setDataRecordSize(dataRecordSize * 2);
    
    // extract reserved field 32 chars at a time
    range_loop(sigNum, 0, signalCount, 1) {
        header->setReserved(sigNum, trim(signalHeader.substr(headerLoc, 32)));
        headerLoc += 32;
    }
    
    return true;
}

bool charactersValid(char *str, int len) {
    range_loop(i, 0, len, 1) {
        if (str[i] < 32 || str[i] > 126) {
            cerr << "Invalid characters detected in header" << endl;
            return false;
        }
    }
    
    return true;
}

bool validFileLength(std::fstream &in, EDFHeader* header, int recordSize) {
    // jump to end of file and make sure all data is present and there is no extra data
    in.seekg(0, std::ios::end);
    long long last = in.tellg();
    long long lengthDiff = last - header->dataRecordCount() * header->dataRecordSize() - recordSize;
    if (lengthDiff < 0) {
        cerr << "Data segement has excessive information. Signal data will not be accessible..." << endl;
        return false;
    } else if (lengthDiff > 0) {
        cerr << "Data segement has missing information. Signal data will not be accessible..." << endl;
        return false;
    }
    
    return true;
}

bool validOnset(string& s) {
    // onset must start with '+' or '-'
    if (s[0] != '+' && s[0] != '-')
        return false;
    
    // can contain at most one '.'
    // and only numerals after first dot character
    int dotIndex = -1;
    range_loop(i, 1, s.length(), 1) {
        if (s[i] == '.' && dotIndex < 0) {// is it a dot
            dotIndex = (int)i;
            if (dotIndex == (int)s.length() - 1)
                return false; // cannot specify the dot without the fractional second part i.e. "+1." is bad
        } else if (s[i] < '0' || s[i] > '9') // not a numeral
            return false;
    }
    
    return true;
}

bool validDuration(string& s) {
    // can contain at most one '.'
    // and only numerals after first dot character
    int dotIndex = -1;
    range_loop(i, 0, s.length(), 1) {
        if (s[i] == '.' && dotIndex < 0) // is it a dot
            dotIndex = (int)i;
        else if (s[i] < '0' || s[i] > '9') // not a numeral
            return false;
    }
    
    return true;
}
