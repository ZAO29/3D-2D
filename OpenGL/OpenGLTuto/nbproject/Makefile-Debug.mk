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
	${OBJECTDIR}/Camera.o \
	${OBJECTDIR}/CameraFree.o \
	${OBJECTDIR}/CameraTrackBall.o \
	${OBJECTDIR}/FBO.o \
	${OBJECTDIR}/LightShader.o \
	${OBJECTDIR}/Material.o \
	${OBJECTDIR}/Mesh.o \
	${OBJECTDIR}/Model.o \
	${OBJECTDIR}/SFXBloom.o \
	${OBJECTDIR}/Shaders.o \
	${OBJECTDIR}/Texture.o \
	${OBJECTDIR}/UIHandler.o \
	${OBJECTDIR}/WindowEnv.o \
	${OBJECTDIR}/imgui.o \
	${OBJECTDIR}/imgui_demo.o \
	${OBJECTDIR}/imgui_draw.o \
	${OBJECTDIR}/imgui_impl_glx.o \
	${OBJECTDIR}/imgui_impl_opengl3.o \
	${OBJECTDIR}/imgui_widgets.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -fpermissive
CXXFLAGS=-std=c++11 -fpermissive

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lassimp

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/opengltuto ${OBJECTFILES} ${LDLIBSOPTIONS} -std=c++11 -lX11 -lGLEW -lGL -lSOIL

${OBJECTDIR}/Application.o: Application.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Application.o Application.cpp

${OBJECTDIR}/BlendingBloomShader.o: BlendingBloomShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BlendingBloomShader.o BlendingBloomShader.cpp

${OBJECTDIR}/BlurShader.o: BlurShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BlurShader.o BlurShader.cpp

${OBJECTDIR}/Camera.o: Camera.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Camera.o Camera.cpp

${OBJECTDIR}/CameraFree.o: CameraFree.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraFree.o CameraFree.cpp

${OBJECTDIR}/CameraTrackBall.o: CameraTrackBall.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraTrackBall.o CameraTrackBall.cpp

${OBJECTDIR}/FBO.o: FBO.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FBO.o FBO.cpp

${OBJECTDIR}/LightShader.o: LightShader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LightShader.o LightShader.cpp

${OBJECTDIR}/Material.o: Material.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Material.o Material.cpp

${OBJECTDIR}/Mesh.o: Mesh.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Mesh.o Mesh.cpp

${OBJECTDIR}/Model.o: Model.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Model.o Model.cpp

${OBJECTDIR}/SFXBloom.o: SFXBloom.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SFXBloom.o SFXBloom.cpp

${OBJECTDIR}/Shaders.o: Shaders.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Shaders.o Shaders.cpp

${OBJECTDIR}/Texture.o: Texture.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Texture.o Texture.cpp

${OBJECTDIR}/UIHandler.o: UIHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UIHandler.o UIHandler.cpp

${OBJECTDIR}/WindowEnv.o: WindowEnv.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WindowEnv.o WindowEnv.cpp

${OBJECTDIR}/imgui.o: imgui.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui.o imgui.cpp

${OBJECTDIR}/imgui_demo.o: imgui_demo.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui_demo.o imgui_demo.cpp

${OBJECTDIR}/imgui_draw.o: imgui_draw.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui_draw.o imgui_draw.cpp

${OBJECTDIR}/imgui_impl_glx.o: imgui_impl_glx.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui_impl_glx.o imgui_impl_glx.cpp

${OBJECTDIR}/imgui_impl_opengl3.o: imgui_impl_opengl3.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui_impl_opengl3.o imgui_impl_opengl3.cpp

${OBJECTDIR}/imgui_widgets.o: imgui_widgets.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/imgui_widgets.o imgui_widgets.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../Include/AlsimGL/glm/ -I../../Include/AlsimGL/SOIL -I/usr/include/assimp -Iimgui -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
