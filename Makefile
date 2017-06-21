#
SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(HOME)/ti/simplelink_msp432_sdk_1_30_00_40

include imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P401R/release

CROSS = $(GCC_ARMCOMPILER)/bin/arm-none-eabi

CC = $(CROSS)-gcc
LNK = $(CROSS)-gcc

INCLUDES =  -I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source \
			-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include \
			-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/posix \
			-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include \
			-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM4F \
			-I$(KERNEL_BUILD) \
			-I$(GCC_ARMCOMPILER)/arm-none-eabi/include \
			-Isrc/damnlib/inc \
		    -Isrc/inc \
		    -Isrc 

OPTIMIZE = -O2

CFLAGS := $(INCLUDES)
CFLAGS += $(OPTIMIZE)
CFLAGS += -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -g -gstrict-dwarf -Wall


LIBS =  -l:$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib/display.am4fg \
	    -l:$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib/drivers_msp432p4xx.am4fg \
	    -l:$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib/fatfs.am4fg \
	    -l:$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/gcc/msp432p4xx_driverlib.a \
	    -l:$(KERNEL_BUILD)/gcc/freertos.lib \

LFLAGS := $(LIBS)
LFLAGS += -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -static -Wl,--gc-sections -lgcc -lc -lm -lnosys

BUILDDIR := build
SRCDIR := src

SRCS_LIB := $(shell find $(SRCDIR)/damnlib -type f -name *.c)
OBJS_LIB := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS_LIB:.c=.o))

SRCS_DEMO := $(shell find $(SRCDIR)/demo -type f -name *.c)
OBJS_DEMO := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS_DEMO:.c=.o))

MSGLIB = $(BUILDDIR)/damnlib.a

DEMO = $(BUILDDIR)/i2cmasterexample1
DEMODIR = $(SRCDIR)/demo
CFLAGS_DEMO = $(CFLAGS) -D__MSP432P401R__ 
LFLAGS_DEMO = $(LFLAGS) -Wl,-T,$(DEMODIR)/MSP_EXP432P401R_FREERTOS.lds -Wl,-Map,$(DEMO).map 

MAINS = $(MSGLIB) $(DEMO).elf

.PHONY: all clean remake

all: $(MAINS)
	@mkdir -p $(BUILDDIR)
	@echo Done compiling $(MAINS)

$(KERNEL_BUILD)/gcc/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $(abspath $@) is not built.
	@ echo You can build it by issuing $(MAKE) in $(abspath $(dir $@)).
	@ $(ECHOBLANKLINE)

$(MSGLIB): $(OBJS_LIB)
	$(CROSS)-ar cr $@ $^ 

$(DEMO).elf: $(OBJS_DEMO) $(KERNEL_BUILD)/gcc/freertos.lib
	$(LNK) $(OBJS_DEMO) $(LFLAGS_DEMO) -o $@

##
# C targets for demo application
$(BUILDDIR)/demo%.o : $(DEMODIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEMO) $(INCLUDES) -c $< -o $@

##
# Generic C targets
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -rf $(BUILDDIR)

remake: clean all
	@echo Finished remake
