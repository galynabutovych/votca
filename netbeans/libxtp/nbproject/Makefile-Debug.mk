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
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/484478074/ERIs.o \
	${OBJECTDIR}/_ext/484478074/aobasis.o \
	${OBJECTDIR}/_ext/708574269/aocoulomb_g.o \
	${OBJECTDIR}/_ext/708574269/aodipole.o \
	${OBJECTDIR}/_ext/708574269/aoecp.o \
	${OBJECTDIR}/_ext/708574269/aoesp.o \
	${OBJECTDIR}/_ext/708574269/aokinetic.o \
	${OBJECTDIR}/_ext/708574269/aomatrix.o \
	${OBJECTDIR}/_ext/708574269/aomomentum.o \
	${OBJECTDIR}/_ext/708574269/aooverlap.o \
	${OBJECTDIR}/_ext/484478074/aoshell.o \
	${OBJECTDIR}/_ext/484478074/bsecoupling.o \
	${OBJECTDIR}/_ext/484478074/bulkesp.o \
	${OBJECTDIR}/_ext/484478074/calculatorfactory.o \
	${OBJECTDIR}/_ext/1584959903/dftengine.o \
	${OBJECTDIR}/_ext/484478074/esp2multipole.o \
	${OBJECTDIR}/_ext/484478074/espfit.o \
	${OBJECTDIR}/_ext/484478074/extractorfactory.o \
	${OBJECTDIR}/_ext/484478074/gdma.o \
	${OBJECTDIR}/_ext/484478074/grid.o \
	${OBJECTDIR}/_ext/1763088751/bse.o \
	${OBJECTDIR}/_ext/1763088751/gwa.o \
	${OBJECTDIR}/_ext/1763088751/gwbse.o \
	${OBJECTDIR}/_ext/1763088751/rpa.o \
	${OBJECTDIR}/_ext/484478074/job.o \
	${OBJECTDIR}/_ext/484478074/jobapplication.o \
	${OBJECTDIR}/_ext/484478074/jobcalculatorfactory.o \
	${OBJECTDIR}/_ext/164445175/dma.o \
	${OBJECTDIR}/_ext/164445175/egwbse.o \
	${OBJECTDIR}/_ext/164445175/iexcitoncl.o \
	${OBJECTDIR}/_ext/164445175/igwbse.o \
	${OBJECTDIR}/_ext/484478074/lowdin.o \
	${OBJECTDIR}/_ext/484478074/mulliken.o \
	${OBJECTDIR}/_ext/484478074/nbo.o \
	${OBJECTDIR}/_ext/1294340152/numerical_integrations.o \
	${OBJECTDIR}/_ext/1294340152/radial_euler_maclaurin_rule.o \
	${OBJECTDIR}/_ext/1294340152/sphere_lebedev_rule.o \
	${OBJECTDIR}/_ext/484478074/orbitals.o \
	${OBJECTDIR}/_ext/484478074/parallelpaircalc.o \
	${OBJECTDIR}/_ext/484478074/parallelxjobcalc.o \
	${OBJECTDIR}/_ext/484478074/qmapemachine.o \
	${OBJECTDIR}/_ext/484478074/qmcalculator.o \
	${OBJECTDIR}/_ext/484478074/qmdatabase.o \
	${OBJECTDIR}/_ext/484478074/qmmachine.o \
	${OBJECTDIR}/_ext/484478074/qmpackagefactory.o \
	${OBJECTDIR}/_ext/1619336126/cpmd.o \
	${OBJECTDIR}/_ext/1619336126/gaussian.o \
	${OBJECTDIR}/_ext/1619336126/nwchem.o \
	${OBJECTDIR}/_ext/1619336126/orca.o \
	${OBJECTDIR}/_ext/1619336126/turbomole.o \
	${OBJECTDIR}/_ext/484478074/sqlapplication.o \
	${OBJECTDIR}/_ext/484478074/statesaversqlite.o \
	${OBJECTDIR}/_ext/484478074/threecenter_rep.o \
	${OBJECTDIR}/_ext/484478074/threecenters.o \
	${OBJECTDIR}/_ext/484478074/threecenters_dft.o \
	${OBJECTDIR}/_ext/484478074/threecenters_tools.o \
	${OBJECTDIR}/_ext/484478074/toolfactory.o \
	${OBJECTDIR}/_ext/484478074/version.o \
	${OBJECTDIR}/_ext/484478074/version_nb.o \
	${OBJECTDIR}/_ext/484478074/xtpapplication.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../src/libxtp/libxtp.a

