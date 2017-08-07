# eece-capstone
Repository to store software work products for capstone

## Configuration

Initial compilation and environment setup was tested using Ubuntu 14.04 LTS in an Oracle 
VirtualBox Virtual Machine, as well as Windows 10.

Theere are two main dependencies for this project. Code Composer Studio, and FreeRTOS.

### Code Composer Studio

The project requires TI's Code Composer Stuido Version 7 (ccsv7). This is necessary for
the cross-compiler as well as TI's SimpleLink SDK.

* Download Code composer studio version 7.2 [here](http://processors.wiki.ti.com/index.php/Download_CCS)
* Ensure that Code Composer studio will be installed at ~/ti (or $HOME/ti or /home/<username>/ti) on linux, or C:/ti on Windows.
* Click the two checkboxes for SimpleLink MSP432 and SimpleLink CC13XX support.
* Perform all  other installation steps per TI's wiki as linked above
* On the "Getting Started" page, click the "Browse Examples" option to open the Resource Explorer. Alternatively, if the Resource Explorer is already open, navigate to that tab.
* Open the tree on the left side of the Resource Explorer titled "Software"
* Click the button that looks like a download button and download the MSP432 SDK v:1.40.01.00 "offline"
* Do the same for the "SimpleLink CC13x0 SDK - v:1.40.01.00"  

### FreeRTOS

FreeRTOS is used to manage task scheduling

* Download FreeRTOSv9.0.0 [here](http://www.freertos.org/a00104.html)
* Extract the archive to either your home directory on linux, or c:/ on Windows

### Adding the project to Code Composer Studio

* Click the "new" icon in the top left of CCS, directly under the "file" menu
* In the window that pops up, expand the C/C++ folder and click the option "Makefile Project with Existing Code"
* Give the project a descriptive name, and select the exisitng code location as the folder for this project (e.g. C:\Users\Andrew\Git\eece-capstone, or /home/andrew/git/eece-capstone)


TODO Clean up these steps

 Steps to start working:
 
 * Download ccsv7 with MSP432 and CC13xx libraries
 	* /home/user/ti
 	* c:\ti
 
 * Install msp432 simplelink sdk v1.40.01.00 from resource explorer
 
 * Install FreeRTOSv9.0.0
 	* /home/user/FreeRTOSv9.0.0
 	* c:\FreeRTOSv9.0.0
 
 * Create new workspace
 
 * Create new "makefile project with existing code"
 
 * Select git clone'd directory, call it whatever you want
 	* C/C++
 	* Cross GCC toolchain
 
### Configure Content Discovery Tool

TODO Change to reccomend adding project variables for FreeRTOS root, simplelinksdk root instead of absolute path
 
 * Right click project (top level blue folder in project explorer that says <NAME> [eece-capstone]) and open properties
 	* ** WINDOWS ONLY ** C/C++ Build
 		* Uncheck Use default build command
 		* Type new build command as ${ccs_install_root}/utils/bin/gmake.exe
 		* Apply
 	* C/C++ General -> Paths and Symbols
 		* Includes, Click GNU C
 			* Add Workspace path
 				* src/FreeRTOS
 	* C/C++ General -> Preprocesssor Include Paths, Macros etc.
 		* Providers, click on CDT Cross GCC Built-in Compiler Settings
 			* Command to get compiler specs, prepend exitsting text with:
 				* ${ccs_install_root}/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-
 			* final should look like:
 				* ${ccs_install_root}/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-${COMMAND} ${FLAGS} -E -P -v -dD "${INPUTS}"
 		* Entries, click GNU C, click CDT User Setting Entries
 			* Add, Include Directory, Filesystem Path, ****Contains System headers****
 				* C:\FreeRTOSv9.0.0\FreeRTOS\Source\include
 				* C:\FreeRTOSv9.0.0\FreeRTOS\Source\portable\GCC\ARM_CM4F
 				* C:\ti\simplelink_msp432_sdk_1_40_01_00\source
 				* C:\ti\simplelink_msp432_sdk_1_40_01_00\source\third_party\CMSIS\Include
 				* C:\ti\simplelink_msp432_sdk_1_40_01_00\kernel\freertos\posix
 		* Entries, click GNU C, click CDT User Setting Entries
 			* Add, Preprocessor Macro from dropdown. Value of 1
 				* FREERTOS 
 				* __MSP432P401R\_\_
 				* NOAPPLICATION 
 			* NOTE: Need to define specific application when that functionality is added
 
 * Open <worskspace>/.metadata/.plugins/org.eclipse.cdt.core/<ProjectName>.language.settings.xml
 	* Find and remove XML entries for
 		* __ARM_FEATURE_FMA
 		* __ARM_FP
 		* __ARM_NEON
 		* __ARM_NEON\_\_
 		* __ARM_NEON_FP
 	* OR: Figure out what the heck these are for and why they show up and come up with a better fix
 
 * Right click project, Close Project
 	* Double click on project to open with refreshed CDT settings :)

## Caveats
* Only configured to build for the MSP432. Additional makefile tweaks required for CC13x0 boards. :(

