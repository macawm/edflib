cmake_minimum_required(VERSION 3.2)

project(edflib)

add_definitions(-std=c++11)

set(edflib_hdrs EDFUtil.h EDFDate.h EDFTime.h EDFAnnotation.h EDFPatient.h
	     EDFSignalData.h EDFHeader.h EDFFile.h EDFLib.h)
set(edflib_srcs EDFUtil.cpp EDFDate.cpp EDFTime.cpp EDFAnnotation.cpp EDFPatient.cpp
	     EDFSignalData.cpp EDFHeader.cpp EDFFile.cpp)

add_library(edf STATIC ${edflib_srcs})

add_executable(testapp test.cpp)
target_link_libraries(testapp edf)

add_executable(units ${edflib_srcs} Unit\ Tests/main.cpp)
include_directories(. Catch/include)

install(TARGETS edf DESTINATION lib)
install(FILES ${edflib_hdrs} DESTINATION include)
