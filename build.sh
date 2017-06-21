#!/usr/bin/env bash

echo "########################################"
echo "Group M2 Capstone Spring 2018 Building ..."
echo "########################################"
if [[ $# -eq 0 ]] ; then
    echo "No arguments supplied, defaulting to 'all'"
    BUILD="all"
else
    BUILD=$1
fi

FREERTOS_DIR=$HOME/ti/simplelink_msp432_sdk_1_30_00_40/kernel/freertos/builds/MSP_EXP432P401R/release/gcc
FREERTOS_LIB=$FREERTOS_DIR/freertos.lib

if [ $BUILD == "all" ] || [ $BUILD == "remake" ]; then
	if [ ! -f $FREERTOS_LIB ] ; then
		echo $FREERTOS_LIB does not exist
		pushd $FREERTOS_DIR
		make
		popd
	fi
	echo "Building ${BUILD}"
	mkdir -p build
	touch build/build.out
	make $BUILD 2>&1 | tee build/build.out
	echo -e "########################################\n"
    echo "Checking for errors ..."
    if grep -q error: build/build.out ; then
        grep error: build/build.out
    else
        echo "No errors"
    fi
    echo -e "\nChecking for warnings ..."
    if grep -q warning: build/build.out ; then
        grep warning: build/build.out
    else
        echo "No warnings"
    fi
elif [ $BUILD == "clean" ] ; then
	echo "Cleaning src"
	make $BUILD
	pushd $FREERTOS_DIR
	make clean
	popd
else
	echo "Invalid option. Candidates are: all clean remake"
fi

echo ""