../../src/libxtp/libxtp.a: ${OBJECTFILES}
	${MKDIR} -p ../../src/libxtp
	${RM} ../../src/libxtp/libxtp.a
	${AR} -rv ../../src/libxtp/libxtp.a ${OBJECTFILES} 
	$(RANLIB) ../../src/libxtp/libxtp.a

${OBJECTDIR}/_ext/484478074/ERIs.o: ../../src/libxtp/ERIs.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/ERIs.o ../../src/libxtp/ERIs.cc

${OBJECTDIR}/_ext/484478074/aobasis.o: ../../src/libxtp/aobasis.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/aobasis.o ../../src/libxtp/aobasis.cc

${OBJECTDIR}/_ext/708574269/aocoulomb_g.o: ../../src/libxtp/aomatrices/aocoulomb_g.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aocoulomb_g.o ../../src/libxtp/aomatrices/aocoulomb_g.cc

${OBJECTDIR}/_ext/708574269/aodipole.o: ../../src/libxtp/aomatrices/aodipole.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aodipole.o ../../src/libxtp/aomatrices/aodipole.cc

${OBJECTDIR}/_ext/708574269/aoecp.o: ../../src/libxtp/aomatrices/aoecp.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aoecp.o ../../src/libxtp/aomatrices/aoecp.cc

${OBJECTDIR}/_ext/708574269/aoesp.o: ../../src/libxtp/aomatrices/aoesp.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aoesp.o ../../src/libxtp/aomatrices/aoesp.cc

${OBJECTDIR}/_ext/708574269/aokinetic.o: ../../src/libxtp/aomatrices/aokinetic.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aokinetic.o ../../src/libxtp/aomatrices/aokinetic.cc

${OBJECTDIR}/_ext/708574269/aomatrix.o: ../../src/libxtp/aomatrices/aomatrix.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aomatrix.o ../../src/libxtp/aomatrices/aomatrix.cc

${OBJECTDIR}/_ext/708574269/aomomentum.o: ../../src/libxtp/aomatrices/aomomentum.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aomomentum.o ../../src/libxtp/aomatrices/aomomentum.cc

${OBJECTDIR}/_ext/708574269/aooverlap.o: ../../src/libxtp/aomatrices/aooverlap.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/708574269
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708574269/aooverlap.o ../../src/libxtp/aomatrices/aooverlap.cc

${OBJECTDIR}/_ext/484478074/aoshell.o: ../../src/libxtp/aoshell.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/aoshell.o ../../src/libxtp/aoshell.cc

${OBJECTDIR}/_ext/484478074/bsecoupling.o: ../../src/libxtp/bsecoupling.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/bsecoupling.o ../../src/libxtp/bsecoupling.cc

${OBJECTDIR}/_ext/484478074/bulkesp.o: ../../src/libxtp/bulkesp.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/bulkesp.o ../../src/libxtp/bulkesp.cc

${OBJECTDIR}/_ext/484478074/calculatorfactory.o: ../../src/libxtp/calculatorfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/calculatorfactory.o ../../src/libxtp/calculatorfactory.cc

${OBJECTDIR}/_ext/1584959903/dftengine.o: ../../src/libxtp/dftengine/dftengine.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1584959903
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1584959903/dftengine.o ../../src/libxtp/dftengine/dftengine.cc

${OBJECTDIR}/_ext/484478074/esp2multipole.o: ../../src/libxtp/esp2multipole.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/esp2multipole.o ../../src/libxtp/esp2multipole.cc

${OBJECTDIR}/_ext/484478074/espfit.o: ../../src/libxtp/espfit.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/espfit.o ../../src/libxtp/espfit.cc

${OBJECTDIR}/_ext/484478074/extractorfactory.o: ../../src/libxtp/extractorfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/extractorfactory.o ../../src/libxtp/extractorfactory.cc

${OBJECTDIR}/_ext/484478074/gdma.o: ../../src/libxtp/gdma.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/gdma.o ../../src/libxtp/gdma.cc

