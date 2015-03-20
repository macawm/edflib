//
//  main.cpp
//  Unit Tests
//
//  Created by Anthony Magee on 2/19/15.
//
//

#define CATCH_CONFIG_MAIN

#include "EDFLib.h"
#include "catch.hpp"

#include <string>
#include <limits>
#include <algorithm>

using std::string;


TEST_CASE("Utils - String to Double Parsing") {
    SECTION("string to double (bad strings)") {
        REQUIRE(a2f("") == 0);
        REQUIRE(a2f("a") == std::numeric_limits<double>::infinity());
    }
    
    SECTION("string to double (good strings)") {
        REQUIRE(a2f("1") == 1.0);
        REQUIRE(a2f("1.0") == 1.0);
        REQUIRE(a2f("2e2") == 200.0);
        REQUIRE(a2f("-4e-1") == -0.4);
        REQUIRE(a2f("876   ") == 876.0);
        REQUIRE(a2f("   876") == 876.0);
        REQUIRE(a2f("  876 ") == 876.0);
        REQUIRE(a2f("-0") == 0.0);
    }
}

TEST_CASE("Utils - String to Integer Parsing") {
    SECTION("string to integer (bad strings)") {
        REQUIRE(a2i("") == 0);
        REQUIRE(a2i("a") == std::numeric_limits<int>::infinity());
    }
    
    SECTION("string to double (good strings)") {
        REQUIRE(a2i("1") == 1);
        REQUIRE(a2i("1.0") == 1);
        REQUIRE(a2i("2e2") == 2);
        REQUIRE(a2i("-4e-1") == -4);
        REQUIRE(a2i("876   ") == 876);
        REQUIRE(a2i("   876") == 876);
        REQUIRE(a2i("  876 ") == 876);
        REQUIRE(a2i("-0") == 0);
    }
}

TEST_CASE("Utils - Space Replacement") {
    SECTION("space to underscore") {
        REQUIRE(0 == convertSpaces("").compare(""));
        REQUIRE(0 == convertSpaces("first last").compare("first_last"));
        REQUIRE(0 == convertSpaces("first_last").compare("first_last"));
        REQUIRE(0 == convertSpaces("   ").compare("___"));
        REQUIRE(0 == convertSpaces(" a ").compare("_a_"));
    }
    
    SECTION("ensure no side effects") {
        string input = "first last";
        string output = convertSpaces(input);
        REQUIRE(0 == output.compare("first_last"));
        REQUIRE(0 == input.compare(input));
    }
}

TEST_CASE("Utils - Underscore Replacement") {
    SECTION("underscore to space") {
        REQUIRE(0 == convertUnderscores("").compare(""));
        REQUIRE(0 == convertUnderscores("first_last").compare("first last"));
        REQUIRE(0 == convertUnderscores("first last").compare("first last"));
        REQUIRE(0 == convertUnderscores("___").compare("   "));
        REQUIRE(0 == convertUnderscores("_a_").compare(" a "));
    }
    
    SECTION("ensure no side effects") {
        string input = "first_last";
        string output = convertUnderscores(input);
        REQUIRE(0 == output.compare("first last"));
        REQUIRE(0 == input.compare(input));
    }
}

TEST_CASE("Utils - Trim String") {
    SECTION("simple tests") {
        REQUIRE(0 == trim("abc").compare("abc"));
        REQUIRE(0 == trim(" abc").compare("abc"));
        REQUIRE(0 == trim("abc ").compare("abc"));
        REQUIRE(0 == trim(" abc ").compare("abc"));
        REQUIRE(0 == trim(" ab c ").compare("ab c"));
        REQUIRE(0 == trim("").compare(""));
        REQUIRE(0 == trim(" ").compare(""));
        REQUIRE(0 == trim("   ").compare(""));
        REQUIRE(0 == trim("abc").compare("abc"));
    }
    
    SECTION("ensure no side effects") {
        string input = " abc";
        string output = trim(input);
        REQUIRE(0 == output.compare("abc"));
        REQUIRE(0 == input.compare(input));
    }
}


