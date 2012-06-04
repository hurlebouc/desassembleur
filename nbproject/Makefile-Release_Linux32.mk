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
CND_CONF=Release_Linux32
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/instruction.o \
	${OBJECTDIR}/LinkedList.o \
	${OBJECTDIR}/loader.o \
	${OBJECTDIR}/loaderElf.o \
	${OBJECTDIR}/processeur.o \
	${OBJECTDIR}/test7.o \
	${OBJECTDIR}/desassembleur.o \
	${OBJECTDIR}/dyndesass.o \
	${OBJECTDIR}/Fichier.o \
	${OBJECTDIR}/liste_instr.o \
	${OBJECTDIR}/commandes.o \
	${OBJECTDIR}/Graphe.o \
	${OBJECTDIR}/registre.o \
	${OBJECTDIR}/memoire.o \
	${OBJECTDIR}/variable.o \
	${OBJECTDIR}/Vide.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-std=c99

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=lib/Linux.gnu.Debug_32/libBeaEngine_s_d.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/desassembleur-code

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/desassembleur-code: lib/Linux.gnu.Debug_32/libBeaEngine_s_d.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/desassembleur-code: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/desassembleur-code ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/instruction.o: instruction.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/instruction.o instruction.c

${OBJECTDIR}/LinkedList.o: LinkedList.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/LinkedList.o LinkedList.c

${OBJECTDIR}/loader.o: loader.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/loader.o loader.c

${OBJECTDIR}/loaderElf.o: loaderElf.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/loaderElf.o loaderElf.c

${OBJECTDIR}/processeur.o: processeur.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/processeur.o processeur.c

${OBJECTDIR}/test7.o: test7.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/test7.o test7.c

${OBJECTDIR}/desassembleur.o: desassembleur.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/desassembleur.o desassembleur.c

${OBJECTDIR}/dyndesass.o: dyndesass.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/dyndesass.o dyndesass.c

${OBJECTDIR}/Fichier.o: Fichier.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Fichier.o Fichier.c

${OBJECTDIR}/liste_instr.o: liste_instr.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/liste_instr.o liste_instr.c

${OBJECTDIR}/commandes.o: commandes.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/commandes.o commandes.c

${OBJECTDIR}/Graphe.o: Graphe.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Graphe.o Graphe.c

${OBJECTDIR}/registre.o: registre.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/registre.o registre.c

${OBJECTDIR}/memoire.o: memoire.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/memoire.o memoire.c

${OBJECTDIR}/variable.o: variable.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/variable.o variable.c

${OBJECTDIR}/Vide.o: Vide.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Vide.o Vide.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/desassembleur-code

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
