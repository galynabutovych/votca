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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/76882303/dump_atoms_bj.o \
	${OBJECTDIR}/_ext/76882303/dump_atoms.o \
	${OBJECTDIR}/_ext/76882303/shufflenrg.o \
	${OBJECTDIR}/_ext/76882303/ecorrelation.o \
	${OBJECTDIR}/_ext/76882303/lambdaout.o \
	${OBJECTDIR}/_ext/1716269789/statesaversqlite.o \
	${OBJECTDIR}/_ext/76882303/polymerrates.o \
	${OBJECTDIR}/_ext/1716269789/qmpair.o \
	${OBJECTDIR}/_ext/1716269789/calculatorfactory.o \
	${OBJECTDIR}/_ext/1716269789/version_nb.o \
	${OBJECTDIR}/_ext/1716269789/qmnblist.o \
	${OBJECTDIR}/_ext/76882303/ecoulomb.o \
	${OBJECTDIR}/_ext/1716269789/qmtopology.o \
	${OBJECTDIR}/_ext/1716269789/qmdatabase.o \
	${OBJECTDIR}/_ext/76882303/egaussian.o \
	${OBJECTDIR}/_ext/1716269789/qmapplication.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/liblibmd2qm.a

dist/Debug/GNU-Linux-x86/liblibmd2qm.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${RM} dist/Debug/GNU-Linux-x86/liblibmd2qm.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibmd2qm.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/GNU-Linux-x86/liblibmd2qm.a

${OBJECTDIR}/_ext/76882303/dump_atoms_bj.o: ../../src/libmd2qm/calculators/dump_atoms_bj.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/dump_atoms_bj.o ../../src/libmd2qm/calculators/dump_atoms_bj.cc

${OBJECTDIR}/_ext/76882303/dump_atoms.o: ../../src/libmd2qm/calculators/dump_atoms.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/dump_atoms.o ../../src/libmd2qm/calculators/dump_atoms.cc

${OBJECTDIR}/_ext/76882303/shufflenrg.o: ../../src/libmd2qm/calculators/shufflenrg.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/shufflenrg.o ../../src/libmd2qm/calculators/shufflenrg.cc

${OBJECTDIR}/_ext/76882303/ecorrelation.o: ../../src/libmd2qm/calculators/ecorrelation.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/ecorrelation.o ../../src/libmd2qm/calculators/ecorrelation.cc

${OBJECTDIR}/_ext/76882303/lambdaout.o: ../../src/libmd2qm/calculators/lambdaout.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/lambdaout.o ../../src/libmd2qm/calculators/lambdaout.cc

${OBJECTDIR}/_ext/1716269789/statesaversqlite.o: ../../src/libmd2qm/statesaversqlite.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/statesaversqlite.o ../../src/libmd2qm/statesaversqlite.cc

${OBJECTDIR}/_ext/76882303/polymerrates.o: ../../src/libmd2qm/calculators/polymerrates.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/polymerrates.o ../../src/libmd2qm/calculators/polymerrates.cc

${OBJECTDIR}/_ext/1716269789/qmpair.o: ../../src/libmd2qm/qmpair.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/qmpair.o ../../src/libmd2qm/qmpair.cc

${OBJECTDIR}/_ext/1716269789/calculatorfactory.o: ../../src/libmd2qm/calculatorfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/calculatorfactory.o ../../src/libmd2qm/calculatorfactory.cc

${OBJECTDIR}/_ext/1716269789/version_nb.o: ../../src/libmd2qm/version_nb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/version_nb.o ../../src/libmd2qm/version_nb.cc

${OBJECTDIR}/_ext/1716269789/qmnblist.o: ../../src/libmd2qm/qmnblist.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/qmnblist.o ../../src/libmd2qm/qmnblist.cc

${OBJECTDIR}/_ext/76882303/ecoulomb.o: ../../src/libmd2qm/calculators/ecoulomb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/ecoulomb.o ../../src/libmd2qm/calculators/ecoulomb.cc

${OBJECTDIR}/_ext/1716269789/qmtopology.o: ../../src/libmd2qm/qmtopology.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/qmtopology.o ../../src/libmd2qm/qmtopology.cc

${OBJECTDIR}/_ext/1716269789/qmdatabase.o: ../../src/libmd2qm/qmdatabase.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/qmdatabase.o ../../src/libmd2qm/qmdatabase.cc

${OBJECTDIR}/_ext/76882303/egaussian.o: ../../src/libmd2qm/calculators/egaussian.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/76882303
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/76882303/egaussian.o ../../src/libmd2qm/calculators/egaussian.cc

${OBJECTDIR}/_ext/1716269789/qmapplication.o: ../../src/libmd2qm/qmapplication.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1716269789
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../moo/include -I/usr/include/libxml2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1716269789/qmapplication.o ../../src/libmd2qm/qmapplication.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/liblibmd2qm.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
