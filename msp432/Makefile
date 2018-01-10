#
include imports.mak

PROJROOT := $(PWD)

BUILDDIR := build
SRCDIR := src

KERNEL_BUILD := $(SRCDIR)/FreeRTOS
FREERTOS = $(BUILDDIR)/freertos.lib

CROSS = $(GCC_ARMCOMPILER)/bin/arm-none-eabi

CC = $(CROSS)-gcc
LNK = $(CROSS)-gcc
AR = $(CROSS)-ar

INCLUDES =  -I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source \
			-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include \
			-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/posix/gcc \
			-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include \
			-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM4F \
			-I$(KERNEL_BUILD) \
			-I$(GCC_ARMCOMPILER)/arm-none-eabi/include \
			-I$(GCC_ARMCOMPILER)/arm-none-eabi/include/newlib-nano \
			-Isrc/damnlib/inc \
		    -Isrc/inc \
		    -Isrc 

OPTIMIZE = -O2

CFLAGS := $(INCLUDES) -MP -MD
CFLAGS += $(OPTIMIZE)
CFLAGS += -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -g -gstrict-dwarf -Wall
CFLAGS += $(EXTRA_CFLAGS)

LFLAGS := -nostartfiles

LIBS :=   "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib" -l:display.am4fg \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib" -l:drivers_msp432p401x.am4fg \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib" -l:fatfs.am4fg \
		  "-L$(BUILDDIR)" -l:freertos.lib \
		  "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/gcc" -l:msp432p4xx_driverlib.a \
		  "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib/thumb/v7e-m/fpv4-sp/hard"

LFLAGS += $(LIBS)
LFLAGS += -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -static -Wl,--gc-sections -lgcc -lc -lm -lnosys --specs=nano.specs

CFLAGS_MSP432 = $(CFLAGS) -D__MSP432P401R__ -DDeviceFamily_MSP432P401x
LFLAGS_MSP432 = $(LFLAGS) -Wl,-T,$(SRCDIR)/MSP_EXP432P401R_FREERTOS.lds -D__MSP432P401R__ -DDeviceFamily_MSP432P401x

SRCS_LIB := $(wildcard $(SRCDIR)/damnlib/*.c) $(wildcard $(SRCDIR)/damnlib/*/*.c) 
OBJS_LIB  := $(SRCS_LIB:$(SRCDIR)%.c=$(BUILDDIR)%.o)

SRCS_DEMO_M := $(wildcard $(SRCDIR)/demo/i2cmaster/*.c)
OBJS_DEMO_M := $(SRCS_DEMO_M:$(SRCDIR)%.c=$(BUILDDIR)%.o)

SRCS_DEMO_S := $(wildcard $(SRCDIR)/demo/i2cslave/*.c)
OBJS_DEMO_S := $(SRCS_DEMO_S:$(SRCDIR)%.c=$(BUILDDIR)%.o)

MSGLIB = $(BUILDDIR)/damnlib.a

DEMO_M = $(BUILDDIR)/i2cmaster
DEMO_S = $(BUILDDIR)/i2cslave
DEMODIR = $(SRCDIR)/demo
LFLAGS_DEMO_M = $(LFLAGS_MSP432) -Wl,-Map,$(DEMO_M).map
LFLAGS_DEMO_S = $(LFLAGS_MSP432) -Wl,-Map,$(DEMO_S).map 

EXAMPLE := $(BUILDDIR)/example
SRCS_EXAMP := $(wildcard $(SRCDIR)/example/*.c)
OBJS_EXAMP := $(SRCS_EXAMP:$(SRCDIR)%.c=$(BUILDDIR)%.o)
LFLAGS_EXAMP = $(LFLAGS_MSP432) -Wl,-Map,$(EXAMPLE).map

rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))
ALL_SRCS := $(call rwildcard,$(SRCDIR)/,*.c)

MAINS = message $(FREERTOS) $(MSGLIB) $(DEMO_M).elf $(DEMO_S).elf $(EXAMPLE).elf

.PHONY: message all clean remake

all: $(MAINS)
	@ echo "Done! :)"

message: 
	@ echo "########################################"
	@ echo "Group M2 Capstone Spring 2018 Build"
	@ echo "########################################"

$(FREERTOS):
	@ $(ECHOBLANKLINE)
	@ $(MAKE) -C $(KERNEL_BUILD) -I$(PROJROOT)
	@ $(ECHOBLANKLINE)

$(MSGLIB): $(OBJS_LIB)
	@ echo AR $@
	@ $(AR) cr $@ $^

##
# I2C Master Demo
$(DEMO_M).elf: $(OBJS_DEMO_M) $(FREERTOS)
	@ echo LNK $@
	@ $(LNK) $(OBJS_DEMO_M) $(LFLAGS_DEMO_M) -o $@

##
# I2C Slave Demo
$(DEMO_S).elf: $(OBJS_DEMO_S) $(FREERTOS)
	@ echo LNK $@
	@ $(LNK) $(OBJS_DEMO_S) $(LFLAGS_DEMO_S) -o $@

##
# Example application
$(EXAMPLE).elf: $(OBJS_EXAMP) $(FREERTOS) $(MSGLIB)
	@ echo LNK $@
	@ $(LNK) $(OBJS_EXAMP) $(MSGLIB) $(LFLAGS_EXAMP) -o $@

##
# C targets for demo applications
$(BUILDDIR)/demo%.o : $(DEMODIR)%.c
	@ echo CC $@
	@ $(call make_dir,"$(dir $@)")
	@ $(CC) $(CFLAGS_MSP432) $(INCLUDES) -c $< -o $@

##
# Generic C targets
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@ $(call make_dir,"$(dir $@)")
	@ echo CC $@
	@ $(CC) -c $(CFLAGS_MSP432) $(INCLUDES) -o $@ $<

clean: message
	@ echo Cleaning
	@ $(RMDIR) $(BUILDDIR)
	$(MAKE) -C $(KERNEL_BUILD) clean -I$(PROJROOT)

remake: clean all
	@ echo Finished remake


-include $(ALL_SRCS:$(SRCDIR)%.c=$(BUILDDIR)%.d)
