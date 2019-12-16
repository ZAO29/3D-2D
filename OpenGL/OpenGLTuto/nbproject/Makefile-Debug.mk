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
	${OBJECTDIR}/Application.o \
	${OBJECTDIR}/BlendingBloomShader.o \
	${OBJECTDIR}/BlurShader.o \
	${OBJECTDIR}/LightShader.o \
	${OBJECTDIR}/Material.o \
	${OBJECTDIR}/Mesh.o \
	${OBJECTDIR}/Model.o \
	${OBJECTDIR}/SFXBloom.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -D GLM_ENABLE_EXPERIMENTAL
CXXFLAGS=-std=c++11 -D GLM_ENABLE_EXPERIMENTAL

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lassimp ../ZGL/dist/Debug/GNU-Linux/libzgl.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto: ../ZGL/dist/Debug/GNU-Linux/libzgl.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto ${OBJECTFILES} ${LDLIBSOPTIONS} -std=c++11 -lX11 -lGLEW -lGL -lSOIL

${OBJECTDIR}/Application.o: Application.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Application.o Application.cpp

${OBJECTDIR}/BlendingBloomShader.o: BlendingBloomShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BlendingBloomShader.o BlendingBloomShader.cpp

${OBJECTDIR}/BlurShader.o: BlurShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BlurShader.o BlurShader.cpp

${OBJECTDIR}/LightShader.o: LightShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LightShader.o LightShader.cpp

${OBJECTDIR}/Material.o: Material.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Material.o Material.cpp

${OBJECTDIR}/Mesh.o: Mesh.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Mesh.o Mesh.cpp

${OBJECTDIR}/Model.o: Model.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Model.o Model.cpp

${OBJECTDIR}/SFXBloom.o: SFXBloom.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SFXBloom.o SFXBloom.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -I.. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
