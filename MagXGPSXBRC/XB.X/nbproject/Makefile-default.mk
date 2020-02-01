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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../hardware.c ../main.c ../swDelay.c ../uart2.c ../uart4.c ../RC.c ../Pot.c ../GPS_I2C.c ../i2c_lib.c ../Gamepad.c ../CycleData.c ../Notice.c ../DMA_UART2.c ../MAG3110.c ../Stepper.c ../TempADC.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/hardware.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/swDelay.o ${OBJECTDIR}/_ext/1472/uart2.o ${OBJECTDIR}/_ext/1472/uart4.o ${OBJECTDIR}/_ext/1472/RC.o ${OBJECTDIR}/_ext/1472/Pot.o ${OBJECTDIR}/_ext/1472/GPS_I2C.o ${OBJECTDIR}/_ext/1472/i2c_lib.o ${OBJECTDIR}/_ext/1472/Gamepad.o ${OBJECTDIR}/_ext/1472/CycleData.o ${OBJECTDIR}/_ext/1472/Notice.o ${OBJECTDIR}/_ext/1472/DMA_UART2.o ${OBJECTDIR}/_ext/1472/MAG3110.o ${OBJECTDIR}/_ext/1472/Stepper.o ${OBJECTDIR}/_ext/1472/TempADC.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/hardware.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/swDelay.o.d ${OBJECTDIR}/_ext/1472/uart2.o.d ${OBJECTDIR}/_ext/1472/uart4.o.d ${OBJECTDIR}/_ext/1472/RC.o.d ${OBJECTDIR}/_ext/1472/Pot.o.d ${OBJECTDIR}/_ext/1472/GPS_I2C.o.d ${OBJECTDIR}/_ext/1472/i2c_lib.o.d ${OBJECTDIR}/_ext/1472/Gamepad.o.d ${OBJECTDIR}/_ext/1472/CycleData.o.d ${OBJECTDIR}/_ext/1472/Notice.o.d ${OBJECTDIR}/_ext/1472/DMA_UART2.o.d ${OBJECTDIR}/_ext/1472/MAG3110.o.d ${OBJECTDIR}/_ext/1472/Stepper.o.d ${OBJECTDIR}/_ext/1472/TempADC.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/hardware.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/swDelay.o ${OBJECTDIR}/_ext/1472/uart2.o ${OBJECTDIR}/_ext/1472/uart4.o ${OBJECTDIR}/_ext/1472/RC.o ${OBJECTDIR}/_ext/1472/Pot.o ${OBJECTDIR}/_ext/1472/GPS_I2C.o ${OBJECTDIR}/_ext/1472/i2c_lib.o ${OBJECTDIR}/_ext/1472/Gamepad.o ${OBJECTDIR}/_ext/1472/CycleData.o ${OBJECTDIR}/_ext/1472/Notice.o ${OBJECTDIR}/_ext/1472/DMA_UART2.o ${OBJECTDIR}/_ext/1472/MAG3110.o ${OBJECTDIR}/_ext/1472/Stepper.o ${OBJECTDIR}/_ext/1472/TempADC.o

