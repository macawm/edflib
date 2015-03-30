/**
 @file EDFPatient.h
 @brief A model object for an EDF patient. Stores an internal code, name, additional info
 , gender, and birthdate srting.
 
 @author Anthony Magee
 @date 11/4/2010
 */

#ifndef _EDFPATIENT_H
#define	_EDFPATIENT_H

#include <string>
#include <ostream>

enum class Gender { UNKNOWN, MALE, FEMALE };

class EDFPatient {
public:
    EDFPatient();
    EDFPatient(std::string, std::string, std::string, Gender, std::string);
    EDFPatient(const EDFPatient&);
    virtual ~EDFPatient() {};

    EDFPatient& operator=(const EDFPatient&);
    friend std::ostream& operator<<(std::ostream&, EDFPatient);

    std::string code() const;
    std::string name() const;
    std::string additional() const;
    Gender gender() const;
    std::string birthdate() const;

    void setCode(std::string);
    void setName(std::string);
    void setAdditional(std::string);
    void setGender(Gender);
    void setBirthdate(std::string);
private:
    std::string p_code;
    std::string p_name;
    std::string p_additional;
    Gender p_gender;
    std::string p_birthdate;
};

#endif	/* _EDFPATIENT_H */