${OBJECTDIR}/_ext/484478074/grid.o: ../../src/libxtp/grid.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/grid.o ../../src/libxtp/grid.cc

${OBJECTDIR}/_ext/1763088751/bse.o: ../../src/libxtp/gwbse/bse.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1763088751
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1763088751/bse.o ../../src/libxtp/gwbse/bse.cc

${OBJECTDIR}/_ext/1763088751/gwa.o: ../../src/libxtp/gwbse/gwa.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1763088751
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1763088751/gwa.o ../../src/libxtp/gwbse/gwa.cc

${OBJECTDIR}/_ext/1763088751/gwbse.o: ../../src/libxtp/gwbse/gwbse.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1763088751
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1763088751/gwbse.o ../../src/libxtp/gwbse/gwbse.cc

${OBJECTDIR}/_ext/1763088751/rpa.o: ../../src/libxtp/gwbse/rpa.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1763088751
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1763088751/rpa.o ../../src/libxtp/gwbse/rpa.cc

${OBJECTDIR}/_ext/484478074/job.o: ../../src/libxtp/job.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/job.o ../../src/libxtp/job.cc

${OBJECTDIR}/_ext/484478074/jobapplication.o: ../../src/libxtp/jobapplication.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/jobapplication.o ../../src/libxtp/jobapplication.cc

${OBJECTDIR}/_ext/484478074/jobcalculatorfactory.o: ../../src/libxtp/jobcalculatorfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/jobcalculatorfactory.o ../../src/libxtp/jobcalculatorfactory.cc

${OBJECTDIR}/_ext/164445175/dma.o: ../../src/libxtp/jobcalculators/dma.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/164445175
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/164445175/dma.o ../../src/libxtp/jobcalculators/dma.cc

${OBJECTDIR}/_ext/164445175/egwbse.o: ../../src/libxtp/jobcalculators/egwbse.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/164445175
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/164445175/egwbse.o ../../src/libxtp/jobcalculators/egwbse.cc

${OBJECTDIR}/_ext/164445175/iexcitoncl.o: ../../src/libxtp/jobcalculators/iexcitoncl.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/164445175
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/164445175/iexcitoncl.o ../../src/libxtp/jobcalculators/iexcitoncl.cc

${OBJECTDIR}/_ext/164445175/igwbse.o: ../../src/libxtp/jobcalculators/igwbse.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/164445175
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/164445175/igwbse.o ../../src/libxtp/jobcalculators/igwbse.cc

${OBJECTDIR}/_ext/484478074/lowdin.o: ../../src/libxtp/lowdin.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/lowdin.o ../../src/libxtp/lowdin.cc

${OBJECTDIR}/_ext/484478074/mulliken.o: ../../src/libxtp/mulliken.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/mulliken.o ../../src/libxtp/mulliken.cc

${OBJECTDIR}/_ext/484478074/nbo.o: ../../src/libxtp/nbo.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/nbo.o ../../src/libxtp/nbo.cc

${OBJECTDIR}/_ext/1294340152/numerical_integrations.o: ../../src/libxtp/numerical_integration/numerical_integrations.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1294340152
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1294340152/numerical_integrations.o ../../src/libxtp/numerical_integration/numerical_integrations.cc

${OBJECTDIR}/_ext/1294340152/radial_euler_maclaurin_rule.o: ../../src/libxtp/numerical_integration/radial_euler_maclaurin_rule.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1294340152
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1294340152/radial_euler_maclaurin_rule.o ../../src/libxtp/numerical_integration/radial_euler_maclaurin_rule.cc

${OBJECTDIR}/_ext/1294340152/sphere_lebedev_rule.o: ../../src/libxtp/numerical_integration/sphere_lebedev_rule.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1294340152
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1294340152/sphere_lebedev_rule.o ../../src/libxtp/numerical_integration/sphere_lebedev_rule.cc

${OBJECTDIR}/_ext/484478074/orbitals.o: ../../src/libxtp/orbitals.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/orbitals.o ../../src/libxtp/orbitals.cc

${OBJECTDIR}/_ext/484478074/parallelpaircalc.o: ../../src/libxtp/parallelpaircalc.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/parallelpaircalc.o ../../src/libxtp/parallelpaircalc.cc