# Source Files
SOURCEFILES=../hardware.c ../main.c ../swDelay.c ../uart2.c ../uart4.c ../RC.c ../Pot.c ../GPS_I2C.c ../i2c_lib.c ../Gamepad.c ../CycleData.c ../Notice.c ../DMA_UART2.c ../MAG3110.c ../Stepper.c ../TempADC.c


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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX370F512L
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
${OBJECTDIR}/_ext/1472/hardware.o: ../hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hardware.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hardware.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hardware.o.d" -o ${OBJECTDIR}/_ext/1472/hardware.o ../hardware.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/swDelay.o: ../swDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/swDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/swDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/swDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/swDelay.o.d" -o ${OBJECTDIR}/_ext/1472/swDelay.o ../swDelay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/uart2.o: ../uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/uart2.o.d" -o ${OBJECTDIR}/_ext/1472/uart2.o ../uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/uart4.o: ../uart4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart4.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/uart4.o.d" -o ${OBJECTDIR}/_ext/1472/uart4.o ../uart4.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/RC.o: ../RC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/RC.o.d" -o ${OBJECTDIR}/_ext/1472/RC.o ../RC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Pot.o: ../Pot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Pot.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Pot.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Pot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Pot.o.d" -o ${OBJECTDIR}/_ext/1472/Pot.o ../Pot.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/GPS_I2C.o: ../GPS_I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/GPS_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/GPS_I2C.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/GPS_I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/GPS_I2C.o.d" -o ${OBJECTDIR}/_ext/1472/GPS_I2C.o ../GPS_I2C.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/i2c_lib.o: ../i2c_lib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_lib.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_lib.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/i2c_lib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/i2c_lib.o.d" -o ${OBJECTDIR}/_ext/1472/i2c_lib.o ../i2c_lib.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Gamepad.o: ../Gamepad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Gamepad.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Gamepad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Gamepad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Gamepad.o.d" -o ${OBJECTDIR}/_ext/1472/Gamepad.o ../Gamepad.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/CycleData.o: ../CycleData.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/CycleData.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/CycleData.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/CycleData.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/CycleData.o.d" -o ${OBJECTDIR}/_ext/1472/CycleData.o ../CycleData.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Notice.o: ../Notice.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Notice.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Notice.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Notice.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Notice.o.d" -o ${OBJECTDIR}/_ext/1472/Notice.o ../Notice.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/DMA_UART2.o: ../DMA_UART2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/DMA_UART2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/DMA_UART2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/DMA_UART2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/DMA_UART2.o.d" -o ${OBJECTDIR}/_ext/1472/DMA_UART2.o ../DMA_UART2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/MAG3110.o: ../MAG3110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MAG3110.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MAG3110.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MAG3110.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MAG3110.o.d" -o ${OBJECTDIR}/_ext/1472/MAG3110.o ../MAG3110.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Stepper.o: ../Stepper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Stepper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Stepper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Stepper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Stepper.o.d" -o ${OBJECTDIR}/_ext/1472/Stepper.o ../Stepper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TempADC.o: ../TempADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TempADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TempADC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/TempADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/TempADC.o.d" -o ${OBJECTDIR}/_ext/1472/TempADC.o ../TempADC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1472/hardware.o: ../hardware.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hardware.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hardware.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hardware.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hardware.o.d" -o ${OBJECTDIR}/_ext/1472/hardware.o ../hardware.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/swDelay.o: ../swDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/swDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/swDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/swDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/swDelay.o.d" -o ${OBJECTDIR}/_ext/1472/swDelay.o ../swDelay.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/uart2.o: ../uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/uart2.o.d" -o ${OBJECTDIR}/_ext/1472/uart2.o ../uart2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/uart4.o: ../uart4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart4.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart4.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/uart4.o.d" -o ${OBJECTDIR}/_ext/1472/uart4.o ../uart4.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/RC.o: ../RC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/RC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/RC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/RC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/RC.o.d" -o ${OBJECTDIR}/_ext/1472/RC.o ../RC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Pot.o: ../Pot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Pot.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Pot.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Pot.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Pot.o.d" -o ${OBJECTDIR}/_ext/1472/Pot.o ../Pot.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/GPS_I2C.o: ../GPS_I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/GPS_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/GPS_I2C.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/GPS_I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/GPS_I2C.o.d" -o ${OBJECTDIR}/_ext/1472/GPS_I2C.o ../GPS_I2C.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/i2c_lib.o: ../i2c_lib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_lib.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_lib.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/i2c_lib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/i2c_lib.o.d" -o ${OBJECTDIR}/_ext/1472/i2c_lib.o ../i2c_lib.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Gamepad.o: ../Gamepad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Gamepad.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Gamepad.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Gamepad.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Gamepad.o.d" -o ${OBJECTDIR}/_ext/1472/Gamepad.o ../Gamepad.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/CycleData.o: ../CycleData.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/CycleData.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/CycleData.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/CycleData.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/CycleData.o.d" -o ${OBJECTDIR}/_ext/1472/CycleData.o ../CycleData.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Notice.o: ../Notice.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Notice.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Notice.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Notice.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Notice.o.d" -o ${OBJECTDIR}/_ext/1472/Notice.o ../Notice.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/DMA_UART2.o: ../DMA_UART2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/DMA_UART2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/DMA_UART2.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/DMA_UART2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/DMA_UART2.o.d" -o ${OBJECTDIR}/_ext/1472/DMA_UART2.o ../DMA_UART2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/MAG3110.o: ../MAG3110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MAG3110.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MAG3110.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MAG3110.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MAG3110.o.d" -o ${OBJECTDIR}/_ext/1472/MAG3110.o ../MAG3110.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Stepper.o: ../Stepper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Stepper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Stepper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Stepper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Stepper.o.d" -o ${OBJECTDIR}/_ext/1472/Stepper.o ../Stepper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/TempADC.o: ../TempADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/TempADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/TempADC.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/TempADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/TempADC.o.d" -o ${OBJECTDIR}/_ext/1472/TempADC.o ../TempADC.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC0275F  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/XB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