TEST_CASE("Time - String Constructor") {
    EDFTime t1("03.44.00");
    
    SECTION("valid parts") {
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin() == 44);
        REQUIRE(t1.getSec() == 0);
    }
    
    EDFTime t2("3.5.0");
    EDFTime t3("-3.00.00");
    SECTION("invalid parts") {
        REQUIRE(t2.getHour() == 0);
        REQUIRE(t2.getMin() == 0);
        REQUIRE(t2.getSec() == 0);
        
        REQUIRE(t3.getHour() == 0);
        REQUIRE(t3.getMin() == 0);
        REQUIRE(t3.getSec() == 0);
    }
}

TEST_CASE("Time - Seconds Constructor") {
    EDFTime t1(13450);
    
    SECTION("valid seconds") {
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin() == 44);
        REQUIRE(t1.getSec() == 10);
        REQUIRE(t1.asSeconds() == 13450);
    }
    
    EDFTime t2(std::numeric_limits<int>::max() + 1);
    SECTION("invalid seconds") {
        REQUIRE(t2.getHour() == 0);
        REQUIRE(t2.getMin() == 0);
        REQUIRE(t2.getSec() == 0);
        REQUIRE(t2.asSeconds() == 0);
    }
}

TEST_CASE("Time - Three Part Constructor") {
    EDFTime t1(3, 44, 10);
    
    SECTION("valid parts") {
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 10);
        REQUIRE(t1.asSeconds() == 13450);
    }
    
    SECTION("invalid parts") {
        REQUIRE(EDFTime(-1, 0, 0).asSeconds() == 0);
        REQUIRE(EDFTime(0, -1, 0).asSeconds() == 0);
        REQUIRE(EDFTime(0, 0, -1).asSeconds() == 0);
    }
}


TEST_CASE("Time - Copy Constructor") {
    EDFTime t1(3, 44, 10);
    EDFTime *t2 = new EDFTime(t1);
    t1.setHour(0);
    t1.setMin(0);
    t1.setSec(0);
    
    SECTION("different pointers") {
        REQUIRE(&t1 != t2);
    }
    
    SECTION("different parts") {
        REQUIRE(t1.getHour() != t2->getHour());
        REQUIRE(t1.getMin() != t2->getMin());
        REQUIRE(t1.getSec() != t2->getSec());
        REQUIRE(t1.asSeconds() != t2->asSeconds());
    }
    
    delete t2;
}

TEST_CASE("Time - Assignment") {
    EDFTime t1(3, 44, 10);
    EDFTime *t2 = &t1;
    
    SECTION("valid parts") {
        REQUIRE(t1.getSec() == t2->getSec());
        REQUIRE(t1.getMin() == t2->getMin());
        REQUIRE(t1.getHour() == t2->getHour());
    }
}

TEST_CASE("Time - Valid Modifications") {
    EDFTime t1("03.44.00");
    
    SECTION("time seconds modifications") {
        t1.setSec(40);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 40);
        REQUIRE(t1.asSeconds() == 3*3600+44*60+40);
        
        t1.setSec(90);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 45);
        REQUIRE(t1.getSec()  == 30);
        REQUIRE(t1.asSeconds() == 3*3600+45*60+30);
        
        t1.setSec(1200);
        REQUIRE(t1.getHour() == 4);
        REQUIRE(t1.getMin()  == 5);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 4*3600+5*60+0);
    }
    
    t1 = EDFTime("03.44.00");
    SECTION("time minutes modifications") {
        t1.setMin(40);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 40);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 3*3600+40*60+0);
        
        t1.setMin(90);
        REQUIRE(t1.getHour() == 4);
        REQUIRE(t1.getMin()  == 30);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 4*3600+30*60+0);
        
        t1.setMin(1200);
        REQUIRE(t1.getHour() == 24);
        REQUIRE(t1.getMin()  == 0);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 24*3600+0*60+0);
    }
    
    t1 = EDFTime("03.44.00");
    SECTION("time hours modifications") {
        t1.setHour(40);
        REQUIRE(t1.getHour() == 40);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 40*3600+44*60+0);
        
        t1.setHour(1);
        REQUIRE(t1.getHour() == 1);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 0);
        REQUIRE(t1.asSeconds() == 1*3600+44*60+0);
    }
}

TEST_CASE("Time - Invalid Modifications") {
    EDFTime t1("03.44.10");
    
    SECTION("time seconds modifications") {
        t1.setSec(-1);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 10);
    }
    
    SECTION("time minutes modifications") {
        t1.setMin(-1);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 10);
    }
    
    SECTION("time hours modifications") {
        t1.setHour(-1);
        REQUIRE(t1.getHour() == 3);
        REQUIRE(t1.getMin()  == 44);
        REQUIRE(t1.getSec()  == 10);
    }
}


