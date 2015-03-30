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
using std::vector;

/***** UTILS *****/

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

/***** UTILS *****/

/***** TIME *****/

TEST_CASE("Time - String Constructor") {
    EDFTime t1("03.44.00");
    
    SECTION("valid parts") {
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute() == 44);
        REQUIRE(t1.second() == 0);
    }
    
    EDFTime t2("3.5.0");
    EDFTime t3("-3.00.00");
    SECTION("invalid parts") {
        REQUIRE(t2.hour() == 0);
        REQUIRE(t2.minute() == 0);
        REQUIRE(t2.second() == 0);
        
        REQUIRE(t3.hour() == 0);
        REQUIRE(t3.minute() == 0);
        REQUIRE(t3.second() == 0);
    }
}

TEST_CASE("Time - Seconds Constructor") {
    EDFTime t1(13450);
    
    SECTION("valid seconds") {
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute() == 44);
        REQUIRE(t1.second() == 10);
        REQUIRE(t1.asSeconds() == 13450);
    }
    
    EDFTime t2(std::numeric_limits<int>::max() + 1);
    SECTION("invalid seconds") {
        REQUIRE(t2.hour() == 0);
        REQUIRE(t2.minute() == 0);
        REQUIRE(t2.second() == 0);
        REQUIRE(t2.asSeconds() == 0);
    }
}

TEST_CASE("Time - Three Part Constructor") {
    EDFTime t1(3, 44, 10);
    
    SECTION("valid parts") {
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 10);
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
    
    SECTION("different pointers") {
        REQUIRE(&t1 != t2);
    }
    
    SECTION("same parts") {
        REQUIRE(t1 == *t2);
    }
    
    delete t2;
}

TEST_CASE("Time - Assignment") {
    EDFTime t1(3, 44, 10);
    EDFTime *t2 = &t1;
    
    SECTION("same parts") {
        REQUIRE(t1 == *t2);
    }
}

TEST_CASE("Time - Valid Modifications") {
    EDFTime t1("03.44.00");
    
    SECTION("time seconds modifications") {
        t1.setSecond(40);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 40);
        REQUIRE(t1.asSeconds() == 3*3600+44*60+40);
        
        t1.setSecond(90);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 45);
        REQUIRE(t1.second()  == 30);
        REQUIRE(t1.asSeconds() == 3*3600+45*60+30);
        
        t1.setSecond(1200);
        REQUIRE(t1.hour() == 4);
        REQUIRE(t1.minute()  == 5);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 4*3600+5*60+0);
    }
    
    t1 = EDFTime("03.44.00");
    SECTION("time minutes modifications") {
        t1.setMinute(40);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 40);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 3*3600+40*60+0);
        
        t1.setMinute(90);
        REQUIRE(t1.hour() == 4);
        REQUIRE(t1.minute()  == 30);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 4*3600+30*60+0);
        
        t1.setMinute(1200);
        REQUIRE(t1.hour() == 24);
        REQUIRE(t1.minute()  == 0);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 24*3600+0*60+0);
    }
    
    t1 = EDFTime("03.44.00");
    SECTION("time hours modifications") {
        t1.setHour(40);
        REQUIRE(t1.hour() == 40);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 40*3600+44*60+0);
        
        t1.setHour(1);
        REQUIRE(t1.hour() == 1);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 0);
        REQUIRE(t1.asSeconds() == 1*3600+44*60+0);
    }
}

TEST_CASE("Time - Invalid Modifications") {
    EDFTime t1("03.44.10");
    
    SECTION("time seconds modifications") {
        t1.setSecond(-1);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 10);
    }
    
    SECTION("time minutes modifications") {
        t1.setMinute(-1);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 10);
    }
    
    SECTION("time hours modifications") {
        t1.setHour(-1);
        REQUIRE(t1.hour() == 3);
        REQUIRE(t1.minute()  == 44);
        REQUIRE(t1.second()  == 10);
    }
}

/***** TIME *****/

/***** DATE *****/

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
        REQUIRE(d1.day() == 15);
        REQUIRE(d1.month() == 5);
        REQUIRE(d1.year() == 77);
        REQUIRE(d1.fullYear() == 2077);
        // read the specs, 0-84 -> 2000-2084, 85-99 -> 1985-1999
        // makes sense that no digital medical recording like this existed before 1985
    }
    
    SECTION("bad values") {
        EDFDate d1(32, 14, 100);
        REQUIRE(d1.day() == 1);
        REQUIRE(d1.month() == 1);
        REQUIRE(d1.year() == 85);
        REQUIRE(d1.fullYear() == 1985);
    }
}


