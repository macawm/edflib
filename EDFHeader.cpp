/**
 @file EDFHeader.cpp
 @brief A model file for an EDF file's header.
 The preheader consists of 256 bytes of information about the file in general.
 For each signal (channel) there are another 256b bytes of information relating
 the qualities of the signal.

 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFHeader.h"
#include "EDFUtil.h"
#include <algorithm>
#include <iostream>

using std::cerr;
using std::endl;

EDFHeader::EDFHeader() {
    filetype = FileType::EDF;
    continuity = Continuity::CONTINUOUS;
    signalCount = 0;
    //fileDuration = 0;
    date = EDFDate();
    startTime = EDFTime();
    patient = EDFPatient();
    recording = "";
    recordingAdditional = "";
    adminCode = "";
    technician = "";
    equipment = "";
    dataRecordDuration = 0;
    dataRecordCount = 0;
    dataRecordSize = 0;
    annotationIndex = -1;
    
    label = nullptr;
    physicalMax = nullptr;
    physicalMin = nullptr;
    digitalMax = nullptr;
    digitalMin = nullptr;
    signalSampleCount = nullptr;
    physicalDimension = nullptr;
    prefilter = nullptr;
    transducer = nullptr;
    reserved = nullptr;
    bufferOffset = nullptr;
}

EDFHeader::EDFHeader(const EDFHeader& orig) {
    filetype = orig.filetype;
    continuity = orig.continuity;
    signalCount = orig.signalCount;
    //fileDuration = orig.fileDuration;
    date = orig.date;
    startTime = orig.startTime;
    patient = orig.patient;
    recording = orig.recording;
    recordingAdditional = orig.recordingAdditional;
    adminCode = orig.adminCode;
    technician = orig.technician;
    equipment = orig.equipment;
    dataRecordDuration = orig.dataRecordDuration;
    dataRecordCount = orig.dataRecordCount;
    dataRecordSize = orig.dataRecordSize;
    annotationIndex = orig.annotationIndex;

    int sig;
    label = new string[signalCount];
    for (sig = 0; sig < signalCount; sig++) label[sig] = orig.label[sig];

    physicalMax = new double[signalCount];
    for (sig = 0; sig < signalCount; sig++) physicalMax[sig] = orig.physicalMax[sig];

    physicalMin = new double[signalCount];
    for (sig = 0; sig < signalCount; sig++) physicalMin[sig] = orig.physicalMin[sig];

    digitalMax = new int[signalCount];
    for (sig = 0; sig < signalCount; sig++) digitalMax[sig] = orig.digitalMax[sig];

    digitalMin = new int[signalCount];
    for (sig = 0; sig < signalCount; sig++) digitalMin[sig] = orig.digitalMin[sig];

    signalSampleCount = new int[signalCount];
    for (sig = 0; sig < signalCount; sig++) signalSampleCount[sig] = orig.signalSampleCount[sig];

    physicalDimension = new string[signalCount];
    for (sig = 0; sig < signalCount; sig++) physicalDimension[sig] = orig.physicalDimension[sig];

    prefilter = new string[signalCount];
    for (sig = 0; sig < signalCount; sig++) prefilter[sig] = orig.prefilter[sig];

    transducer = new string[signalCount];
    for (sig = 0; sig < signalCount; sig++) transducer[sig] = orig.transducer[sig];

    reserved = new string[signalCount];
    for (sig = 0; sig < signalCount; sig++) reserved[sig] = orig.reserved[sig];

    bufferOffset = new int[signalCount];
    for (sig = 0; sig < signalCount; sig++) bufferOffset[sig] = orig.bufferOffset[sig];
}

EDFHeader::~EDFHeader() {
    delete [] label;
    delete [] physicalMax;
    delete [] physicalMin;
    delete [] digitalMax;
    delete [] digitalMin;
    delete [] signalSampleCount;
    delete [] physicalDimension;
    delete [] prefilter;
    delete [] transducer;
    delete [] reserved;
    delete [] bufferOffset;
}

EDFHeader& EDFHeader::operator=(const EDFHeader& rhs) {
    if (this != &rhs) {
        filetype = rhs.filetype;
        continuity = rhs.continuity;
        signalCount = rhs.signalCount;
        //fileDuration = rhs.fileDuration;
        date = rhs.date;
        startTime = rhs.startTime;
        patient = rhs.patient;
        recording = rhs.recording;
        recordingAdditional = rhs.recordingAdditional;
        adminCode = rhs.adminCode;
        technician = rhs.technician;
        equipment = rhs.equipment;
        dataRecordDuration = rhs.dataRecordDuration;
        dataRecordCount = rhs.dataRecordCount;
        dataRecordSize = rhs.dataRecordSize;
        annotationIndex = rhs.annotationIndex;

        int sig;
        if (label != nullptr) delete [] label;
        label = new string[signalCount];
        for (sig = 0; sig < signalCount; sig++) label[sig] = rhs.label[sig];

        if (physicalMax != nullptr) delete [] physicalMax;
        physicalMax = new double[signalCount];
        for (sig = 0; sig < signalCount; sig++) physicalMax[sig] = rhs.physicalMax[sig];

        if (physicalMin != nullptr) delete [] physicalMin;
        physicalMin = new double[signalCount];
        for (sig = 0; sig < signalCount; sig++) physicalMin[sig] = rhs.physicalMin[sig];

        if (digitalMax != nullptr) delete [] digitalMax;
        digitalMax = new int[signalCount];
        for (sig = 0; sig < signalCount; sig++) digitalMax[sig] = rhs.digitalMax[sig];

        if (digitalMin != nullptr) delete [] digitalMin;
        digitalMin = new int[signalCount];
        for (sig = 0; sig < signalCount; sig++) digitalMin[sig] = rhs.digitalMin[sig];

        if (signalSampleCount != nullptr) delete [] signalSampleCount;
        signalSampleCount = new int[signalCount];
        for (sig = 0; sig < signalCount; sig++) signalSampleCount[sig] = rhs.signalSampleCount[sig];

        if (physicalDimension != nullptr) delete [] physicalDimension;
        physicalDimension = new string[signalCount];
        for (sig = 0; sig < signalCount; sig++) physicalDimension[sig] = rhs.physicalDimension[sig];

        if (prefilter != nullptr) delete [] prefilter;
        prefilter = new string[signalCount];
        for (sig = 0; sig < signalCount; sig++) prefilter[sig] = rhs.prefilter[sig];

        if (transducer != nullptr) delete [] transducer;
        transducer = new string[signalCount];
        for (sig = 0; sig < signalCount; sig++) transducer[sig] = rhs.transducer[sig];

        if (reserved != nullptr) delete [] reserved;
        reserved = new string[signalCount];
        for (sig = 0; sig < signalCount; sig++) reserved[sig] = rhs.reserved[sig];

        if (bufferOffset != nullptr) delete [] bufferOffset;
        bufferOffset = new int[signalCount];
        for (sig = 0; sig < signalCount; sig++) bufferOffset[sig] = rhs.bufferOffset[sig];
    }
    return *this;
}

void EDFHeader::setFiletype(FileType filetype) {
    this->filetype = filetype;
}

void EDFHeader::setContinuity(Continuity continuity) {
    this->continuity = continuity;
}

void EDFHeader::setSignalCount(int signalCount) {
    this->signalCount = signalCount;

    if (label != nullptr) delete [] label;
    label = new string[signalCount];

    if (physicalMax != nullptr) delete [] physicalMax;
    physicalMax = new double[signalCount];

    if (physicalMin != nullptr) delete [] physicalMin;
    physicalMin = new double[signalCount];

    if (digitalMax != nullptr) delete [] digitalMax;
    digitalMax = new int[signalCount];

    if (digitalMin != nullptr) delete [] digitalMin;
    digitalMin = new int[signalCount];

    if (signalSampleCount != nullptr) delete [] signalSampleCount;
    signalSampleCount = new int[signalCount];

    if (physicalDimension != nullptr) delete [] physicalDimension;
    physicalDimension = new string[signalCount];

    if (prefilter != nullptr) delete [] prefilter;
    prefilter = new string[signalCount];

    if (transducer != nullptr) delete [] transducer;
    transducer = new string[signalCount];

    if (reserved != nullptr) delete [] reserved;
    reserved = new string[signalCount];

    if (bufferOffset != nullptr) delete [] bufferOffset;
    bufferOffset = new int[signalCount];
}

void EDFHeader::setDate(EDFDate date) {
    this->date = date;
}

void EDFHeader::setStartTime(EDFTime startTime) {
    this->startTime = startTime;
}

void EDFHeader::setPatient(EDFPatient patient) {
    this->patient = patient;
}

void EDFHeader::setRecording(string recording) {
    this->recording = recording;
}

void EDFHeader::setRecordingAdditional(string newRecordingAdditional) {
    this->recordingAdditional = newRecordingAdditional;
}

void EDFHeader::setAdminCode(string adminCode) {
    this->adminCode = adminCode;
}

void EDFHeader::setTechnician(string technician) {
    this->technician = technician;
}

void EDFHeader::setEquipment(string equipment) {
    this->equipment = equipment;
}

void EDFHeader::setDataRecordDuration(double dataRecordDuration) {
    this->dataRecordDuration = dataRecordDuration;
}

void EDFHeader::setDataRecordCount(int dataRecordCount) {
    this->dataRecordCount = dataRecordCount;
}

void EDFHeader::setDataRecordSize(int dataRecordSize) {
    this->dataRecordSize = dataRecordSize;
}

void EDFHeader::setAnnotationIndex(int annotationIndex) {
    if (!signalAvailable(annotationIndex))
        cerr << "Channel " << annotationIndex << " does not exist." << endl;

    this->annotationIndex = annotationIndex;
}

void EDFHeader::setLabel(int sigNum, string label) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->label[sigNum] = label;
}

void EDFHeader::setPhysicalMax(int sigNum, double physicalMax) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->physicalMax[sigNum] = physicalMax;
}

void EDFHeader::setPhysicalMin(int sigNum, double physicalMin) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->physicalMin[sigNum] = physicalMin;
}

void EDFHeader::setDigitalMax(int sigNum, int digitalMax) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->digitalMax[sigNum] = digitalMax;
}

void EDFHeader::setDigitalMin(int sigNum, int digitalMin) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->digitalMin[sigNum] = digitalMin;
}

void EDFHeader::setSignalSampleCount(int sigNum, int signalSampleCount) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->signalSampleCount[sigNum] = signalSampleCount;
}

void EDFHeader::setPhysicalDimension(int sigNum, string physicalDimension) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->physicalDimension[sigNum] = physicalDimension;
}

void EDFHeader::setPrefilter(int sigNum, string prefilter) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->prefilter[sigNum] = prefilter;
}

void EDFHeader::setTransducer(int sigNum, string transducer) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->transducer[sigNum] = transducer;
}

void EDFHeader::setReserved(int sigNum, string reserved) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->reserved[sigNum] = reserved;
}

void EDFHeader::setBufferOffset(int sigNum, int bufferOffset) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->bufferOffset[sigNum] = bufferOffset;
}

FileType EDFHeader::getFiletype() { return filetype; }

Continuity EDFHeader::getContinuity() { return continuity; }

int EDFHeader::getSignalCount() { return signalCount; }

EDFDate EDFHeader::getDate() { return date; }

EDFTime EDFHeader::getStartTime() {return startTime; }

EDFPatient EDFHeader::getPatient() { return patient; }

string EDFHeader::getRecording() { return recording; }

string EDFHeader::getRecordingAdditional() { return recordingAdditional; }

string EDFHeader::getAdminCode() { return adminCode; }

string EDFHeader::getTechnician() { return technician; }

string EDFHeader::getEquipment() { return equipment; }

double EDFHeader::getDataRecordDuration() { return dataRecordDuration; }

int EDFHeader::getDataRecordCount() { return dataRecordCount; }

int EDFHeader::getDataRecordSize() { return dataRecordSize; }

int EDFHeader::getAnnotationIndex() { return annotationIndex; }

string EDFHeader::getLabel(int sigNum) {
    if (signalAvailable(sigNum))
        return label[sigNum];
    else
        return "!!!";
}

double EDFHeader::getPhysicalMax(int sigNum) {
    if (signalAvailable(sigNum))
        return physicalMax[sigNum];
    else
        return 0;
}

double EDFHeader::getPhysicalMin(int sigNum) {
    if (signalAvailable(sigNum))
        return physicalMin[sigNum];
    else
        return 0;
}

int EDFHeader::getDigitalMax(int sigNum) {
    if (signalAvailable(sigNum))
        return digitalMax[sigNum];
    else
        return 0;
}

int EDFHeader::getDigitalMin(int sigNum) {
    if (signalAvailable(sigNum))
        return digitalMin[sigNum];
    else
        return 0;
}

int EDFHeader::getSignalSampleCount(int sigNum) {
    if (signalAvailable(sigNum))
        return signalSampleCount[sigNum];
    else
        return -1;
}

string EDFHeader::getPhysicalDimension(int sigNum) {
    if (signalAvailable(sigNum))
        return physicalDimension[sigNum];
    else
        return "!!!";
}

string EDFHeader::getPrefilter(int sigNum) {
    if (signalAvailable(sigNum))
        return prefilter[sigNum];
    else
        return "!!!";
}

string EDFHeader::getTransducer(int sigNum) {
    if (signalAvailable(sigNum))
        return transducer[sigNum];
    else
        return "!!!";
}

string EDFHeader::getReserved(int sigNum) {
    if (signalAvailable(sigNum))
        return reserved[sigNum];
    else
        return "!!!";
}

int EDFHeader::getBufferOffset(int sigNum) {
    if (signalAvailable(sigNum))
        return bufferOffset[sigNum];
    else
        return -1;
}

bool EDFHeader::hasAnnotations() {
    return annotationIndex > -1;
}

double EDFHeader::getRecordingTime() {
    return dataRecordCount * dataRecordDuration;
}

bool EDFHeader::signalAvailable(int sigNum) {
    if (sigNum >= 0 && sigNum < signalCount)
        return true;
    else
        return false;
}
