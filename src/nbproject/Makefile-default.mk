#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/ads7846.c src/anatype.c src/can.c src/clock.c src/comsbusmaitre.c src/config.c src/echantillonnage.c src/exceptions.c src/font.c src/graphics.c src/gsm+.c src/gsm.c src/image.c src/kalman.c src/main.c src/mtrxsbus.c src/pwm.c src/relais.c src/rtc.c src/s1d13700.c src/secteur.c src/slvxsbus.c src/timers.c src/tor.c src/uart.c src/variables.c src/historique.c src/terminal.c src/eauglycolee.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/ads7846.o ${OBJECTDIR}/src/anatype.o ${OBJECTDIR}/src/can.o ${OBJECTDIR}/src/clock.o ${OBJECTDIR}/src/comsbusmaitre.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/echantillonnage.o ${OBJECTDIR}/src/exceptions.o ${OBJECTDIR}/src/font.o ${OBJECTDIR}/src/graphics.o ${OBJECTDIR}/src/gsm+.o ${OBJECTDIR}/src/gsm.o ${OBJECTDIR}/src/image.o ${OBJECTDIR}/src/kalman.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mtrxsbus.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/relais.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/s1d13700.o ${OBJECTDIR}/src/secteur.o ${OBJECTDIR}/src/slvxsbus.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/tor.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/variables.o ${OBJECTDIR}/src/historique.o ${OBJECTDIR}/src/terminal.o ${OBJECTDIR}/src/eauglycolee.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/ads7846.o.d ${OBJECTDIR}/src/anatype.o.d ${OBJECTDIR}/src/can.o.d ${OBJECTDIR}/src/clock.o.d ${OBJECTDIR}/src/comsbusmaitre.o.d ${OBJECTDIR}/src/config.o.d ${OBJECTDIR}/src/echantillonnage.o.d ${OBJECTDIR}/src/exceptions.o.d ${OBJECTDIR}/src/font.o.d ${OBJECTDIR}/src/graphics.o.d ${OBJECTDIR}/src/gsm+.o.d ${OBJECTDIR}/src/gsm.o.d ${OBJECTDIR}/src/image.o.d ${OBJECTDIR}/src/kalman.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/mtrxsbus.o.d ${OBJECTDIR}/src/pwm.o.d ${OBJECTDIR}/src/relais.o.d ${OBJECTDIR}/src/rtc.o.d ${OBJECTDIR}/src/s1d13700.o.d ${OBJECTDIR}/src/secteur.o.d ${OBJECTDIR}/src/slvxsbus.o.d ${OBJECTDIR}/src/timers.o.d ${OBJECTDIR}/src/tor.o.d ${OBJECTDIR}/src/uart.o.d ${OBJECTDIR}/src/variables.o.d ${OBJECTDIR}/src/historique.o.d ${OBJECTDIR}/src/terminal.o.d ${OBJECTDIR}/src/eauglycolee.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/ads7846.o ${OBJECTDIR}/src/anatype.o ${OBJECTDIR}/src/can.o ${OBJECTDIR}/src/clock.o ${OBJECTDIR}/src/comsbusmaitre.o ${OBJECTDIR}/src/config.o ${OBJECTDIR}/src/echantillonnage.o ${OBJECTDIR}/src/exceptions.o ${OBJECTDIR}/src/font.o ${OBJECTDIR}/src/graphics.o ${OBJECTDIR}/src/gsm+.o ${OBJECTDIR}/src/gsm.o ${OBJECTDIR}/src/image.o ${OBJECTDIR}/src/kalman.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/mtrxsbus.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/relais.o ${OBJECTDIR}/src/rtc.o ${OBJECTDIR}/src/s1d13700.o ${OBJECTDIR}/src/secteur.o ${OBJECTDIR}/src/slvxsbus.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/tor.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/variables.o ${OBJECTDIR}/src/historique.o ${OBJECTDIR}/src/terminal.o ${OBJECTDIR}/src/eauglycolee.o

