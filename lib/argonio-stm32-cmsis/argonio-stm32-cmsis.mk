ifeq ($(MCU_FAMILY_LC),)
    MCU_FAMILY_LC = $(shell echo $(MCU_FAMILY) | tr '[:upper:]' '[:lower:]')
endif

ifeq ($(MCU_MODEL_FAMILY_LC),)
    MCU_MODEL_FAMILY_LC  = $(shell echo $(MCU_MODEL_FAMILY) | tr '[:upper:]' '[:lower:]')
endif

CMSIS := $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(CMSIS),.dep/)
    CMSIS = $(dir $(lastword $(filter-out $(lastword $(MAKEFILE_LIST),$(MAKEFILE_LIST))))
endif

CSRC += $(CMSIS)/Device/ST/$(MCU_FAMILY)/Source/Templates/system_$(MCU_FAMILY_LC).c

ASMSRC += $(CMSIS)/Device/ST/$(MCU_FAMILY)/Source/Templates/gcc/startup_$(MCU_MODEL_FAMILY_LC).s

INCDIR += $(CMSIS)/Device/ST/$(MCU_FAMILY)/Include \
           $(CMSIS)/Include

