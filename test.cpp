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
#include "EDFFile.h"
#include "EDFUtil.h"
#include "EDFHeader.h"
#include "EDFAnnotation.h"
#include "EDFSignalData.h"

using namespace std;

void printHeaderInfo(string filename, EDFHeader* head) {
    cout << "File \"" << filename << "\"" << endl << endl << endl;
    cout << "Header Info" << endl << endl;
    cout << "File Type            | ";
    if (head->getFiletype() == EDFHeader::EDF)
        cout << "EDF" << endl;
    else
        cout << "EDF+" << (head->getContinuity() == 0 ? "C" : "D") << endl;
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
    cout << "Annotation Count     | " << annot->size() << endl;
    for (size_t i = 0; i < annot->size(); i++) {
        cout << " Onset " << annot->at(i).getOnset() << "s";
        cout << " Duration " << annot->at(i).getDuration() << "s";
        for (size_t j = 0; j < annot->at(i).getStrings().size(); j++)
            cout << " \"" << (annot->at(i).getStrings())[j] << "\" ";
        cout << endl;
    }
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
    if (sigData) {
        vector<int16_t> data = sigData->getData();
        cout << endl << sigData->getLength() / sigData->getFrequency() << " seconds  " << sigData->getLength() << " samples" << endl;

        int i = 0;
        for (vector<int16_t>::iterator it = data.begin(); it != data.end(); it++, i++) {
            if (i % 20 == 0)
                cout << endl << i << ":  ";
            cout << *it << " ";
        }
        cout << endl << endl;
    }
}

int main(int argc, char** argv) {
    EDFFile newFile(argv[1]);
    int signal = atoi(argv[2]) - 1;

    EDFHeader* header = newFile.getHeader();
    vector<EDFAnnotation>* annotations = newFile.getAnnotations();

    //printHeaderInfo(argv[1], header);
    printSignalInfo(signal, header);
    //printAnnotationInfo(annotations);
    double startTime = 0.0; // seconds
    double length = 2.0; // seconds
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    printSignalData(newFile, signal, startTime, length);

    startTime = 1.0; // seconds
    length = 1.0; // seconds
    cout << startTime << "s -> " << startTime + length << "s" << endl;
    printSignalData(newFile, signal, startTime, length);

    return EXIT_SUCCESS;
}