TEST_CASE("Date - Static Helpers") {
    SECTION("int to month string") {
        REQUIRE(0 == EDFDate::monthIntToText(1).compare("JAN"));
        REQUIRE(0 == EDFDate::monthIntToText(2).compare("FEB"));
        REQUIRE(0 == EDFDate::monthIntToText(3).compare("MAR"));
        REQUIRE(0 == EDFDate::monthIntToText(4).compare("APR"));
        REQUIRE(0 == EDFDate::monthIntToText(5).compare("MAY"));
        REQUIRE(0 == EDFDate::monthIntToText(6).compare("JUN"));
        REQUIRE(0 == EDFDate::monthIntToText(7).compare("JLY"));
        REQUIRE(0 == EDFDate::monthIntToText(8).compare("AUG"));
        REQUIRE(0 == EDFDate::monthIntToText(9).compare("SEP"));
        REQUIRE(0 == EDFDate::monthIntToText(10).compare("OCT"));
        REQUIRE(0 == EDFDate::monthIntToText(11).compare("NOV"));
        REQUIRE(0 == EDFDate::monthIntToText(12).compare("DEC"));
        
        REQUIRE(0 == EDFDate::monthIntToText(0).compare(""));
        REQUIRE(0 == EDFDate::monthIntToText(13).compare(""));
    }
    
    SECTION("month string to int") {
        string m;
        REQUIRE(1 == EDFDate::monthTextToInt(m = "JAN"));
        REQUIRE(2 == EDFDate::monthTextToInt(m = "FEB"));
        REQUIRE(3 == EDFDate::monthTextToInt(m = "MAR"));
        REQUIRE(4 == EDFDate::monthTextToInt(m = "APR"));
        REQUIRE(5 == EDFDate::monthTextToInt(m = "MAY"));
        REQUIRE(6 == EDFDate::monthTextToInt(m = "JUN"));
        
        REQUIRE(7 == EDFDate::monthTextToInt(m = "jly"));
        REQUIRE(8 == EDFDate::monthTextToInt(m = "aug"));
        REQUIRE(9 == EDFDate::monthTextToInt(m = "sep"));
        
        REQUIRE(10 == EDFDate::monthTextToInt(m = "oCt"));
        REQUIRE(11 == EDFDate::monthTextToInt(m = "noV"));
        REQUIRE(12 == EDFDate::monthTextToInt(m = "Dec"));
        
        REQUIRE(-1 == EDFDate::monthTextToInt(m = ""));
        REQUIRE(-1 == EDFDate::monthTextToInt(m = "fe"));
        REQUIRE(-1 == EDFDate::monthTextToInt(m = "august"));
    }
}

TEST_CASE("Date - Parts Constructor") {
    SECTION("reasonable values") {
        EDFDate d1(15, 5, 77);
        REQUIRE(d1.getDay() == 15);
        REQUIRE(d1.getMonth() == 5);
        REQUIRE(d1.getYear() == 77);
        REQUIRE(d1.getFullYear() == 2077);
        // read the specs, 0-84 -> 2000-2084, 85-99 -> 1985-1999
        // makes sense that no digital medical recording like this existed before 1985
    }
    
    SECTION("bad values") {
        EDFDate d1(32, 14, 100);
        REQUIRE(d1.getDay() == 1);
        REQUIRE(d1.getMonth() == 1);
        REQUIRE(d1.getYear() == 85);
        REQUIRE(d1.getFullYear() == 1985);
    }
}


TEST_CASE("Data - String Constructor") {
    SECTION("reasonable string") {
        EDFDate d1("02.10.01");
        REQUIRE(d1.getDay() == 2);
        REQUIRE(d1.getMonth() == 10);
        REQUIRE(d1.getYear() == 1);
        REQUIRE(d1.getFullYear() == 2001);
    }
    
    SECTION("bad values") {
        EDFDate d1("2.10.0");
        REQUIRE(d1.getDay() == 1);
        REQUIRE(d1.getMonth() == 1);
        REQUIRE(d1.getYear() == 85);
        REQUIRE(d1.getFullYear() == 1985);
        
        EDFDate d2("1a.10.01");
        REQUIRE(d2.getDay() == 1);
        REQUIRE(d2.getMonth() == 1);
        REQUIRE(d2.getYear() == 85);
        REQUIRE(d2.getFullYear() == 1985);
    }
}

