#
#
# Extra configuration items

EXTRA_CFLAGS= -DNOAPPLICATION

SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(HOME)/ti/simplelink_msp432_sdk_1_40_00_28

ifeq ($(OS),Windows_NT)
	os := Windows
define make_dir
	if not exist $(1) mkdir $(1)
endef 
else
	MSOFTCHK = $(shell grep Microsoft /proc/version)
	ifneq ($(MSOFTCHK),)
		#Why are you calling this from bash in Win10? Making life difficult :/
		SHELL := cmd
	endif
define make_dir
	mkdir -p $(1)
endef
endif