TEST_CASE("Data - String Constructor") {
    SECTION("reasonable string") {
        EDFDate d1("02.10.01");
        REQUIRE(d1.day() == 2);
        REQUIRE(d1.month() == 10);
        REQUIRE(d1.year() == 1);
        REQUIRE(d1.fullYear() == 2001);
    }
    
    SECTION("bad values") {
        EDFDate d1("2.10.0");
        REQUIRE(d1.day() == 1);
        REQUIRE(d1.month() == 1);
        REQUIRE(d1.year() == 85);
        REQUIRE(d1.fullYear() == 1985);
        
        EDFDate d2("1a.10.01");
        REQUIRE(d2.day() == 1);
        REQUIRE(d2.month() == 1);
        REQUIRE(d2.year() == 85);
        REQUIRE(d2.fullYear() == 1985);
    }
}

TEST_CASE("Date - Copy Constructor") {
    EDFDate d1(3, 2, 10);
    EDFDate *d2 = new EDFDate(d1);
    
    SECTION("different pointers") {
        REQUIRE(&d1 != d2);
    }
    
    SECTION("same parts") {
        REQUIRE(d1 == *d2);
    }
    
    delete d2;
}

TEST_CASE("Date - Assignment") {
    EDFDate d1(3, 2, 10);
    EDFDate *d2 = &d1;
    
    SECTION("same parts") {
        REQUIRE(d1 == *d2);
    }
}

TEST_CASE("Date - Valid Modifications") {
    EDFDate d1(3, 2, 10);
    
    SECTION("valid modification") {
        d1.setDay(5);
        REQUIRE(d1.day() == 5);
        
        d1.setMonth(5);
        REQUIRE(d1.month() == 5);
        
        d1.setYear(90);
        REQUIRE(d1.year() == 90);
    }
}

TEST_CASE("Date - Invalid Modifications") {
    EDFDate d1(3, 2, 10);
    
    SECTION("invalid modification") {
        d1.setDay(0);
        REQUIRE(d1.day() == 1);
        d1.setDay(40);
        REQUIRE(d1.day() == 1);
        
        d1.setMonth(0);
        REQUIRE(d1.month() == 1);
        d1.setMonth(20);
        REQUIRE(d1.month() == 1);
        
        d1.setYear(-1);
        REQUIRE(d1.year() == 85);
        d1.setYear(200);
        REQUIRE(d1.year() == 85);
    }
}

/***** DATE *****/

/***** PATIENT *****/

TEST_CASE("Patient - Standard Constructor") {
    string code = "56789";
    string name = "Firstname Lastname";
    string add = "Not used";
    Gender gender = Gender::FEMALE;
    string bdate = "09-APR-1955";
    
    EDFPatient p1(code, name, add, gender, bdate);
    
    SECTION("reasonable values") {
        REQUIRE(0 == p1.code().compare("56789"));
        REQUIRE(0 == p1.name().compare("Firstname_Lastname"));
        REQUIRE(0 == p1.additional().compare("Not_used"));
        REQUIRE(p1.gender() == Gender::FEMALE);
        REQUIRE(0 == p1.birthdate().compare("09-APR-1955"));
    }
    
    SECTION("other interesting values") {
        p1.setName("Doe_John");
        REQUIRE(0 == p1.name().compare("Doe_John"));
        
        p1.setCode("123  a");
        REQUIRE(0 == p1.code().compare("123__a"));
        
        p1.setAdditional("some other good info");
        REQUIRE(0 == p1.additional().compare("some_other_good_info"));
    }
}


TEST_CASE("Patient - Copy Constructor") {
    EDFPatient p1("12345", "Last First", "", Gender::UNKNOWN, "01-JAN-2000");
    EDFPatient *p2 = new EDFPatient(p1);
    
    SECTION("different pointers") {
        REQUIRE(&p1 != p2);
    }
    
    SECTION("same parts") {
        REQUIRE(p1.code() == p2->code());
        REQUIRE(p1.name() == p2->name());
        REQUIRE(p1.gender() == p2->gender());
        REQUIRE(p1.additional() == p2->additional());
        REQUIRE(p1.birthdate() == p2->birthdate());
    }
    
    delete p2;
}

