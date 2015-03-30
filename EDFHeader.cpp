/**
 @file EDFHeader.cpp
 @author Anthony Magee
 @date 11/4/2010
*/

#include "EDFHeader.h"
#include "EDFUtil.h"
#include <algorithm>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;

EDFHeader::EDFHeader()
    : h_filetype(FileType::EDF)
    , h_continuity(Continuity::CONTINUOUS)
    , h_signalCount(0)
    , h_date(EDFDate())
    , h_startTime(EDFTime())
    , h_patient(EDFPatient())
    , h_recording("")
    , h_adminCode("")
    , h_technician("")
    , h_equipment("")
    , h_dataRecordDuration(0)
    , h_dataRecordCount(0)
    , h_dataRecordSize(0)
    , h_annotationIndex(-1)
    , h_label(nullptr)
    , h_physicalMax(nullptr)
    , h_physicalMin(nullptr)
    , h_digitalMax(nullptr)
    , h_digitalMin(nullptr)
    , h_signalSampleCount(nullptr)
    , h_physicalDimension(nullptr)
    , h_prefilter(nullptr)
    , h_transducer(nullptr)
    , h_reserved(nullptr)
    , h_bufferOffset(nullptr)
{}



EDFHeader::EDFHeader(const EDFHeader& orig) {
    h_filetype = orig.h_filetype;
    h_continuity = orig.h_continuity;
    h_signalCount = orig.h_signalCount;
    h_date = orig.h_date;
    h_startTime = orig.h_startTime;
    h_patient = orig.h_patient;
    h_recording = orig.h_recording;
    h_recordingAdditional = orig.h_recordingAdditional;
    h_adminCode = orig.h_adminCode;
    h_technician = orig.h_technician;
    h_equipment = orig.h_equipment;
    h_dataRecordDuration = orig.h_dataRecordDuration;
    h_dataRecordCount = orig.h_dataRecordCount;
    h_dataRecordSize = orig.h_dataRecordSize;
    h_annotationIndex = orig.h_annotationIndex;

    int sig;
    h_label = new string[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_label[sig] = orig.h_label[sig];

    h_physicalMax = new double[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_physicalMax[sig] = orig.h_physicalMax[sig];

    h_physicalMin = new double[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_physicalMin[sig] = orig.h_physicalMin[sig];

    h_digitalMax = new int[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_digitalMax[sig] = orig.h_digitalMax[sig];

    h_digitalMin = new int[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_digitalMin[sig] = orig.h_digitalMin[sig];

    h_signalSampleCount = new int[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_signalSampleCount[sig] = orig.h_signalSampleCount[sig];

    h_physicalDimension = new string[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_physicalDimension[sig] = orig.h_physicalDimension[sig];

    h_prefilter = new string[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_prefilter[sig] = orig.h_prefilter[sig];

    h_transducer = new string[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_transducer[sig] = orig.h_transducer[sig];

    h_reserved = new string[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_reserved[sig] = orig.h_reserved[sig];

    h_bufferOffset = new int[h_signalCount];
    for (sig = 0; sig < h_signalCount; sig++) h_bufferOffset[sig] = orig.h_bufferOffset[sig];
}

EDFHeader::~EDFHeader() {
    delete [] h_label;
    delete [] h_physicalMax;
    delete [] h_physicalMin;
    delete [] h_digitalMax;
    delete [] h_digitalMin;
    delete [] h_signalSampleCount;
    delete [] h_physicalDimension;
    delete [] h_prefilter;
    delete [] h_transducer;
    delete [] h_reserved;
    delete [] h_bufferOffset;
}

EDFHeader& EDFHeader::operator=(const EDFHeader& rhs) {
    if (this != &rhs) {
        h_filetype = rhs.h_filetype;
        h_continuity = rhs.h_continuity;
        h_signalCount = rhs.h_signalCount;
        h_date = rhs.h_date;
        h_startTime = rhs.h_startTime;
        h_patient = rhs.h_patient;
        h_recording = rhs.h_recording;
        h_recordingAdditional = rhs.h_recordingAdditional;
        h_adminCode = rhs.h_adminCode;
        h_technician = rhs.h_technician;
        h_equipment = rhs.h_equipment;
        h_dataRecordDuration = rhs.h_dataRecordDuration;
        h_dataRecordCount = rhs.h_dataRecordCount;
        h_dataRecordSize = rhs.h_dataRecordSize;
        h_annotationIndex = rhs.h_annotationIndex;

        int sig;
        if (h_label != nullptr) delete [] h_label;
        h_label = new string[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_label[sig] = rhs.h_label[sig];

        if (h_physicalMax != nullptr) delete [] h_physicalMax;
        h_physicalMax = new double[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_physicalMax[sig] = rhs.h_physicalMax[sig];

        if (h_physicalMin != nullptr) delete [] h_physicalMin;
        h_physicalMin = new double[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_physicalMin[sig] = rhs.h_physicalMin[sig];

        if (h_digitalMax != nullptr) delete [] h_digitalMax;
        h_digitalMax = new int[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_digitalMax[sig] = rhs.h_digitalMax[sig];

        if (h_digitalMin != nullptr) delete [] h_digitalMin;
        h_digitalMin = new int[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_digitalMin[sig] = rhs.h_digitalMin[sig];

        if (h_signalSampleCount != nullptr) delete [] h_signalSampleCount;
        h_signalSampleCount = new int[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_signalSampleCount[sig] = rhs.h_signalSampleCount[sig];

        if (h_physicalDimension != nullptr) delete [] h_physicalDimension;
        h_physicalDimension = new string[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_physicalDimension[sig] = rhs.h_physicalDimension[sig];

        if (h_prefilter != nullptr) delete [] h_prefilter;
        h_prefilter = new string[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_prefilter[sig] = rhs.h_prefilter[sig];

        if (h_transducer != nullptr) delete [] h_transducer;
        h_transducer = new string[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_transducer[sig] = rhs.h_transducer[sig];

        if (h_reserved != nullptr) delete [] h_reserved;
        h_reserved = new string[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_reserved[sig] = rhs.h_reserved[sig];

        if (h_bufferOffset != nullptr) delete [] h_bufferOffset;
        h_bufferOffset = new int[h_signalCount];
        for (sig = 0; sig < h_signalCount; sig++) h_bufferOffset[sig] = rhs.h_bufferOffset[sig];
    }
    return *this;
}

void EDFHeader::setFiletype(FileType filetype) {
    this->h_filetype = filetype;
}

void EDFHeader::setContinuity(Continuity continuity) {
    this->h_continuity = continuity;
}

void EDFHeader::setSignalCount(int h_signalCount) {
    this->h_signalCount = h_signalCount;

    if (h_label != nullptr) delete [] h_label;
    h_label = new string[h_signalCount];

    if (h_physicalMax != nullptr) delete [] h_physicalMax;
    h_physicalMax = new double[h_signalCount];

    if (h_physicalMin != nullptr) delete [] h_physicalMin;
    h_physicalMin = new double[h_signalCount];

    if (h_digitalMax != nullptr) delete [] h_digitalMax;
    h_digitalMax = new int[h_signalCount];

    if (h_digitalMin != nullptr) delete [] h_digitalMin;
    h_digitalMin = new int[h_signalCount];

    if (h_signalSampleCount != nullptr) delete [] h_signalSampleCount;
    h_signalSampleCount = new int[h_signalCount];

    if (h_physicalDimension != nullptr) delete [] h_physicalDimension;
    h_physicalDimension = new string[h_signalCount];

    if (h_prefilter != nullptr) delete [] h_prefilter;
    h_prefilter = new string[h_signalCount];

    if (h_transducer != nullptr) delete [] h_transducer;
    h_transducer = new string[h_signalCount];

    if (h_reserved != nullptr) delete [] h_reserved;
    h_reserved = new string[h_signalCount];

    if (h_bufferOffset != nullptr) delete [] h_bufferOffset;
    h_bufferOffset = new int[h_signalCount];
}

void EDFHeader::setDate(EDFDate date) {
    this->h_date = date;
}

void EDFHeader::setStartTime(EDFTime startTime) {
    this->h_startTime = startTime;
}

void EDFHeader::setPatient(EDFPatient patient) {
    this->h_patient = patient;
}

void EDFHeader::setRecording(string recording) {
    this->h_recording = recording;
}

void EDFHeader::setRecordingAdditional(string newRecordingAdditional) {
    this->h_recordingAdditional = newRecordingAdditional;
}

void EDFHeader::setAdminCode(string adminCode) {
    this->h_adminCode = adminCode;
}

void EDFHeader::setTechnician(string technician) {
    this->h_technician = technician;
}

void EDFHeader::setEquipment(string equipment) {
    this->h_equipment = equipment;
}

void EDFHeader::setDataRecordDuration(double dataRecordDuration) {
    this->h_dataRecordDuration = dataRecordDuration;
}

void EDFHeader::setDataRecordCount(int dataRecordCount) {
    this->h_dataRecordCount = dataRecordCount;
}

void EDFHeader::setDataRecordSize(int dataRecordSize) {
    this->h_dataRecordSize = dataRecordSize;
}

void EDFHeader::setAnnotationIndex(int annotationIndex) {
    if (!signalAvailable(annotationIndex))
        cerr << "Channel " << annotationIndex << " does not exist." << endl;

    this->h_annotationIndex = annotationIndex;
}

void EDFHeader::setLabel(int sigNum, string label) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_label[sigNum] = label;
}

void EDFHeader::setPhysicalMax(int sigNum, double physicalMax) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_physicalMax[sigNum] = physicalMax;
}

void EDFHeader::setPhysicalMin(int sigNum, double physicalMin) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_physicalMin[sigNum] = physicalMin;
}

void EDFHeader::setDigitalMax(int sigNum, int digitalMax) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_digitalMax[sigNum] = digitalMax;
}

void EDFHeader::setDigitalMin(int sigNum, int digitalMin) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_digitalMin[sigNum] = digitalMin;
}

void EDFHeader::setSignalSampleCount(int sigNum, int signalSampleCount) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_signalSampleCount[sigNum] = signalSampleCount;
}

void EDFHeader::setPhysicalDimension(int sigNum, string physicalDimension) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_physicalDimension[sigNum] = physicalDimension;
}

void EDFHeader::setPrefilter(int sigNum, string prefilter) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_prefilter[sigNum] = prefilter;
}

void EDFHeader::setTransducer(int sigNum, string transducer) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_transducer[sigNum] = transducer;
}

void EDFHeader::setReserved(int sigNum, string reserved) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_reserved[sigNum] = reserved;
}

void EDFHeader::setBufferOffset(int sigNum, int bufferOffset) {
    if (!signalAvailable(sigNum))
        cerr << "Channel " << sigNum << " does not exist." << endl;

    this->h_bufferOffset[sigNum] = bufferOffset;
}

FileType EDFHeader::filetype() const { return h_filetype; }

Continuity EDFHeader::continuity() const { return h_continuity; }

int EDFHeader::signalCount() const { return h_signalCount; }

EDFDate EDFHeader::date() const { return h_date; }

EDFTime EDFHeader::startTime() const {return h_startTime; }

EDFPatient EDFHeader::patient() const { return h_patient; }

string EDFHeader::recording() const { return h_recording; }

string EDFHeader::recordingAdditional() const { return h_recordingAdditional; }

string EDFHeader::adminCode() const { return h_adminCode; }

string EDFHeader::technician() const { return h_technician; }

string EDFHeader::equipment() const { return h_equipment; }

double EDFHeader::dataRecordDuration() const { return h_dataRecordDuration; }

int EDFHeader::dataRecordCount() const { return h_dataRecordCount; }

int EDFHeader::dataRecordSize() const { return h_dataRecordSize; }

int EDFHeader::annotationIndex() const { return h_annotationIndex; }

string EDFHeader::label(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_label[sigNum];
    else
        return "!!!";
}

double EDFHeader::physicalMax(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_physicalMax[sigNum];
    else
        return 0;
}

double EDFHeader::physicalMin(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_physicalMin[sigNum];
    else
        return 0;
}

int EDFHeader::digitalMax(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_digitalMax[sigNum];
    else
        return 0;
}

int EDFHeader::digitalMin(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_digitalMin[sigNum];
    else
        return 0;
}

int EDFHeader::signalSampleCount(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_signalSampleCount[sigNum];
    else
        return -1;
}

string EDFHeader::physicalDimension(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_physicalDimension[sigNum];
    else
        return "!!!";
}

string EDFHeader::prefilter(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_prefilter[sigNum];
    else
        return "!!!";
}

string EDFHeader::transducer(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_transducer[sigNum];
    else
        return "!!!";
}

string EDFHeader::reserved(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_reserved[sigNum];
    else
        return "!!!";
}

int EDFHeader::bufferOffset(int sigNum) const {
    if (signalAvailable(sigNum))
        return h_bufferOffset[sigNum];
    else
        return -1;
}

bool EDFHeader::hasAnnotations() const {
    return h_annotationIndex > -1;
}

double EDFHeader::recordingTime() const {
    return h_dataRecordCount * h_dataRecordDuration;
}

bool EDFHeader::signalAvailable(int sigNum) const {
    if (sigNum >= 0 && sigNum < h_signalCount)
        return true;
    else
        return false;
}