TEST_CASE("Date - Copy Constructor") {
    EDFDate d1(3, 2, 10);
    EDFDate *d2 = new EDFDate(d1);
    d1.setDay(1);
    d1.setMonth(1);
    d1.setYear(0);
    
    SECTION("different pointers") {
        REQUIRE(&d1 != d2);
    }
    
    SECTION("different parts") {
        REQUIRE(d1.getDay() != d2->getDay());
        REQUIRE(d1.getMonth() != d2->getMonth());
        REQUIRE(d1.getYear() != d2->getYear());
    }
    
    delete d2;
}

TEST_CASE("Date - Assignment") {
    EDFDate d1(3, 2, 10);
    EDFDate *d2 = new EDFDate(d1);
    
    SECTION("valid parts") {
        REQUIRE(d1.getDay() == d2->getDay());
        REQUIRE(d1.getMonth() == d2->getMonth());
        REQUIRE(d1.getYear() == d2->getYear());
    }
    
    delete d2;
}

TEST_CASE("Date - Valid Modifications") {
    EDFDate d1(3, 2, 10);
    
    SECTION("valid modification") {
        d1.setDay(5);
        REQUIRE(d1.getDay() == 5);
        
        d1.setMonth(5);
        REQUIRE(d1.getMonth() == 5);
        
        d1.setYear(90);
        REQUIRE(d1.getYear() == 90);
    }
}

TEST_CASE("Date - Invalid Modifications") {
    EDFDate d1(3, 2, 10);
    
    SECTION("valid modification") {
        d1.setDay(0);
        REQUIRE(d1.getDay() == 1);
        d1.setDay(40);
        REQUIRE(d1.getDay() == 1);
        
        d1.setMonth(0);
        REQUIRE(d1.getMonth() == 1);
        d1.setMonth(20);
        REQUIRE(d1.getMonth() == 1);
        
        d1.setYear(-1);
        REQUIRE(d1.getYear() == 85);
        d1.setYear(200);
        REQUIRE(d1.getYear() == 85);
    }
}



TEST_CASE("Patient - Standard Constructor") {
    string code = "56789";
    string name = "Firstname Lastname";
    string add = "Not used";
    Gender gender = Gender::FEMALE;
    string bdate = "09-APR-1955";
    
    EDFPatient p1(code, name, add, gender, bdate);
    
    SECTION("reasonable values") {
        REQUIRE(0 == p1.getCode().compare("56789"));
        REQUIRE(0 == p1.getName().compare("Firstname_Lastname"));
        REQUIRE(0 == p1.getAdditional().compare("Not_used"));
        REQUIRE(p1.getGender() == Gender::FEMALE);
        REQUIRE(0 == p1.getBirthdate().compare("09-APR-1955"));
    }
    
    SECTION("other interesting values") {
        p1.setName("Doe_John");
        REQUIRE(0 == p1.getName().compare("Doe_John"));
        
        p1.setCode("123  a");
        REQUIRE(0 == p1.getCode().compare("123__a"));
        
        p1.setAdditional("some other good info");
        REQUIRE(0 == p1.getAdditional().compare("some_other_good_info"));
    }
}


TEST_CASE("Patient - Copy Constructor") {
    EDFPatient p1("12345", "Last First", "", Gender::UNKNOWN, "01-JAN-2000");
    EDFPatient *p2 = new EDFPatient(p1);
    p1.setName("Doe John");
    p1.setCode("98765");
    p1.setAdditional("none");
    p1.setBirthdate("10-Feb-1955");
    p1.setGender(Gender::MALE);
    
    SECTION("different pointers") {
        REQUIRE(&p1 != p2);
    }
    
    SECTION("different parts") {
        REQUIRE(p1.getCode() != p2->getCode());
        REQUIRE(p1.getName() != p2->getName());
        REQUIRE(p1.getGender() != p2->getGender());
        REQUIRE(p1.getAdditional() != p2->getAdditional());
        REQUIRE(p1.getBirthdate() != p2->getBirthdate());
    }
    
    delete p2;
}

