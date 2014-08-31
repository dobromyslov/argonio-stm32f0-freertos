FREERTOS := $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(FREERTOS),.dep/)
    FREERTOS = $(dir $(lastword $(filter-out $(lastword $(MAKEFILE_LIST),$(MAKEFILE_LIST))))
endif

CSRC += $(FREERTOS)/Source/croutine.c \
        $(FREERTOS)/Source/list.c \
        $(FREERTOS)/Source/queue.c \
        $(FREERTOS)/Source/tasks.c \
        $(FREERTOS)/Source/timers.c \
        $(FREERTOS)/Source/CMSIS_RTOS/cmsis_os.c \
        $(FREERTOS)/Source/portable/GCC/ARM_CM0/port.c \
        $(FREERTOS)/Source/portable/MemMang/heap_4.c

INCDIR += $(FREERTOS)/Source/include \
          $(FREERTOS)/Source/CMSIS_RTOS \
          $(FREERTOS)/Source/portable/GCC/ARM_CM0
