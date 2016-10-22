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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cdfee43a/Scanner.o \
	${OBJECTDIR}/Errores.o \
	${OBJECTDIR}/Lexico.o \
	${OBJECTDIR}/Parametros.o \
	${OBJECTDIR}/Parser.o \
	${OBJECTDIR}/PreScanner.o \
	${OBJECTDIR}/Standar.o \
	${OBJECTDIR}/Util.o \
	${OBJECTDIR}/tds.o


# C Compiler Flags
CFLAGS=-shared

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk /opt/armus/lib/libArmus.Lib.so

/opt/armus/lib/libArmus.Lib.so: ${OBJECTFILES}
	${MKDIR} -p /opt/armus/lib
	${LINK.c} -o /opt/armus/lib/libArmus.Lib.so ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/cdfee43a/Scanner.o: /home/nextor/NetBeansProjects/Armus.Lib/Scanner.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/cdfee43a
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cdfee43a/Scanner.o /home/nextor/NetBeansProjects/Armus.Lib/Scanner.c

${OBJECTDIR}/Errores.o: Errores.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Errores.o Errores.c

${OBJECTDIR}/Lexico.o: Lexico.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Lexico.o Lexico.c

${OBJECTDIR}/Parametros.o: Parametros.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parametros.o Parametros.c

${OBJECTDIR}/Parser.o: Parser.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser.o Parser.c

${OBJECTDIR}/PreScanner.o: PreScanner.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PreScanner.o PreScanner.c

${OBJECTDIR}/Standar.o: Standar.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Standar.o Standar.c

${OBJECTDIR}/Util.o: Util.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Util.o Util.c

${OBJECTDIR}/tds.o: tds.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -w -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tds.o tds.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} /opt/armus/lib/libArmus.Lib.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