TEST_CASE("Patient - Assignment") {
    EDFPatient p1("12345", "Last First", "", Gender::UNKNOWN, "01-JAN-2000");
    EDFPatient *p2 = &p1;
    
    SECTION("valid parts") {
        REQUIRE(p1.getCode() == p2->getCode());
        REQUIRE(p1.getName() == p2->getName());
        REQUIRE(p1.getGender() == p2->getGender());
        REQUIRE(p1.getAdditional() == p2->getAdditional());
        REQUIRE(p1.getBirthdate() == p2->getBirthdate());
    }
}

TEST_CASE("File - Constructor") {
    EDFFile newFile("../../../../../sample.edf"); // this nasty, but it works on *nix filesystems
    EDFHeader* header = newFile.getHeader();
    
    SECTION("check a known file header") {
        REQUIRE(header->getFiletype() == FileType::EDFPLUS);
        REQUIRE(header->getContinuity() == Continuity::CONTINUOUS);
        REQUIRE(header->getSignalCount() == 38);
        
        EDFDate date = header->getDate();
        REQUIRE(date.getYear() == 5);
        REQUIRE(date.getMonth() == 9);
        REQUIRE(date.getDay() == 15);
        
        EDFTime time = header->getStartTime();
        REQUIRE(time.getHour() == 10);
        REQUIRE(time.getMin() == 18);
        REQUIRE(time.getSec() == 42);
        
        EDFPatient p = header->getPatient();
        REQUIRE(0 == p.getName().compare("L._Smith"));
        REQUIRE(0 == p.getCode().compare("1234567"));
        REQUIRE(0 == p.getBirthdate().compare("09-APR-1955"));
        REQUIRE(p.getGender() == Gender::MALE);
        REQUIRE(0 == p.getAdditional().compare(""));
        
        REQUIRE(0 == header->getRecording().compare(""));
        REQUIRE(0 == header->getRecordingAdditional().compare(""));
        REQUIRE(0 == header->getAdminCode().compare("2"));
        REQUIRE(0 == header->getTechnician().compare("Kesteren"));
        REQUIRE(0 == header->getEquipment().compare("Nihon_Kohden_EEG-1100C_V01.00"));
        
        REQUIRE(header->getDataRecordDuration() == 0.1);
        REQUIRE(header->getDataRecordCount() == 18181);
        REQUIRE(header->getDataRecordSize() == 1530);
        REQUIRE(header->getAnnotationIndex() == 37);
        
        int sigNum = 0;
        REQUIRE(header->signalAvailable(sigNum));
        REQUIRE(0 == header->getLabel(sigNum).compare("EEG FP1"));
        REQUIRE(Approx(header->getPhysicalMax(sigNum)) == 3199.902);
        REQUIRE(Approx(header->getPhysicalMin(sigNum)) == -3200.0);
        REQUIRE(header->getDigitalMax(sigNum) == 32767);
        REQUIRE(header->getDigitalMin(sigNum) == -32768);
        REQUIRE(0 == header->getPhysicalDimension(sigNum).compare("uV"));
        REQUIRE(0 == header->getPrefilter(sigNum).compare(""));
        REQUIRE(0 == header->getTransducer(sigNum).compare(""));
        REQUIRE(0 == header->getReserved(sigNum).compare(""));
        REQUIRE(header->getBufferOffset(sigNum) == 0);
        
        
        sigNum = 35;
        REQUIRE(header->signalAvailable(sigNum));
        REQUIRE(0 == header->getLabel(sigNum).compare("DC03"));
        REQUIRE(Approx(header->getPhysicalMax(sigNum)) == 12002.56);
        REQUIRE(Approx(header->getPhysicalMin(sigNum)) == -12002.9);
        REQUIRE(header->getDigitalMax(sigNum) == 32767);
        REQUIRE(header->getDigitalMin(sigNum) == -32768);
        REQUIRE(0 == header->getPhysicalDimension(sigNum).compare("mV"));
        REQUIRE(0 == header->getPrefilter(sigNum).compare(""));
        REQUIRE(0 == header->getTransducer(sigNum).compare(""));
        REQUIRE(0 == header->getReserved(sigNum).compare(""));
        REQUIRE(header->getBufferOffset(sigNum) == 1400);
        
        REQUIRE(header->hasAnnotations());
        REQUIRE(Approx(header->getRecordingTime()) == 1818.1);
    }
    
    auto annotations = newFile.getAnnotations();
    
    SECTION("check file annotation at index 10") {
        REQUIRE(annotations->size() == 41);
        
        EDFAnnotation ann = annotations->at(10);
        REQUIRE(Approx(ann.getOnset()) == 1127.0);
        REQUIRE(Approx(ann.getDuration()) == 0.0);
        
        auto strings = ann.getStrings();
        REQUIRE(strings.size() == 1);
        REQUIRE(0 == strings.at(0).compare("hoest"));
    }
    
    EDFSignalData *signalPart = newFile.getSignalData(6, 600, 0.5);
    
    SECTION("check some data") {
        REQUIRE(Approx(signalPart->getLength()) == 100.0);
        REQUIRE(Approx(signalPart->getMin()) == -3200.0);
        REQUIRE(Approx(signalPart->getMax()) == 3199.902);
        REQUIRE(Approx(signalPart->getFrequency()) == 200);
        
        vector<int16_t> data = signalPart->getData();
        int knownArray[] = {-86,-87,-65,-77,-114,91,60,108,102,89,-107,-80,-57,-23,268,292,329,157,148,134,368,373,372,
            338,318,264,-4,-5,-39,-39,-58,-90,86,85,98,113,67,107,111,117,112,-121,-106,-96,-109,127,115,106,87,85,66,
            58,39,54,44,70,91,110,127,-104,-79,-52,-24,-4,-13,-24,-2,-29,-42,-42,-30,-8,262,274,270,257,271,-3,-18,-31,
            -29,-23,265,256,-12,-39,-24,273,-5,261,296,308,293,295,274,262,307,300,284,291};
        vector<int16_t> known = vector<int16_t>(knownArray, knownArray + sizeof(knownArray) / sizeof(int));
        REQUIRE(data == known);
    }
}


