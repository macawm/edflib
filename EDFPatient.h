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

enum class Gender { UNKNOWN, MALE, FEMALE };

class EDFPatient {
public:
    EDFPatient();
    EDFPatient(string, string, string, Gender, string);
    EDFPatient(const EDFPatient&);
    virtual ~EDFPatient() {};

    EDFPatient& operator=(const EDFPatient&);
    friend std::ostream& operator<<(std::ostream&, EDFPatient);

    string getCode();
    string getName();
    string getAdditional();
    Gender getGender();
    string getBirthdate();

    void setCode(string);
    void setName(string);
    void setAdditional(string);
    void setGender(Gender);
    void setBirthdate(string);
private:
    string code;
    string name;
    string additional;
    Gender gender;
    string birthdate;
};

#endif	/* _EDFPATIENT_H */

