#
#
# Extra configuration items

EXTRA_CFLAGS= -DAPPFOO -DFREERTOS -Isrc/example

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

ifeq ("$(SHELL)","sh.exe")
# for Windows/DOS shell
    RM     = del
    RMDIR  = -rmdir /S /Q
    DEVNULL = NUL
    ECHOBLANKLINE = @cmd /c echo.
    TI_ROOT = c:/
else
# for Linux-like shells
    RM     = rm -f
    RMDIR  = rm -rf
    DEVNULL = /dev/null
    ECHOBLANKLINE = echo
    TI_ROOT = $(HOME)/
endif

SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(TI_ROOT)/ti/simplelink_msp432_sdk_1_40_01_00
