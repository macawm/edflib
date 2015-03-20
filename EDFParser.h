/*
 * File:   EDFParser.h
 *
 * Created on November 4, 2010, 3:36 PM
 */

#ifndef _EDFPARSER_H
#define	_EDFPARSER_H

#include <fstream>
#include <string>
#include <vector>
#include "EDFFile.h"
#include "EDFHeader.h"
#include "EDFSignalData.h"

using std::string;
using std::vector;

class EDFParser {
public:
    /**
     Get the singleton instance of EDFParser
     @return Parser instance.
     */
    static EDFParser* Instance();

    EDFHeader* parseHeader(std::fstream&);
    vector<EDFAnnotation>* parseAnnotation(std::fstream&, EDFHeader*);
    EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int);
    EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int, double, double);
    
private:
    EDFParser() {};
    virtual ~EDFParser();
    EDFParser(const EDFParser&) = delete;
    EDFHeader& operator=(const EDFHeader&) = delete;
    
    bool validOnset(string&);
    bool validDuration(string&);
    bool charactersValid(char*, int);
    
    void parsePatientInfo(const string&, EDFHeader*);
    void parseFileType(const string&, EDFHeader*);
    void parseStdRecordInfo(const string&, EDFHeader*);
    void parsePlusRecordInfo(const string&, EDFHeader*);
    bool parseSignalHeaders(std::fstream&, EDFHeader*);
    bool validFileLength(std::fstream&, EDFHeader*, int);
    
    static EDFParser* instance;
};

#endif	/* _EDFPARSER_H */

