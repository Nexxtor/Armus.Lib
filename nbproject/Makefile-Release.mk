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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cdfee43a/Errores.o \
	${OBJECTDIR}/_ext/cdfee43a/Lexico.o \
	${OBJECTDIR}/_ext/cdfee43a/Parametros.o \
	${OBJECTDIR}/_ext/cdfee43a/PreScanner.o \
	${OBJECTDIR}/_ext/cdfee43a/Scanner.o \
	${OBJECTDIR}/_ext/cdfee43a/Standar.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libArmus.Lib.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libArmus.Lib.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libArmus.Lib.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/cdfee43a/Errores.o: /home/nextor/NetBeansProjects/Armus.Lib/Errores.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Errores.o /home/nextor/NetBeansProjects/Armus.Lib/Errores.c

${OBJECTDIR}/_ext/cdfee43a/Lexico.o: /home/nextor/NetBeansProjects/Armus.Lib/Lexico.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Lexico.o /home/nextor/NetBeansProjects/Armus.Lib/Lexico.c

${OBJECTDIR}/_ext/cdfee43a/Parametros.o: /home/nextor/NetBeansProjects/Armus.Lib/Parametros.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Parametros.o /home/nextor/NetBeansProjects/Armus.Lib/Parametros.c

${OBJECTDIR}/_ext/cdfee43a/PreScanner.o: /home/nextor/NetBeansProjects/Armus.Lib/PreScanner.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/PreScanner.o /home/nextor/NetBeansProjects/Armus.Lib/PreScanner.c

${OBJECTDIR}/_ext/cdfee43a/Scanner.o: /home/nextor/NetBeansProjects/Armus.Lib/Scanner.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Scanner.o /home/nextor/NetBeansProjects/Armus.Lib/Scanner.c

${OBJECTDIR}/_ext/cdfee43a/Standar.o: /home/nextor/NetBeansProjects/Armus.Lib/Standar.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Standar.o /home/nextor/NetBeansProjects/Armus.Lib/Standar.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libArmus.Lib.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
