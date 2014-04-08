/* 
 * File:   EDFPatient.cpp
 * Author: macawm
 * 
 * Created on November 4, 2010, 4:36 PM
 */

#include "EDFPatient.h"
#include "EDFUtil.h"

EDFPatient::EDFPatient(string newId, string newCode, string newName,
        string newAdditional, EDFPatient::GENDER newGender, string newBirthdate) {
    id = newId;
    code = convertSpaces(newCode);
    name = convertSpaces(newName);
    additional = newAdditional;
    gender = newGender;
    birthdate = newBirthdate;
}

EDFPatient::EDFPatient(const EDFPatient& orig) {
    id = orig.id;
    code = orig.code;
    name = orig.name;
    additional = orig.additional;
    gender = orig.gender;
    birthdate = orig.birthdate;
}

EDFPatient::~EDFPatient() {
}

EDFPatient& EDFPatient::operator=(const EDFPatient& rhs) {
    if (this != &rhs) {
        id = rhs.id;
        code = rhs.code;
        name = rhs.name;
        additional = rhs.additional;
        gender = rhs.gender;
        birthdate = rhs.birthdate;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFPatient p) {
    s << "ID           | " << p.id << std::endl;
    s << "Code         | " << p.code << std::endl;
    s << "Name         | " << p.name << std::endl;
    s << "Gender       | ";
    if (p.gender == EDFPatient::FEMALE)
        s << "Female" << std::endl;
    else if(p.gender == EDFPatient::MALE)
        s << "Male" << std::endl;
    else
        s << "X" << std::endl;
    s << "Birthdate    | " << p.birthdate << std::endl;
    s << "Additional   | " << p.additional;
    return s;
}

string EDFPatient::getId() { return id; }
string EDFPatient::getCode() { return code; }
string EDFPatient::getName() { return name; }
string EDFPatient::getAdditional() { return additional; }
EDFPatient::GENDER EDFPatient::getGender() { return gender; }
string EDFPatient::getBirthdate() { return birthdate; }

void EDFPatient::setId(string newId) {
    id = newId;
}

void EDFPatient::setCode(string newCode) {
    code = convertSpaces(newCode);
}

void EDFPatient::setName(string newName) {
    name = convertSpaces(newName);
}

void EDFPatient::setAdditional(string newAdditional) {
    additional = newAdditional;
}

void EDFPatient::setGender(EDFPatient::GENDER newGender) {
    gender = newGender;
}

void EDFPatient::setBirthdate(string newBirthdate) {
    birthdate = newBirthdate;
}