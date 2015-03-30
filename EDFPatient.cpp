/**
 @file EDFPatient.cpp
 @author Anthony Magee
 @date 11/4/2010
 */

#include "EDFPatient.h"
#include "EDFUtil.h"

using std::string;

EDFPatient::EDFPatient()
    : p_code("")
    , p_name("")
    , p_additional("")
    , p_gender(Gender::UNKNOWN)
    , p_birthdate("")
{}

EDFPatient::EDFPatient(string newCode, string newName,
        string newAdditional, Gender newGender, string newBirthdate) {
    p_code = convertSpaces(newCode);
    p_name = convertSpaces(newName);
    p_additional = convertSpaces(newAdditional);
    p_gender = newGender;
    p_birthdate = newBirthdate;
}

EDFPatient::EDFPatient(const EDFPatient& orig) {
    p_code       = orig.p_code;
    p_name       = orig.p_name;
    p_additional = orig.p_additional;
    p_gender     = orig.p_gender;
    p_birthdate  = orig.p_birthdate;
}

EDFPatient& EDFPatient::operator=(const EDFPatient& rhs) {
    if (this != &rhs) {
        p_code       = rhs.p_code;
        p_name       = rhs.p_name;
        p_additional = rhs.p_additional;
        p_gender     = rhs.p_gender;
        p_birthdate  = rhs.p_birthdate;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& s, EDFPatient p) {
    s << "Code           | " << p.p_code << std::endl;
    s << "Name           | " << p.p_name << std::endl;
    s << "Gender         | ";
    if (p.p_gender == Gender::FEMALE)
        s << "Female" << std::endl;
    else if(p.p_gender == Gender::MALE)
        s << "Male" << std::endl;
    else
        s << "X" << std::endl;
    s << "Birthdate      | " << p.p_birthdate << std::endl;
    s << "Additional     | " << p.p_additional;
    return s;
}

string EDFPatient::code() const { return p_code; }
string EDFPatient::name() const { return p_name; }
string EDFPatient::additional() const { return p_additional; }
Gender EDFPatient::gender() const { return p_gender; }
string EDFPatient::birthdate() const { return p_birthdate; }

void EDFPatient::setCode(string newCode) {
    p_code = convertSpaces(newCode);
}

void EDFPatient::setName(string newName) {
    p_name = convertSpaces(newName);
}

void EDFPatient::setAdditional(string newAdditional) {
    p_additional = convertSpaces(newAdditional);
}

void EDFPatient::setGender(Gender newGender) {
    p_gender = newGender;
}

void EDFPatient::setBirthdate(string newBirthdate) {
    p_birthdate = newBirthdate;
}