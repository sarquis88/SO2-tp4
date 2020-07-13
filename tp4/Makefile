include makedefs

RTOS_SOURCE_DIR=Source
DEMO_SOURCE_DIR=Common/Minimal
LUMINARY_DRIVER_DIR=Common/drivers/LuminaryMicro

CFLAGS+=-I hw_include -I . -I ${RTOS_SOURCE_DIR}/include -I ${LUMINARY_DRIVER_DIR} -I ${RTOS_SOURCE_DIR}/portable/GCC/ARM_CM3 -I ../Common/include -D GCC_ARMCM3_LM3S102 -g

VPATH=${RTOS_SOURCE_DIR}:${RTOS_SOURCE_DIR}/portable/MemMang:${RTOS_SOURCE_DIR}/portable/GCC/ARM_CM3:${DEMO_SOURCE_DIR}:init:hw_include

OBJS=${COMPILER}/main.o			\
	  ${COMPILER}/list.o    	\
      ${COMPILER}/queue.o   	\
      ${COMPILER}/tasks.o   	\
      ${COMPILER}/port.o    	\
      ${COMPILER}/heap_4.o  	\
	  ${COMPILER}/BlockQ.o		\
	  ${COMPILER}/PollQ.o		\
	  ${COMPILER}/integer.o		\
	  ${COMPILER}/semtest.o 	\
	  ${COMPILER}/timertest.o 	\
	  ${COMPILER}/osram96x16.o 	\
	  ${COMPILER}/printf-stdarg.o

INIT_OBJS= ${COMPILER}/startup.o

LIBS= hw_include/libdriver.a

#
# The default rule, which causes init to be built.
#
all: ${COMPILER}           \
     ${COMPILER}/RTOSDemo.axf \

#
# The rule to clean out all the build products
#

clean:
	@rm -rf ${COMPILER} ${wildcard *.bin} RTOSDemo.axf

#
# The rule to create the target directory
#
${COMPILER}:
	@mkdir ${COMPILER}

${COMPILER}/RTOSDemo.axf: ${INIT_OBJS} ${OBJS} ${LIBS}
SCATTER_RTOSDemo=standalone.ld
ENTRY_RTOSDemo=ResetISR

#
#
# Include the automatically generated dependency files.
#
-include ${wildcard ${COMPILER}/*.d} __dummy__