# Source Files
SOURCEFILES=src/ads7846.c src/anatype.c src/can.c src/clock.c src/comsbusmaitre.c src/config.c src/echantillonnage.c src/exceptions.c src/font.c src/graphics.c src/gsm+.c src/gsm.c src/image.c src/kalman.c src/main.c src/mtrxsbus.c src/pwm.c src/relais.c src/rtc.c src/s1d13700.c src/secteur.c src/slvxsbus.c src/timers.c src/tor.c src/uart.c src/variables.c src/historique.c src/terminal.c src/eauglycolee.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/ads7846.o: src/ads7846.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ads7846.o.d 
	@${RM} ${OBJECTDIR}/src/ads7846.o 
	@${FIXDEPS} "${OBJECTDIR}/src/ads7846.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/ads7846.o.d" -o ${OBJECTDIR}/src/ads7846.o src/ads7846.c  
	
${OBJECTDIR}/src/anatype.o: src/anatype.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/anatype.o.d 
	@${RM} ${OBJECTDIR}/src/anatype.o 
	@${FIXDEPS} "${OBJECTDIR}/src/anatype.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/anatype.o.d" -o ${OBJECTDIR}/src/anatype.o src/anatype.c  
	
${OBJECTDIR}/src/can.o: src/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/can.o.d 
	@${RM} ${OBJECTDIR}/src/can.o 
	@${FIXDEPS} "${OBJECTDIR}/src/can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/can.o.d" -o ${OBJECTDIR}/src/can.o src/can.c  
	
${OBJECTDIR}/src/clock.o: src/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/clock.o.d 
	@${RM} ${OBJECTDIR}/src/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/src/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/clock.o.d" -o ${OBJECTDIR}/src/clock.o src/clock.c  
	
${OBJECTDIR}/src/comsbusmaitre.o: src/comsbusmaitre.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comsbusmaitre.o.d 
	@${RM} ${OBJECTDIR}/src/comsbusmaitre.o 
	@${FIXDEPS} "${OBJECTDIR}/src/comsbusmaitre.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/comsbusmaitre.o.d" -o ${OBJECTDIR}/src/comsbusmaitre.o src/comsbusmaitre.c  
	
${OBJECTDIR}/src/config.o: src/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	@${FIXDEPS} "${OBJECTDIR}/src/config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c  
	
${OBJECTDIR}/src/echantillonnage.o: src/echantillonnage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/echantillonnage.o.d 
	@${RM} ${OBJECTDIR}/src/echantillonnage.o 
	@${FIXDEPS} "${OBJECTDIR}/src/echantillonnage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/echantillonnage.o.d" -o ${OBJECTDIR}/src/echantillonnage.o src/echantillonnage.c  
	
${OBJECTDIR}/src/exceptions.o: src/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/exceptions.o.d 
	@${RM} ${OBJECTDIR}/src/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/src/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/exceptions.o.d" -o ${OBJECTDIR}/src/exceptions.o src/exceptions.c  
	
${OBJECTDIR}/src/font.o: src/font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/font.o.d 
	@${RM} ${OBJECTDIR}/src/font.o 
	@${FIXDEPS} "${OBJECTDIR}/src/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/font.o.d" -o ${OBJECTDIR}/src/font.o src/font.c  
	
${OBJECTDIR}/src/graphics.o: src/graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/graphics.o.d 
	@${RM} ${OBJECTDIR}/src/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/src/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/graphics.o.d" -o ${OBJECTDIR}/src/graphics.o src/graphics.c  
	
${OBJECTDIR}/src/gsm+.o: src/gsm+.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/gsm+.o.d 
	@${RM} ${OBJECTDIR}/src/gsm+.o 
	@${FIXDEPS} "${OBJECTDIR}/src/gsm+.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/gsm+.o.d" -o ${OBJECTDIR}/src/gsm+.o src/gsm+.c  
	
${OBJECTDIR}/src/gsm.o: src/gsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/gsm.o.d 
	@${RM} ${OBJECTDIR}/src/gsm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/gsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/gsm.o.d" -o ${OBJECTDIR}/src/gsm.o src/gsm.c  
	
${OBJECTDIR}/src/image.o: src/image.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/image.o.d 
	@${RM} ${OBJECTDIR}/src/image.o 
	@${FIXDEPS} "${OBJECTDIR}/src/image.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/image.o.d" -o ${OBJECTDIR}/src/image.o src/image.c  
	
