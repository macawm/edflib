/* 
 * File:   EDFPatient.h
 * Author: macawm
 *
 * Created on November 4, 2010, 4:36 PM
 */

#ifndef _EDFPATIENT_H
#define	_EDFPATIENT_H

#include <string>
#include <ostream>

using std::string;

class EDFPatient {
public:
    enum GENDER { UNKNOWN, MALE, FEMALE };

    EDFPatient() {};
    EDFPatient(string, string, string, string, GENDER, string);
    EDFPatient(const EDFPatient&);
    virtual ~EDFPatient();

    EDFPatient& operator=(const EDFPatient&);
    friend std::ostream& operator<<(std::ostream&, EDFPatient);

    string getId();
    string getCode();
    string getName();
    string getAdditional();
    GENDER getGender();
    string getBirthdate();

    void setId(string);
    void setCode(string);
    void setName(string);
    void setAdditional(string);
    void setGender(GENDER);
    void setBirthdate(string);
private:
    string id;
    string code;
    string name;
    string additional;
    GENDER gender;
    string birthdate;
};

#endif	/* _EDFPATIENT_H */

