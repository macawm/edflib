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
    static EDFParser* Instance();

    enum ENCODING { UTF8, LATIN1 };

    EDFHeader* parseHeader(std::fstream&);
    vector<EDFAnnotation>* parseAnnotation(std::fstream&, EDFHeader*);
    EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int);
    EDFSignalData* parseSignal(std::fstream&, EDFHeader*, int, double, double);
private:
    EDFParser(){};
    EDFParser(const EDFParser&){};
    bool validOnset(string&);
    bool validDuration(string&);
    
    static EDFParser* instance;
};

#endif	/* _EDFPARSER_H */

