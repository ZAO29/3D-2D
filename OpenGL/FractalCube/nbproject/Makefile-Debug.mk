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
	${OBJECTDIR}/App.o \
	${OBJECTDIR}/BasicShader.o \
	${OBJECTDIR}/QuadShader.o \
	${OBJECTDIR}/ZVBO.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=../ZGL/dist/Debug/GNU-Linux/libzgl.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fractalcube

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fractalcube: ../ZGL/dist/Debug/GNU-Linux/libzgl.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fractalcube: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fractalcube ${OBJECTFILES} ${LDLIBSOPTIONS} -std=c++11 -lX11 -lGLEW -lGL -lSOIL

${OBJECTDIR}/App.o: App.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm -I../ -I../../../../../../nicolas/Documents/Perso/3D-2D/OpenGL/ZGL/imgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/App.o App.cpp

${OBJECTDIR}/BasicShader.o: BasicShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm -I../ -I../../../../../../nicolas/Documents/Perso/3D-2D/OpenGL/ZGL/imgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BasicShader.o BasicShader.cpp

${OBJECTDIR}/QuadShader.o: QuadShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm -I../ -I../../../../../../nicolas/Documents/Perso/3D-2D/OpenGL/ZGL/imgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/QuadShader.o QuadShader.cpp

${OBJECTDIR}/ZVBO.o: ZVBO.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm -I../ -I../../../../../../nicolas/Documents/Perso/3D-2D/OpenGL/ZGL/imgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ZVBO.o ZVBO.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm -I../ -I../../../../../../nicolas/Documents/Perso/3D-2D/OpenGL/ZGL/imgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../ZGL && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
	cd ../ZGL && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