${OBJECTDIR}/_ext/484478074/parallelxjobcalc.o: ../../src/libxtp/parallelxjobcalc.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/parallelxjobcalc.o ../../src/libxtp/parallelxjobcalc.cc

${OBJECTDIR}/_ext/484478074/qmapemachine.o: ../../src/libxtp/qmapemachine.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/qmapemachine.o ../../src/libxtp/qmapemachine.cc

${OBJECTDIR}/_ext/484478074/qmcalculator.o: ../../src/libxtp/qmcalculator.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/qmcalculator.o ../../src/libxtp/qmcalculator.cc

${OBJECTDIR}/_ext/484478074/qmdatabase.o: ../../src/libxtp/qmdatabase.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/qmdatabase.o ../../src/libxtp/qmdatabase.cc

${OBJECTDIR}/_ext/484478074/qmmachine.o: ../../src/libxtp/qmmachine.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/qmmachine.o ../../src/libxtp/qmmachine.cc

${OBJECTDIR}/_ext/484478074/qmpackagefactory.o: ../../src/libxtp/qmpackagefactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/qmpackagefactory.o ../../src/libxtp/qmpackagefactory.cc

${OBJECTDIR}/_ext/1619336126/cpmd.o: ../../src/libxtp/qmpackages/cpmd.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1619336126
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1619336126/cpmd.o ../../src/libxtp/qmpackages/cpmd.cc

${OBJECTDIR}/_ext/1619336126/gaussian.o: ../../src/libxtp/qmpackages/gaussian.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1619336126
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1619336126/gaussian.o ../../src/libxtp/qmpackages/gaussian.cc

${OBJECTDIR}/_ext/1619336126/nwchem.o: ../../src/libxtp/qmpackages/nwchem.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1619336126
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1619336126/nwchem.o ../../src/libxtp/qmpackages/nwchem.cc

${OBJECTDIR}/_ext/1619336126/orca.o: ../../src/libxtp/qmpackages/orca.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1619336126
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1619336126/orca.o ../../src/libxtp/qmpackages/orca.cc

${OBJECTDIR}/_ext/1619336126/turbomole.o: ../../src/libxtp/qmpackages/turbomole.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/1619336126
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1619336126/turbomole.o ../../src/libxtp/qmpackages/turbomole.cc

${OBJECTDIR}/_ext/484478074/sqlapplication.o: ../../src/libxtp/sqlapplication.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/sqlapplication.o ../../src/libxtp/sqlapplication.cc

${OBJECTDIR}/_ext/484478074/statesaversqlite.o: ../../src/libxtp/statesaversqlite.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/statesaversqlite.o ../../src/libxtp/statesaversqlite.cc

${OBJECTDIR}/_ext/484478074/threecenter_rep.o: ../../src/libxtp/threecenter_rep.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/threecenter_rep.o ../../src/libxtp/threecenter_rep.cc

${OBJECTDIR}/_ext/484478074/threecenters.o: ../../src/libxtp/threecenters.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/threecenters.o ../../src/libxtp/threecenters.cc

${OBJECTDIR}/_ext/484478074/threecenters_dft.o: ../../src/libxtp/threecenters_dft.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/threecenters_dft.o ../../src/libxtp/threecenters_dft.cc

${OBJECTDIR}/_ext/484478074/threecenters_tools.o: ../../src/libxtp/threecenters_tools.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/threecenters_tools.o ../../src/libxtp/threecenters_tools.cc

${OBJECTDIR}/_ext/484478074/toolfactory.o: ../../src/libxtp/toolfactory.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/toolfactory.o ../../src/libxtp/toolfactory.cc

${OBJECTDIR}/_ext/484478074/version.o: ../../src/libxtp/version.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/version.o ../../src/libxtp/version.cc

${OBJECTDIR}/_ext/484478074/version_nb.o: ../../src/libxtp/version_nb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/version_nb.o ../../src/libxtp/version_nb.cc

${OBJECTDIR}/_ext/484478074/xtpapplication.o: ../../src/libxtp/xtpapplication.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/484478074
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../../tools/include -I../../../csg/include -I../../../xtp/include -I/usr/include/libxml2 -I/sw/linux/intel/XE13u2/mkl/include -I../../../ctp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/484478074/xtpapplication.o ../../src/libxtp/xtpapplication.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../src/libxtp/libxtp.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
