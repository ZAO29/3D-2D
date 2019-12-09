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
	${OBJECTDIR}/Camera.o \
	${OBJECTDIR}/CameraFree.o \
	${OBJECTDIR}/CameraTrackBall.o \
	${OBJECTDIR}/FBO.o \
	${OBJECTDIR}/Texture.o \
	${OBJECTDIR}/WindowEnv.o \
	${OBJECTDIR}/ZGLApp.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-D GLM_ENABLE_EXPERIMENTAL
CXXFLAGS=-D GLM_ENABLE_EXPERIMENTAL

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzgl.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzgl.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzgl.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzgl.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libzgl.a

${OBJECTDIR}/Camera.o: Camera.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Camera.o Camera.cpp

${OBJECTDIR}/CameraFree.o: CameraFree.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraFree.o CameraFree.cpp

${OBJECTDIR}/CameraTrackBall.o: CameraTrackBall.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraTrackBall.o CameraTrackBall.cpp

${OBJECTDIR}/FBO.o: FBO.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FBO.o FBO.cpp

${OBJECTDIR}/Texture.o: Texture.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Texture.o Texture.cpp

${OBJECTDIR}/WindowEnv.o: WindowEnv.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WindowEnv.o WindowEnv.cpp

${OBJECTDIR}/ZGLApp.o: ZGLApp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ZGLApp.o ZGLApp.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
