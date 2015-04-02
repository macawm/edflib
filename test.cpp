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
    if (head->filetype() == FileType::EDF)
        cout << "EDF" << endl;
    else
        cout << "EDF+" << (head->continuity() == Continuity::CONTINUOUS ? "C" : "D") << endl;
    cout << "Signal Count         | " << head->signalCount() << endl;
    cout << "Recording Length     | " << head->dataRecordCount() * head->dataRecordDuration() << " seconds" << endl;
    cout << "Start Date           | " << head->date() << endl;
    cout << "Start Time           | " << head->startTime() << endl;
    cout << "Recording            | " << head->recording() << endl;
    cout << "Patient Info ---------" << endl << head->patient() << endl;
    cout << "Admin Code           | " << head->adminCode() << endl;
    cout << "Technician           | " << head->technician() << endl;
    cout << "Equipment            | " << head->equipment() << endl;
    cout << "Recording Additional | " << head->recordingAdditional() << endl;
    cout << "Record Duration      | " << head->dataRecordDuration() << " seconds" << endl;
    cout << "Data Record Count    | " << head->dataRecordCount() << endl;
    cout << "Data Record Size     | " << head->dataRecordSize() << endl;
    cout << "Annotation Signal    | " << head->annotationIndex() + 1 << endl;
    cout << endl << endl;
}

void printAnnotationInfo(vector<EDFAnnotation>* annot) {
    cout << fixed << setprecision(1);
    cout << "Annotation Info" << endl << endl;
    cout << "Annotations" << endl;
    
    int onsetWidth = 2;
    EDFAnnotation last = *(--annot->end());
    int l = last.onset();
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
        cout << " Onset " << std::setw(onsetWidth) << annot->at(i).onset() << "s";
        cout << " Duration " << std::setw(8) << annot->at(i).duration() << "s";
        vector<string> strings = annot->at(i).strings();
        for (size_t j = 0; j < strings.size(); j++)
            cout << " \"" << strings[j] << "\" ";
        cout << endl;
    }
    cout << endl;
}

void printSignalInfo(int sig, EDFHeader* head) {
    cout << fixed << setprecision(4);
    cout << "Signal (" << sig + 1 << ") Info" << endl << endl;
    cout << "Label                | " << head->label(sig) << endl;
    cout << "Record Samples       | " << head->dataRecordCount() * head->signalSampleCount(sig) << endl;
    cout << "Sample Count         | " << head->signalSampleCount(sig) << endl;
    cout << "Physical Max         | " << head->physicalMax(sig) << endl;
    cout << "Physical Min         | " << head->physicalMin(sig) << endl;
    cout << "Physical Dimension   | " << head->physicalDimension(sig) << endl;
    cout << "Digital Max          | " << head->digitalMax(sig) << endl;
    cout << "Digital Min          | " << head->digitalMin(sig) << endl;
    cout << "Prefilter            | " << head->prefilter(sig) << endl;
    cout << "Transducer           | " << head->transducer(sig) << endl;
    cout << endl << endl;
}

void printAllSignalInfo(EDFHeader* head) {
    cout << fixed << setprecision(4);
    for (int sig = 0; sig < head->signalCount() && sig != head->annotationIndex(); sig++) {
        cout << "Signal (" << sig + 1 << ") Info" << endl << endl;
        cout << "Label                | " << head->label(sig) << endl;
        cout << "Record Samples       | " << head->dataRecordCount() * head->signalSampleCount(sig) << endl;
        cout << "Sample Count         | " << head->signalSampleCount(sig) << endl;
        cout << "Physical Max         | " << head->physicalMax(sig) << endl;
        cout << "Physical Min         | " << head->physicalMin(sig) << endl;
        cout << "Physical Dimension   | " << head->physicalDimension(sig) << endl;
        cout << "Digital Max          | " << head->digitalMax(sig) << endl;
        cout << "Digital Min          | " << head->digitalMin(sig) << endl;
        cout << "Prefilter            | " << head->prefilter(sig) << endl;
        cout << "Transducer           | " << head->transducer(sig) << endl;
        cout << endl << endl;
    }
}

int main(int argc, char** argv) {
    EDFFile *newFile = new EDFFile(argv[1]);
    int signal = atoi(argv[2]) - 1;

    EDFHeader* header = newFile->header();
    vector<EDFAnnotation>* annotations = newFile->annotations();

    printHeaderInfo(argv[1], header);
    printSignalInfo(signal, header);
    printAnnotationInfo(annotations);
    
    double startTime = 0.0; // seconds
    double length = 2.0; // seconds
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    EDFSignalData* sigData = newFile->extractSignalData(signal, startTime, length);
    cout << *sigData;
    delete sigData;

    startTime = 1.0;
    length = 1.0;
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    sigData = newFile->extractSignalData(signal, startTime, length);
    cout << *sigData;
    delete sigData;
    
    delete newFile;

    return EXIT_SUCCESS;
}