TEST_CASE("Header - Constructor") {
    EDFHeader *header = new EDFHeader();
    
    SECTION("check default header") {
        REQUIRE(header->getFiletype() == FileType::EDF);
        REQUIRE(header->getContinuity() == Continuity::CONTINUOUS);
        REQUIRE(header->getSignalCount() == 0);
        
        EDFDate date = header->getDate();
        REQUIRE(date.getYear() == 85);
        REQUIRE(date.getMonth() == 1);
        REQUIRE(date.getDay() == 1);
        
        EDFTime time = header->getStartTime();
        REQUIRE(time.getHour() == 0);
        REQUIRE(time.getMin() == 0);
        REQUIRE(time.getSec() == 0);
        
        EDFPatient p = header->getPatient();
        REQUIRE(0 == p.getName().compare(""));
        REQUIRE(0 == p.getCode().compare(""));
        REQUIRE(0 == p.getBirthdate().compare(""));
        REQUIRE(p.getGender() == Gender::UNKNOWN);
        REQUIRE(0 == p.getAdditional().compare(""));
        
        REQUIRE(0 == header->getRecording().compare(""));
        REQUIRE(0 == header->getRecordingAdditional().compare(""));
        REQUIRE(0 == header->getAdminCode().compare(""));
        REQUIRE(0 == header->getTechnician().compare(""));
        REQUIRE(0 == header->getEquipment().compare(""));
        
        REQUIRE(header->getDataRecordDuration() == 0.0);
        REQUIRE(header->getDataRecordCount() == 0);
        REQUIRE(header->getDataRecordSize() == 0);
        REQUIRE(header->getAnnotationIndex() == -1);
        
        int sigNum = 0;
        REQUIRE_FALSE(header->signalAvailable(sigNum));
        REQUIRE(0 == header->getLabel(sigNum).compare("!!!"));
        REQUIRE(Approx(header->getPhysicalMax(sigNum)) == 0.0);
        REQUIRE(Approx(header->getPhysicalMin(sigNum)) == 0.0);
        REQUIRE(header->getDigitalMax(sigNum) == 0);
        REQUIRE(header->getDigitalMin(sigNum) == 0);
        REQUIRE(0 == header->getPhysicalDimension(sigNum).compare("!!!"));
        REQUIRE(0 == header->getPrefilter(sigNum).compare("!!!"));
        REQUIRE(0 == header->getTransducer(sigNum).compare("!!!"));
        REQUIRE(0 == header->getReserved(sigNum).compare("!!!"));
        REQUIRE(header->getBufferOffset(sigNum) == -1);
        
        REQUIRE(header->hasAnnotations() == false);
        REQUIRE(Approx(header->getRecordingTime()) == 0.0);
    }
    
    delete header;
}
