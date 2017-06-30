#
include config.mak
include imports.mak

PROJROOT := $(PWD)

ifeq ($(OS),Windows_NT)
	os := Windows
else
	MSOFTCHK = $(shell grep Microsoft /proc/version)
	ifneq ($(MSOFTCHK),)
		#Why are you calling this from bash in Win10? Making life difficult :/
		SHELL := cmd
	endif
endif

BUILDDIR := build
SRCDIR := src

KERNEL_BUILD := $(SRCDIR)/FreeRTOS

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

LFLAGS := -nostartfiles

LIBS := "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib" -l:display.am4fg \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib" -l:drivers_msp432p401x.am4fg \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib" -l:fatfs.am4fg \
		  "-L$(KERNEL_BUILD)" -l:freertos.lib \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/gcc" -l:msp432p4xx_driverlib.a

LFLAGS += $(LIBS)
LFLAGS += -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -static -Wl,--gc-sections -lgcc -lc -lm -lnosys

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
CFLAGS_DEMO = $(CFLAGS) -D__MSP432P401R__ -DDeviceFamily_MSP432P401x
LFLAGS_DEMO = $(LFLAGS) -Wl,-T,$(DEMODIR)/MSP_EXP432P401R_FREERTOS.lds -D__MSP432P401R__ -DDeviceFamily_MSP432P401x
LFLAGS_DEMO_M = $(LFLAGS_DEMO) -Wl,-Map,$(DEMO_M).map
LFLAGS_DEMO_S = $(LFLAGS_DEMO) -Wl,-Map,$(DEMO_S).map 

MAINS = message $(KERNEL_BUILD)/freertos.lib $(MSGLIB) $(DEMO_M).elf $(DEMO_S).elf

.PHONY: message all clean remake

all: $(MAINS)
	@ mkdir -p $(BUILDDIR)

message: 
	@ echo "########################################"
	@ echo "Group M2 Capstone Spring 2018 Build"
	@ echo "########################################"

$(KERNEL_BUILD)/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ $(MAKE) -C $(KERNEL_BUILD) -I$(PROJROOT)
	@ $(ECHOBLANKLINE)

$(MSGLIB): $(OBJS_LIB)
	@ $(ECHOBLANKLINE)
	@ echo AR $@
	@ $(AR) cr $@ $^

##
# I2C Master Demo
$(DEMO_M).elf: $(OBJS_DEMO_M) $(KERNEL_BUILD)/freertos.lib
	@ echo LNK $@
	@ $(LNK) $(OBJS_DEMO_M) $(LFLAGS_DEMO_M) -o $@

##
# I2C Slave Demo
$(DEMO_S).elf: $(OBJS_DEMO_S) $(KERNEL_BUILD)/freertos.lib
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

clean: message
	@ echo Cleaning
	@ $(RMDIR) $(BUILDDIR)
	@ $(MAKE) -C $(KERNEL_BUILD) -I$(PROJROOT) clean

remake: clean all
	@ echo Finished remake