${OBJECTDIR}/src/kalman.o: src/kalman.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/kalman.o.d 
	@${RM} ${OBJECTDIR}/src/kalman.o 
	@${FIXDEPS} "${OBJECTDIR}/src/kalman.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/kalman.o.d" -o ${OBJECTDIR}/src/kalman.o src/kalman.c  
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c  
	
${OBJECTDIR}/src/mtrxsbus.o: src/mtrxsbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/mtrxsbus.o.d 
	@${RM} ${OBJECTDIR}/src/mtrxsbus.o 
	@${FIXDEPS} "${OBJECTDIR}/src/mtrxsbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/mtrxsbus.o.d" -o ${OBJECTDIR}/src/mtrxsbus.o src/mtrxsbus.c  
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c  
	
${OBJECTDIR}/src/relais.o: src/relais.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/relais.o.d 
	@${RM} ${OBJECTDIR}/src/relais.o 
	@${FIXDEPS} "${OBJECTDIR}/src/relais.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/relais.o.d" -o ${OBJECTDIR}/src/relais.o src/relais.c  
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/src/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/rtc.o.d" -o ${OBJECTDIR}/src/rtc.o src/rtc.c  
	
${OBJECTDIR}/src/s1d13700.o: src/s1d13700.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/s1d13700.o.d 
	@${RM} ${OBJECTDIR}/src/s1d13700.o 
	@${FIXDEPS} "${OBJECTDIR}/src/s1d13700.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/s1d13700.o.d" -o ${OBJECTDIR}/src/s1d13700.o src/s1d13700.c  
	
${OBJECTDIR}/src/secteur.o: src/secteur.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/secteur.o.d 
	@${RM} ${OBJECTDIR}/src/secteur.o 
	@${FIXDEPS} "${OBJECTDIR}/src/secteur.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/secteur.o.d" -o ${OBJECTDIR}/src/secteur.o src/secteur.c  
	
${OBJECTDIR}/src/slvxsbus.o: src/slvxsbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/slvxsbus.o.d 
	@${RM} ${OBJECTDIR}/src/slvxsbus.o 
	@${FIXDEPS} "${OBJECTDIR}/src/slvxsbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/slvxsbus.o.d" -o ${OBJECTDIR}/src/slvxsbus.o src/slvxsbus.c  
	
${OBJECTDIR}/src/timers.o: src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${RM} ${OBJECTDIR}/src/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/src/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/timers.o.d" -o ${OBJECTDIR}/src/timers.o src/timers.c  
	
${OBJECTDIR}/src/tor.o: src/tor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/tor.o.d 
	@${RM} ${OBJECTDIR}/src/tor.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/tor.o.d" -o ${OBJECTDIR}/src/tor.o src/tor.c  
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/uart.o.d" -o ${OBJECTDIR}/src/uart.o src/uart.c  
	
${OBJECTDIR}/src/variables.o: src/variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/variables.o.d 
	@${RM} ${OBJECTDIR}/src/variables.o 
	@${FIXDEPS} "${OBJECTDIR}/src/variables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/variables.o.d" -o ${OBJECTDIR}/src/variables.o src/variables.c  
	
${OBJECTDIR}/src/historique.o: src/historique.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/historique.o.d 
	@${RM} ${OBJECTDIR}/src/historique.o 
	@${FIXDEPS} "${OBJECTDIR}/src/historique.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/historique.o.d" -o ${OBJECTDIR}/src/historique.o src/historique.c  
	
${OBJECTDIR}/src/terminal.o: src/terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/terminal.o.d 
	@${RM} ${OBJECTDIR}/src/terminal.o 
	@${FIXDEPS} "${OBJECTDIR}/src/terminal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/terminal.o.d" -o ${OBJECTDIR}/src/terminal.o src/terminal.c  
	
${OBJECTDIR}/src/eauglycolee.o: src/eauglycolee.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/eauglycolee.o.d 
	@${RM} ${OBJECTDIR}/src/eauglycolee.o 
	@${FIXDEPS} "${OBJECTDIR}/src/eauglycolee.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/eauglycolee.o.d" -o ${OBJECTDIR}/src/eauglycolee.o src/eauglycolee.c  
	
else
${OBJECTDIR}/src/ads7846.o: src/ads7846.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ads7846.o.d 
	@${RM} ${OBJECTDIR}/src/ads7846.o 
	@${FIXDEPS} "${OBJECTDIR}/src/ads7846.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/ads7846.o.d" -o ${OBJECTDIR}/src/ads7846.o src/ads7846.c  
	