TEST_CASE("Patient - Assignment") {
    EDFPatient p1("12345", "Last First", "", Gender::UNKNOWN, "01-JAN-2000");
    EDFPatient *p2 = &p1;
    
    SECTION("valid parts") {
        REQUIRE(p1.code() == p2->code());
        REQUIRE(p1.name() == p2->name());
        REQUIRE(p1.gender() == p2->gender());
        REQUIRE(p1.additional() == p2->additional());
        REQUIRE(p1.birthdate() == p2->birthdate());
    }
}

/***** PATIENT *****/

/***** SIGNAL DATA *****/

TEST_CASE("SignalData - Constructor") {
    int len = 10;
    double sig[] = {94.0, 32.0, 0.0, -11.0, 90.0, 80.0, -47.0, -39.0, 1.0, 17.0};
    EDFSignalData d1(10, 100, -100);
    d1.addDataPoints(sig, len);
    
    SECTION("test signal properties") {
        REQUIRE(d1.size() == len);
        REQUIRE(d1.channelMin() == -100);
        REQUIRE(d1.channelMax() == 100);
        REQUIRE(Approx(d1.time()) == 1.0);
    }

    SECTION("test signal algorithms") {
        REQUIRE(d1.max() == 94);
        REQUIRE(d1.min() == -47);
        REQUIRE(Approx(d1.mean()) == 21.700000);
        REQUIRE(Approx(d1.variance()) == 2643.566666);
        REQUIRE(Approx(d1.stddev()) == 51.415626);
        REQUIRE(Approx(d1.skewness()) == 0.253116);
        REQUIRE(Approx(d1.kurtosis()) == 1.732129);
    }
}

TEST_CASE("SignalData - modifications") {
    int len = 10;
    double sig[] = {94.0, 32.0, 0.0, -11.0, 90.0, 80.0, -47.0, -39.0, 1.0, 17.0};
    EDFSignalData d1(10, 100, -100);
    d1.addDataPoints(sig, len);
    
    SECTION("test signal additions") {
        d1.addDataPoint(99);
        len++;
        REQUIRE(d1.size() == len);
        REQUIRE(d1.max() == 99);
        REQUIRE(d1.min() == -47);
        REQUIRE(Approx(d1.mean()) == 28.727272);
        REQUIRE(Approx(d1.variance()) == 2922.418181);
        REQUIRE(Approx(d1.stddev()) == 54.059394);
        REQUIRE(Approx(d1.skewness()) == 0.090099);
        REQUIRE(Approx(d1.kurtosis()) == 1.561710);
        
        d1.addDataPoint(-82);
        len++;
        REQUIRE(d1.size() == len);
        REQUIRE(d1.max() == 99);
        REQUIRE(d1.min() == -82);
        REQUIRE(Approx(d1.mean()) == 19.500000);
        REQUIRE(Approx(d1.variance()) == 3678.454545);
        REQUIRE(Approx(d1.stddev()) == 60.650264);
        REQUIRE(Approx(d1.skewness()) == -0.039300);
        REQUIRE(Approx(d1.kurtosis()) == 1.813335);
    }
}

TEST_CASE("SignalData - copy constructor") {
    int len = 10;
    double sig[] = {94.0, 32.0, 0.0, -11.0, 90.0, 80.0, -47.0, -39.0, 1.0, 17.0};
    EDFSignalData d1(10, 100, -100);
    d1.addDataPoints(sig, len);
    EDFSignalData *d2 = new EDFSignalData(d1);
    
    SECTION("different pointers") {
        REQUIRE(&d1 != d2);
    }
    
    SECTION("same parts") {
        REQUIRE(d1.channelMin() == d2->channelMin());
        REQUIRE(d1.channelMax() == d2->channelMax());
        REQUIRE(d1.frequency() == d2->frequency());
        REQUIRE(d1.min() == d2->min());
        REQUIRE(d1.max() == d2->max());
        REQUIRE(d1.size() == d2->size());
        REQUIRE(Approx(d1.time()) == d2->time());
        REQUIRE(d1.data() == d2->data());
        
        REQUIRE(Approx(d1.mean()) == d2->mean());
        REQUIRE(Approx(d1.variance()) == d2->variance());
        REQUIRE(Approx(d1.stddev()) == d2->stddev());
        REQUIRE(Approx(d1.skewness()) == d2->skewness());
        REQUIRE(Approx(d1.kurtosis()) == d2->kurtosis());
    }
    
    delete d2;
}

