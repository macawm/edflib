/* 
 * File:   EDFPatient.cpp
 * Author: macawm
 * 
 * Created on November 4, 2010, 4:36 PM
 */

#include "EDFPatient.h"
#include "EDFUtil.h"

EDFPatient::EDFPatient() : EDFPatient("", "", "", Gender::UNKNOWN, "") {}

EDFPatient::EDFPatient(string newCode, string newName,
        string newAdditional, Gender newGender, string newBirthdate) {
    code = convertSpaces(newCode);
    name = convertSpaces(newName);
    additional = convertSpaces(newAdditional);
    gender = newGender;
    birthdate = newBirthdate;
}

EDFPatient::EDFPatient(const EDFPatient& orig) {
    code       = orig.code;
    name       = orig.name;
    additional = orig.additional;
    gender     = orig.gender;
    birthdate  = orig.birthdate;
}

EDFPatient& EDFPatient::operator=(const EDFPatient& rhs) {
    if (this != &rhs) {
        code       = rhs.code;
        name       = rhs.name;
        additional = rhs.additional;
        gender     = rhs.gender;
        birthdate  = rhs.birthdate;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFPatient p) {
    s << "Code           | " << p.code << std::endl;
    s << "Name           | " << p.name << std::endl;
    s << "Gender         | ";
    if (p.gender == Gender::FEMALE)
        s << "Female" << std::endl;
    else if(p.gender == Gender::MALE)
        s << "Male" << std::endl;
    else
        s << "X" << std::endl;
    s << "Birthdate      | " << p.birthdate << std::endl;
    s << "Additional     | " << p.additional;
    return s;
}

string EDFPatient::getCode() { return code; }
string EDFPatient::getName() { return name; }
string EDFPatient::getAdditional() { return additional; }
Gender EDFPatient::getGender() { return gender; }
string EDFPatient::getBirthdate() { return birthdate; }

void EDFPatient::setCode(string newCode) {
    code = convertSpaces(newCode);
}

void EDFPatient::setName(string newName) {
    name = convertSpaces(newName);
}

void EDFPatient::setAdditional(string newAdditional) {
    additional = convertSpaces(newAdditional);
}

void EDFPatient::setGender(Gender newGender) {
    gender = newGender;
}

void EDFPatient::setBirthdate(string newBirthdate) {
    birthdate = newBirthdate;
}