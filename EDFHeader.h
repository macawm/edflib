/**
 @file EDFHeader.h
 @brief A model object for an EDF file's header.
 The preheader consists of 256 bytes of information about the file in general.
 For each signal (channel) there are another 256b bytes of information relating
 the qualities of the signal.
 
 @author Anthony Magee
 @date 11/4/2010
 */

#ifndef _EDFHEADER_H
#define	_EDFHEADER_H

#include <string>
#include "EDFPatient.h"
#include "EDFTime.h"
#include "EDFDate.h"

enum class FileType { EDF, EDFPLUS };
enum class Continuity { CONTINUOUS, DISCONTINUOUS };

class EDFHeader {
public:
    EDFHeader();
    EDFHeader(const EDFHeader&);
    virtual ~EDFHeader();
    EDFHeader& operator=(const EDFHeader&);

    void setFiletype(FileType);
    void setContinuity(Continuity);
    void setSignalCount(int);
    void setDate(EDFDate);
    void setStartTime(EDFTime);
    void setPatient(EDFPatient);
    void setRecording(std::string);
    void setRecordingAdditional(std::string);
    void setAdminCode(std::string);
    void setTechnician(std::string);
    void setEquipment(std::string);
    void setDataRecordDuration(double);
    void setDataRecordCount(int);
    void setDataRecordSize(int);
    void setAnnotationIndex(int);

    void setLabel(int, std::string);
    void setPhysicalMax(int, double);
    void setPhysicalMin(int, double);
    void setDigitalMax(int, int);
    void setDigitalMin(int, int);
    void setSignalSampleCount(int, int);
    void setPhysicalDimension(int, std::string);
    void setPrefilter(int, std::string);
    void setTransducer(int, std::string);
    void setReserved(int, std::string);
    void setBufferOffset(int, int);

    FileType   filetype() const;
    Continuity continuity() const;
    int        signalCount() const;
    EDFDate    date() const;
    EDFTime    startTime() const;
    EDFPatient patient() const;
    std::string     recording() const;
    std::string     recordingAdditional() const;
    std::string     adminCode() const;
    std::string     technician() const;
    std::string     equipment() const;
    double     dataRecordDuration() const;
    int        dataRecordCount() const;
    int        dataRecordSize() const;
    int        annotationIndex() const;

    bool   signalAvailable(int) const;
    std::string label(int) const;
    double physicalMax(int) const;
    double physicalMin(int) const;
    int    digitalMax(int) const;
    int    digitalMin(int) const;
    int    signalSampleCount(int) const;
    std::string physicalDimension(int) const;
    std::string prefilter(int) const;
    std::string transducer(int) const;
    std::string reserved(int) const;
    int    bufferOffset(int) const;

    bool   hasAnnotations() const;
    double recordingTime() const;

private:
    FileType   h_filetype;
    Continuity h_continuity;
    int        h_signalCount;
    EDFDate    h_date;
    EDFTime    h_startTime;
    EDFPatient h_patient;
    std::string     h_recording, h_recordingAdditional, h_adminCode, h_technician;
    std::string     h_equipment;
    double     h_dataRecordDuration;
    int        h_dataRecordCount;
    int        h_dataRecordSize;
    int        h_annotationIndex;
    std::string*    h_label;
    double*    h_physicalMax;
    double*    h_physicalMin;
    int*       h_digitalMax;
    int*       h_digitalMin;
    int*       h_signalSampleCount;
    std::string*    h_physicalDimension;
    std::string*    h_prefilter;
    std::string*    h_transducer;
    std::string*    h_reserved;
    int*       h_bufferOffset;
};

#endif	/* _EDFHeader_H */