TEST_CASE("SignalData - assignment") {
    int len = 10;
    double sig[] = {94.0, 32.0, 0.0, -11.0, 90.0, 80.0, -47.0, -39.0, 1.0, 17.0};
    EDFSignalData d1(10, 100, -100);
    d1.addDataPoints(sig, len);
    EDFSignalData *d2 = &d1;
    
    SECTION("same parts") {
        REQUIRE(d1.channelMin() == d2->channelMin());
        REQUIRE(d1.channelMax() == d2->channelMax());
        REQUIRE(d1.frequency() == d2->frequency());
        REQUIRE(d1.min() == d2->min());
        REQUIRE(d1.max() == d2->max());
        REQUIRE(d1.size() == d2->size());
        REQUIRE(Approx(d1.time()) == d2->time());
        REQUIRE(d1.data() == d2->data());
        
        REQUIRE(Approx(d1.mean()) == d2->mean());
        REQUIRE(Approx(d1.variance()) == d2->variance());
        REQUIRE(Approx(d1.stddev()) == d2->stddev());
        REQUIRE(Approx(d1.skewness()) == d2->skewness());
        REQUIRE(Approx(d1.kurtosis()) == d2->kurtosis());
    }
}

TEST_CASE("SignalData - stats numerical stability") {
    int len = 10;
    double sig[] = {94.0, 32.0, 0.0, -11.0, 90.0, 80.0, -47.0, -39.0, 1.0, 17.0};
    EDFSignalData d1(10, 100, -100);
    d1.addDataPoints(sig, len);
    
    SECTION("test some stats") {
        EDFSignalData std(1, 2, -2);
        std.addDataPoints(d1.standardizedData().data(), len);
        REQUIRE(std.size() == len);
        REQUIRE(Approx(std.mean()) == 0.0);
        REQUIRE(Approx(std.variance()) == 1.0);
        REQUIRE(Approx(std.stddev()) == 1.0);
        REQUIRE(Approx(std.skewness()) == 0.253116);
        REQUIRE(Approx(std.kurtosis()) == 1.73213);
    }
    
    EDFSignalData d2(1, std::numeric_limits<double>::max(), std::numeric_limits<double>::min());
    len = 1000;
    for (int i = 0; i < len; i++)
        d2.addDataPoint(3.1415926E10);
    
    SECTION("test some more stats") {
        REQUIRE(d2.size() == len);
        REQUIRE(Approx(d2.mean()) == 3.1415926E10);
        REQUIRE(Approx(d2.variance()) == 0.0);
        REQUIRE(Approx(d2.stddev()) == 0.0);
        REQUIRE(isnan(d2.skewness()));
        REQUIRE(isnan(d2.kurtosis()));
    }
}

/***** SIGNAL DATA *****/

/***** FILE *****/

