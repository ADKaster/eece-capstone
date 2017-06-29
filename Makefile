#
include config.mak
include imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P401R/release

CROSS = $(GCC_ARMCOMPILER)/bin/arm-none-eabi

CC = $(CROSS)-gcc
LNK = $(CROSS)-gcc
AR = $(CROSS)-ar

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
CFLAGS += $(EXTRA_CFLAGS)

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

SRCS_DEMO_M := $(shell find $(SRCDIR)/demo/i2cmaster -type f -name *.c)
OBJS_DEMO_M := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS_DEMO_M:.c=.o))
SRCS_DEMO_S := $(shell find $(SRCDIR)/demo/i2cslave -type f -name *.c)
OBJS_DEMO_S := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS_DEMO_S:.c=.o))


MSGLIB = $(BUILDDIR)/damnlib.a

DEMO_M = $(BUILDDIR)/i2cmaster
DEMO_S = $(BUILDDIR)/i2cslave
DEMODIR = $(SRCDIR)/demo
CFLAGS_DEMO = $(CFLAGS) -D__MSP432P401R__ 
LFLAGS_DEMO = $(LFLAGS) -Wl,-T,$(DEMODIR)/MSP_EXP432P401R_FREERTOS.lds
LFLAGS_DEMO_M = $(LFLAGS_DEMO) -Wl,-Map,$(DEMO_M).map
LFLAGS_DEMO_S = $(LFLAGS_DEMO) -Wl,-Map,$(DEMO_S).map 

MAINS = $(MSGLIB) $(DEMO_M).elf $(DEMO_S).elf

.PHONY: all clean remake

all: $(MAINS)
	@mkdir -p $(BUILDDIR)

$(KERNEL_BUILD)/gcc/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $(abspath $@) is not built.
	@ echo You can build it by issuing $(MAKE) in $(abspath $(dir $@)).
	@ $(ECHOBLANKLINE)

$(MSGLIB): $(OBJS_LIB)
	@ echo AR $@
	@ $(AR) cr $@ $^

##
# I2C Master Demo
$(DEMO_M).elf: $(OBJS_DEMO_M) $(KERNEL_BUILD)/gcc/freertos.lib
	@ echo LNK $@
	@ $(LNK) $(OBJS_DEMO_M) $(LFLAGS_DEMO_M) -o $@

##
# I2C Slave Demo
$(DEMO_S).elf: $(OBJS_DEMO_S) $(KERNEL_BUILD)/gcc/freertos.lib
	@ echo LNK $@
	@ $(LNK) $(OBJS_DEMO_S) $(LFLAGS_DEMO_S) -o $@

##
# C targets for demo applications
$(BUILDDIR)/demo%.o : $(DEMODIR)%.c
	@ echo CC $@
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS_DEMO) $(INCLUDES) -c $< -o $@

##
# Generic C targets
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@ mkdir -p $(dir $@)
	@ echo CC $@
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@ echo Cleaning
	@ $(RM) -rf $(BUILDDIR)

remake: clean all
	@ echo Finished remake
