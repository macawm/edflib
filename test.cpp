/* 
 * File:   test.cpp
 *
 * Created on November 5, 2010, 2:30 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "EDFLib.h"

using namespace std;

void printHeaderInfo(string filename, EDFHeader* head) {
    cout << "File \"" << filename << "\"" << endl << endl << endl;
    cout << "Header Info" << endl << endl;
    cout << "File Type            | ";
    if (head->getFiletype() == FileType::EDF)
        cout << "EDF" << endl;
    else
        cout << "EDF+" << (head->getContinuity() == Continuity::CONTINUOUS ? "C" : "D") << endl;
    cout << "Signal Count         | " << head->getSignalCount() << endl;
    cout << "Recording Length     | " << head->getDataRecordCount() * head->getDataRecordDuration() << " seconds" << endl;
    cout << "Start Date           | " << head->getDate() << endl;
    cout << "Start Time           | " << head->getStartTime() << endl;
    cout << "Recording            | " << head->getRecording() << endl;
    cout << "Patient Info ---------" << endl << head->getPatient() << endl;
    cout << "Admin Code           | " << head->getAdminCode() << endl;
    cout << "Technician           | " << head->getTechnician() << endl;
    cout << "Equipment            | " << head->getEquipment() << endl;
    cout << "Recording Additional | " << head->getRecordingAdditional() << endl;
    cout << "Record Duration      | " << head->getDataRecordDuration() << " seconds" << endl;
    cout << "Data Record Count    | " << head->getDataRecordCount() << endl;
    cout << "Data Record Size     | " << head->getDataRecordSize() << endl;
    cout << "Annotation Signal    | " << head->getAnnotationIndex() + 1 << endl;
    cout << endl << endl;
}

void printAnnotationInfo(vector<EDFAnnotation>* annot) {
    cout << fixed << setprecision(1);
    cout << "Annotation Info" << endl << endl;
    cout << "Annotations" << endl;
    
    int onsetWidth = 2;
    EDFAnnotation last = *(--annot->end());
    int l = last.getOnset();
    while (l > 0) {
        l /= 10;
        onsetWidth++;
    }
    
    int idxWidth = 0;
    l = annot->size();
    while (l > 0) {
        l /= 10;
        idxWidth++;
    }
    
    for (size_t i = 0; i < annot->size(); i++) {
        cout << " " << std::setw(idxWidth) << i+1;
        cout << " Onset " << std::setw(onsetWidth) << annot->at(i).getOnset() << "s";
        cout << " Duration " << std::setw(8) << annot->at(i).getDuration() << "s";
        for (size_t j = 0; j < annot->at(i).getStrings().size(); j++)
            cout << " \"" << (annot->at(i).getStrings())[j] << "\" ";
        cout << endl;
    }
    cout << endl;
}

void printSignalInfo(int sig, EDFHeader* head) {
    cout << fixed << setprecision(4);
    cout << "Signal (" << sig + 1 << ") Info" << endl << endl;
    cout << "Label                | " << head->getLabel(sig) << endl;
    cout << "Record Samples       | " << head->getDataRecordCount() * head->getSignalSampleCount(sig) << endl;
    cout << "Sample Count         | " << head->getSignalSampleCount(sig) << endl;
    cout << "Physical Max         | " << head->getPhysicalMax(sig) << endl;
    cout << "Physical Min         | " << head->getPhysicalMin(sig) << endl;
    cout << "Physical Dimension   | " << head->getPhysicalDimension(sig) << endl;
    cout << "Digital Max          | " << head->getDigitalMax(sig) << endl;
    cout << "Digital Min          | " << head->getDigitalMin(sig) << endl;
    cout << "Prefilter            | " << head->getPrefilter(sig) << endl;
    cout << "Transducer           | " << head->getTransducer(sig) << endl;
    cout << endl << endl;
}

void printAllSignalInfo(EDFHeader* head) {
    cout << fixed << setprecision(4);
    for (int sig = 0; sig < head->getSignalCount() && sig != head->getAnnotationIndex(); sig++) {
        cout << "Signal (" << sig + 1 << ") Info" << endl << endl;
        cout << "Label                | " << head->getLabel(sig) << endl;
        cout << "Record Samples       | " << head->getDataRecordCount() * head->getSignalSampleCount(sig) << endl;
        cout << "Sample Count         | " << head->getSignalSampleCount(sig) << endl;
        cout << "Physical Max         | " << head->getPhysicalMax(sig) << endl;
        cout << "Physical Min         | " << head->getPhysicalMin(sig) << endl;
        cout << "Physical Dimension   | " << head->getPhysicalDimension(sig) << endl;
        cout << "Digital Max          | " << head->getDigitalMax(sig) << endl;
        cout << "Digital Min          | " << head->getDigitalMin(sig) << endl;
        cout << "Prefilter            | " << head->getPrefilter(sig) << endl;
        cout << "Transducer           | " << head->getTransducer(sig) << endl;
        cout << endl << endl;
    }
}

void printSignalData(EDFFile& file, int signal, float start, float length) {
    EDFSignalData* sigData = file.getSignalData(signal, start, length);
    cout << *sigData;
}

int main(int argc, char** argv) {
    EDFFile *newFile = new EDFFile(argv[1]);
    int signal = atoi(argv[2]) - 1;

    EDFHeader* header = newFile->getHeader();
    vector<EDFAnnotation>* annotations = newFile->getAnnotations();

    printHeaderInfo(argv[1], header);
    printSignalInfo(signal, header);
    printAnnotationInfo(annotations);
    
    double startTime = 0.0; // seconds
    double length = 2.0; // seconds
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    printSignalData(*newFile, signal, startTime, length);

    startTime = 1.0;
    length = 1.0;
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    printSignalData(*newFile, signal, startTime, length);
    
    delete newFile;

    return EXIT_SUCCESS;
}