TEST_CASE("File - Constructor") {
    EDFFile newFile("../../../../../sample.edf"); // this nasty, but it works on *nix filesystems
    EDFHeader* header = newFile.header();
    
    SECTION("check a known file header") {
        REQUIRE(header->filetype() == FileType::EDFPLUS);
        REQUIRE(header->continuity() == Continuity::CONTINUOUS);
        REQUIRE(header->signalCount() == 38);
        
        EDFDate date = header->date();
        REQUIRE(date.year() == 5);
        REQUIRE(date.month() == 9);
        REQUIRE(date.day() == 15);
        
        EDFTime time = header->startTime();
        REQUIRE(time.hour() == 10);
        REQUIRE(time.minute() == 18);
        REQUIRE(time.second() == 42);
        
        EDFPatient p = header->patient();
        REQUIRE(0 == p.name().compare("L._Smith"));
        REQUIRE(0 == p.code().compare("1234567"));
        REQUIRE(0 == p.birthdate().compare("09-APR-1955"));
        REQUIRE(p.gender() == Gender::MALE);
        REQUIRE(0 == p.additional().compare(""));
        
        REQUIRE(0 == header->recording().compare(""));
        REQUIRE(0 == header->recordingAdditional().compare(""));
        REQUIRE(0 == header->adminCode().compare("2"));
        REQUIRE(0 == header->technician().compare("Kesteren"));
        REQUIRE(0 == header->equipment().compare("Nihon_Kohden_EEG-1100C_V01.00"));
        
        REQUIRE(header->dataRecordDuration() == 0.1);
        REQUIRE(header->dataRecordCount() == 18181);
        REQUIRE(header->dataRecordSize() == 1530);
        REQUIRE(header->annotationIndex() == 37);
        
        int sigNum = 0;
        REQUIRE(header->signalAvailable(sigNum));
        REQUIRE(0 == header->label(sigNum).compare("EEG FP1"));
        REQUIRE(Approx(header->physicalMax(sigNum)) == 3199.902);
        REQUIRE(Approx(header->physicalMin(sigNum)) == -3200.0);
        REQUIRE(header->digitalMax(sigNum) == 32767);
        REQUIRE(header->digitalMin(sigNum) == -32768);
        REQUIRE(0 == header->physicalDimension(sigNum).compare("uV"));
        REQUIRE(0 == header->prefilter(sigNum).compare(""));
        REQUIRE(0 == header->transducer(sigNum).compare(""));
        REQUIRE(0 == header->reserved(sigNum).compare(""));
        REQUIRE(header->bufferOffset(sigNum) == 0);
        
        
        sigNum = 35;
        REQUIRE(header->signalAvailable(sigNum));
        REQUIRE(0 == header->label(sigNum).compare("DC03"));
        REQUIRE(Approx(header->physicalMax(sigNum)) == 12002.56);
        REQUIRE(Approx(header->physicalMin(sigNum)) == -12002.9);
        REQUIRE(header->digitalMax(sigNum) == 32767);
        REQUIRE(header->digitalMin(sigNum) == -32768);
        REQUIRE(0 == header->physicalDimension(sigNum).compare("mV"));
        REQUIRE(0 == header->prefilter(sigNum).compare(""));
        REQUIRE(0 == header->transducer(sigNum).compare(""));
        REQUIRE(0 == header->reserved(sigNum).compare(""));
        REQUIRE(header->bufferOffset(sigNum) == 1400);
        
        REQUIRE(header->hasAnnotations());
        REQUIRE(Approx(header->recordingTime()) == 1818.1);
    }
    
    auto annotations = newFile.annotations();
    
    SECTION("check file annotation at index 10") {
        REQUIRE(annotations->size() == 41);
        
        EDFAnnotation ann = annotations->at(10);
        REQUIRE(Approx(ann.onset()) == 1127.0);
        REQUIRE(Approx(ann.duration()) == 0.0);
        
        auto strings = ann.strings();
        REQUIRE(strings.size() == 1);
        REQUIRE(0 == strings.at(0).compare("hoest"));
    }
    
    EDFSignalData *signalPart = newFile.extractSignalData(6, 600, 0.5);
    
    SECTION("check some data") {
        REQUIRE(Approx(signalPart->size()) == 100.0);
        REQUIRE(Approx(signalPart->channelMin()) == -3200.0);
        REQUIRE(Approx(signalPart->channelMax()) == 3199.902);
        REQUIRE(Approx(signalPart->frequency()) == 200);
        
        vector<double> data = signalPart->data();
        int knownArray[] = {-86,-87,-65,-77,-114,91,60,108,102,89,-107,-80,-57,-23,268,292,329,157,148,134,368,373,372,
            338,318,264,-4,-5,-39,-39,-58,-90,86,85,98,113,67,107,111,117,112,-121,-106,-96,-109,127,115,106,87,85,66,
            58,39,54,44,70,91,110,127,-104,-79,-52,-24,-4,-13,-24,-2,-29,-42,-42,-30,-8,262,274,270,257,271,-3,-18,-31,
            -29,-23,265,256,-12,-39,-24,273,-5,261,296,308,293,295,274,262,307,300,284,291};
        vector<double> known = vector<double>(knownArray, knownArray + sizeof(knownArray) / sizeof(int));
        REQUIRE(data == known);
    }
}

/***** FILE *****/

/***** HEADER *****/

