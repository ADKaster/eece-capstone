#
# Set location of various cgtools
#
# These variables can be set here or on the command line.
#
# The CCS_ARMCOMPILER, GCC_ARMCOMPILER, and IAR_ARMCOMPILER variables,
# in addition to pointing to their respective locations, also serve
# as "switches" for disabling a build using those cgtools. To disable a
# build using a specific cgtool, either set the cgtool's variable to
# empty or delete/comment-out its definition:
#     IAR_ARMCOMPILER ?=
# or
#     #IAR_ARMCOMPILER ?= c:/Program Files (x86)/IAR Systems/Embedded Workbench 7.5/arm
#
# If a cgtool's *_ARMCOMPILER variable is set (non-empty), various sub-makes
# in the installation will attempt to build with that cgtool.  This means
# that if multiple *_ARMCOMPILER cgtool variables are set, the sub-makes
# will build using each non-empty *_ARMCOMPILER cgtool.
#

include config.mak

XDC_INSTALL_DIR        ?= $(TI_ROOT)ti/xdctools_3_50_02_20_core

FREERTOS_INSTALL_DIR   ?= $(TI_ROOT)FreeRTOSv9.0.0

CCS_ARMCOMPILER        ?= $(TI_ROOT)ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.3.LTS
GCC_ARMCOMPILER        ?= $(TI_ROOT)ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update
IAR_ARMCOMPILER        ?=