${OBJECTDIR}/src/anatype.o: src/anatype.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/anatype.o.d 
	@${RM} ${OBJECTDIR}/src/anatype.o 
	@${FIXDEPS} "${OBJECTDIR}/src/anatype.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/anatype.o.d" -o ${OBJECTDIR}/src/anatype.o src/anatype.c  
	
${OBJECTDIR}/src/can.o: src/can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/can.o.d 
	@${RM} ${OBJECTDIR}/src/can.o 
	@${FIXDEPS} "${OBJECTDIR}/src/can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/can.o.d" -o ${OBJECTDIR}/src/can.o src/can.c  
	
${OBJECTDIR}/src/clock.o: src/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/clock.o.d 
	@${RM} ${OBJECTDIR}/src/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/src/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/clock.o.d" -o ${OBJECTDIR}/src/clock.o src/clock.c  
	
${OBJECTDIR}/src/comsbusmaitre.o: src/comsbusmaitre.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comsbusmaitre.o.d 
	@${RM} ${OBJECTDIR}/src/comsbusmaitre.o 
	@${FIXDEPS} "${OBJECTDIR}/src/comsbusmaitre.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/comsbusmaitre.o.d" -o ${OBJECTDIR}/src/comsbusmaitre.o src/comsbusmaitre.c  
	
${OBJECTDIR}/src/config.o: src/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/config.o.d 
	@${RM} ${OBJECTDIR}/src/config.o 
	@${FIXDEPS} "${OBJECTDIR}/src/config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/config.o.d" -o ${OBJECTDIR}/src/config.o src/config.c  
	
${OBJECTDIR}/src/echantillonnage.o: src/echantillonnage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/echantillonnage.o.d 
	@${RM} ${OBJECTDIR}/src/echantillonnage.o 
	@${FIXDEPS} "${OBJECTDIR}/src/echantillonnage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/echantillonnage.o.d" -o ${OBJECTDIR}/src/echantillonnage.o src/echantillonnage.c  
	
${OBJECTDIR}/src/exceptions.o: src/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/exceptions.o.d 
	@${RM} ${OBJECTDIR}/src/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/src/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/exceptions.o.d" -o ${OBJECTDIR}/src/exceptions.o src/exceptions.c  
	
${OBJECTDIR}/src/font.o: src/font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/font.o.d 
	@${RM} ${OBJECTDIR}/src/font.o 
	@${FIXDEPS} "${OBJECTDIR}/src/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/font.o.d" -o ${OBJECTDIR}/src/font.o src/font.c  
	
${OBJECTDIR}/src/graphics.o: src/graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/graphics.o.d 
	@${RM} ${OBJECTDIR}/src/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/src/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/graphics.o.d" -o ${OBJECTDIR}/src/graphics.o src/graphics.c  
	
${OBJECTDIR}/src/gsm+.o: src/gsm+.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/gsm+.o.d 
	@${RM} ${OBJECTDIR}/src/gsm+.o 
	@${FIXDEPS} "${OBJECTDIR}/src/gsm+.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/gsm+.o.d" -o ${OBJECTDIR}/src/gsm+.o src/gsm+.c  
	
${OBJECTDIR}/src/gsm.o: src/gsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/gsm.o.d 
	@${RM} ${OBJECTDIR}/src/gsm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/gsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/gsm.o.d" -o ${OBJECTDIR}/src/gsm.o src/gsm.c  
	
${OBJECTDIR}/src/image.o: src/image.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/image.o.d 
	@${RM} ${OBJECTDIR}/src/image.o 
	@${FIXDEPS} "${OBJECTDIR}/src/image.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/image.o.d" -o ${OBJECTDIR}/src/image.o src/image.c  
	
${OBJECTDIR}/src/kalman.o: src/kalman.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/kalman.o.d 
	@${RM} ${OBJECTDIR}/src/kalman.o 
	@${FIXDEPS} "${OBJECTDIR}/src/kalman.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/kalman.o.d" -o ${OBJECTDIR}/src/kalman.o src/kalman.c  
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/main.o.d" -o ${OBJECTDIR}/src/main.o src/main.c  
	