TEST_CASE("Header - Constructor") {
    EDFHeader header;
    
    SECTION("check default header") {
        REQUIRE(header.filetype() == FileType::EDF);
        REQUIRE(header.continuity() == Continuity::CONTINUOUS);
        REQUIRE(header.signalCount() == 0);
        
        EDFDate date = header.date();
        REQUIRE(date.year() == 85);
        REQUIRE(date.month() == 1);
        REQUIRE(date.day() == 1);
        
        EDFTime time = header.startTime();
        REQUIRE(time.hour() == 0);
        REQUIRE(time.minute() == 0);
        REQUIRE(time.second() == 0);
        
        EDFPatient p = header.patient();
        REQUIRE(0 == p.name().compare(""));
        REQUIRE(0 == p.code().compare(""));
        REQUIRE(0 == p.birthdate().compare(""));
        REQUIRE(p.gender() == Gender::UNKNOWN);
        REQUIRE(0 == p.additional().compare(""));
        
        REQUIRE(0 == header.recording().compare(""));
        REQUIRE(0 == header.recordingAdditional().compare(""));
        REQUIRE(0 == header.adminCode().compare(""));
        REQUIRE(0 == header.technician().compare(""));
        REQUIRE(0 == header.equipment().compare(""));
        
        REQUIRE(header.dataRecordDuration() == 0.0);
        REQUIRE(header.dataRecordCount() == 0);
        REQUIRE(header.dataRecordSize() == 0);
        REQUIRE(header.annotationIndex() == -1);
        
        int sigNum = 0;
        REQUIRE_FALSE(header.signalAvailable(sigNum));
        REQUIRE(0 == header.label(sigNum).compare("!!!"));
        REQUIRE(Approx(header.physicalMax(sigNum)) == 0.0);
        REQUIRE(Approx(header.physicalMin(sigNum)) == 0.0);
        REQUIRE(header.digitalMax(sigNum) == 0);
        REQUIRE(header.digitalMin(sigNum) == 0);
        REQUIRE(0 == header.physicalDimension(sigNum).compare("!!!"));
        REQUIRE(0 == header.prefilter(sigNum).compare("!!!"));
        REQUIRE(0 == header.transducer(sigNum).compare("!!!"));
        REQUIRE(0 == header.reserved(sigNum).compare("!!!"));
        REQUIRE(header.bufferOffset(sigNum) == -1);
        
        REQUIRE(header.hasAnnotations() == false);
        REQUIRE(Approx(header.recordingTime()) == 0.0);
    }
}

TEST_CASE("Header - copy constructor") {
    EDFFile newFile("../../../../../sample.edf"); // this nasty, but it works on *nix filesystems
    EDFHeader h1 = *newFile.header();
    
    EDFHeader *h2 = new EDFHeader(h1);
    
    SECTION("different pointers") {
        REQUIRE(&h1 != h2);
    }
    
    SECTION("same parts") {
        REQUIRE(h1.filetype() == h2->filetype());
        REQUIRE(h1.continuity() == h2->continuity());
        REQUIRE(h1.signalCount() == h2->signalCount());
        
        REQUIRE(h1.date() == h2->date());
        REQUIRE(h1.startTime() == h2->startTime());
        
        EDFPatient p1 = h1.patient(), p2 = h2->patient();
        REQUIRE(0 == p1.name().compare(p2.name()));
        REQUIRE(0 == p1.code().compare(p2.code()));
        REQUIRE(0 == p1.birthdate().compare(p2.birthdate()));
        REQUIRE(p1.gender() == p2.gender());
        REQUIRE(0 == p1.additional().compare(p2.additional()));
        
        REQUIRE(0 == h1.recording().compare(h2->recording()));
        REQUIRE(0 == h1.recordingAdditional().compare(h2->recordingAdditional()));
        REQUIRE(0 == h1.adminCode().compare(h2->adminCode()));
        REQUIRE(0 == h1.technician().compare(h2->technician()));
        REQUIRE(0 == h1.equipment().compare(h2->equipment()));
        
        REQUIRE(h1.dataRecordDuration() == h2->dataRecordDuration());
        REQUIRE(h1.dataRecordCount() == h2->dataRecordCount());
        REQUIRE(h1.dataRecordSize() == h2->dataRecordSize());
        REQUIRE(h1.annotationIndex() == h2->annotationIndex());
        
        int sigNum = 0;
        REQUIRE(h1.signalAvailable(sigNum));
        REQUIRE(h2->signalAvailable(sigNum));
        
        REQUIRE(0 == h1.label(sigNum).compare(h2->label(sigNum)));
        REQUIRE(Approx(h1.physicalMax(sigNum)) == h2->physicalMax(sigNum));
        REQUIRE(Approx(h1.physicalMin(sigNum)) == h2->physicalMin(sigNum));
        REQUIRE(h1.digitalMax(sigNum) == h2->digitalMax(sigNum));
        REQUIRE(h1.digitalMin(sigNum) == h2->digitalMin(sigNum));
        REQUIRE(0 == h1.physicalDimension(sigNum).compare(h2->physicalDimension(sigNum)));
        REQUIRE(0 == h1.prefilter(sigNum).compare(h2->prefilter(sigNum)));
        REQUIRE(0 == h1.transducer(sigNum).compare(h2->transducer(sigNum)));
        REQUIRE(0 == h1.reserved(sigNum).compare(h2->reserved(sigNum)));
        REQUIRE(h1.bufferOffset(sigNum) == h2->bufferOffset(sigNum));
    }
}

/***** HEADER *****/
