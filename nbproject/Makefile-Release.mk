#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include edflib-Makefile.mk

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/EDFDate.o \
	${OBJECTDIR}/EDFPatient.o \
	${OBJECTDIR}/EDFFile.o \
	${OBJECTDIR}/EDFTime.o \
	${OBJECTDIR}/test.o \
	${OBJECTDIR}/EDFParser.o \
	${OBJECTDIR}/EDFAnnotation.o \
	${OBJECTDIR}/EDFUtil.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/edflib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/edflib: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/edflib ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/EDFDate.o: EDFDate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFDate.o EDFDate.cpp

${OBJECTDIR}/EDFPatient.o: EDFPatient.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFPatient.o EDFPatient.cpp

${OBJECTDIR}/EDFFile.o: EDFFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFFile.o EDFFile.cpp

${OBJECTDIR}/EDFTime.o: EDFTime.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFTime.o EDFTime.cpp

${OBJECTDIR}/test.o: test.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/test.o test.cpp

${OBJECTDIR}/EDFParser.o: EDFParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFParser.o EDFParser.cpp

${OBJECTDIR}/EDFAnnotation.o: EDFAnnotation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFAnnotation.o EDFAnnotation.cpp

${OBJECTDIR}/EDFUtil.o: EDFUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/EDFUtil.o EDFUtil.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/edflib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