${OBJECTDIR}/src/mtrxsbus.o: src/mtrxsbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/mtrxsbus.o.d 
	@${RM} ${OBJECTDIR}/src/mtrxsbus.o 
	@${FIXDEPS} "${OBJECTDIR}/src/mtrxsbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/mtrxsbus.o.d" -o ${OBJECTDIR}/src/mtrxsbus.o src/mtrxsbus.c  
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c  
	
${OBJECTDIR}/src/relais.o: src/relais.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/relais.o.d 
	@${RM} ${OBJECTDIR}/src/relais.o 
	@${FIXDEPS} "${OBJECTDIR}/src/relais.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/relais.o.d" -o ${OBJECTDIR}/src/relais.o src/relais.c  
	
${OBJECTDIR}/src/rtc.o: src/rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/src/rtc.o 
	@${FIXDEPS} "${OBJECTDIR}/src/rtc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/rtc.o.d" -o ${OBJECTDIR}/src/rtc.o src/rtc.c  
	
${OBJECTDIR}/src/s1d13700.o: src/s1d13700.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/s1d13700.o.d 
	@${RM} ${OBJECTDIR}/src/s1d13700.o 
	@${FIXDEPS} "${OBJECTDIR}/src/s1d13700.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/s1d13700.o.d" -o ${OBJECTDIR}/src/s1d13700.o src/s1d13700.c  
	
${OBJECTDIR}/src/secteur.o: src/secteur.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/secteur.o.d 
	@${RM} ${OBJECTDIR}/src/secteur.o 
	@${FIXDEPS} "${OBJECTDIR}/src/secteur.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/secteur.o.d" -o ${OBJECTDIR}/src/secteur.o src/secteur.c  
	
${OBJECTDIR}/src/slvxsbus.o: src/slvxsbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/slvxsbus.o.d 
	@${RM} ${OBJECTDIR}/src/slvxsbus.o 
	@${FIXDEPS} "${OBJECTDIR}/src/slvxsbus.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/slvxsbus.o.d" -o ${OBJECTDIR}/src/slvxsbus.o src/slvxsbus.c  
	
${OBJECTDIR}/src/timers.o: src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${RM} ${OBJECTDIR}/src/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/src/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/timers.o.d" -o ${OBJECTDIR}/src/timers.o src/timers.c  
	
${OBJECTDIR}/src/tor.o: src/tor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/tor.o.d 
	@${RM} ${OBJECTDIR}/src/tor.o 
	@${FIXDEPS} "${OBJECTDIR}/src/tor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/tor.o.d" -o ${OBJECTDIR}/src/tor.o src/tor.c  
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/uart.o.d" -o ${OBJECTDIR}/src/uart.o src/uart.c  
	
${OBJECTDIR}/src/variables.o: src/variables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/variables.o.d 
	@${RM} ${OBJECTDIR}/src/variables.o 
	@${FIXDEPS} "${OBJECTDIR}/src/variables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/variables.o.d" -o ${OBJECTDIR}/src/variables.o src/variables.c  
	
${OBJECTDIR}/src/historique.o: src/historique.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/historique.o.d 
	@${RM} ${OBJECTDIR}/src/historique.o 
	@${FIXDEPS} "${OBJECTDIR}/src/historique.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/historique.o.d" -o ${OBJECTDIR}/src/historique.o src/historique.c  
	
${OBJECTDIR}/src/terminal.o: src/terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/terminal.o.d 
	@${RM} ${OBJECTDIR}/src/terminal.o 
	@${FIXDEPS} "${OBJECTDIR}/src/terminal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/terminal.o.d" -o ${OBJECTDIR}/src/terminal.o src/terminal.c  
	
${OBJECTDIR}/src/eauglycolee.o: src/eauglycolee.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/eauglycolee.o.d 
	@${RM} ${OBJECTDIR}/src/eauglycolee.o 
	@${FIXDEPS} "${OBJECTDIR}/src/eauglycolee.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"h" -Os -MMD -MF "${OBJECTDIR}/src/eauglycolee.o.d" -o ${OBJECTDIR}/src/eauglycolee.o src/eauglycolee.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_REAL_ICE=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,-Map="MC_Conso" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="MC_Conso"
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/src.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
