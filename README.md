# eece-capstone
Repository to store software work products for capstone.

## Project Overview

The communication library is the Distributed Microcontroller Communication Framework Library (dmcflib for short).

The end goal is to enable an application developer to use the publish-subscribe API presented in the library to easily send messages
between different microcontrollers in a system over I2C. The real demo application will be a telemetry system for an amateur rocket
with a Sensor, Data Logging, Power and Radio Communcations board. The project will be implemented using the TI Simplelink family
of ARM Cortex-M4F microcontrollers, with 3 MSP432s and one CC1310 Sub-1GHz MCU. 

## Configuration

Initial compilation and environment setup was tested using Ubuntu 14.04 LTS in an Oracle 
VirtualBox Virtual Machine, as well as Windows 10.

Theere are two main dependencies for this project. Code Composer Studio, and FreeRTOS.

### Code Composer Studio

The project requires TI's Code Composer Stuido Version 7 (ccsv7). This is necessary for
the cross-compiler as well as TI's SimpleLink SDK.

* Download Code composer studio version 7.x [here](http://processors.wiki.ti.com/index.php/Download_CCS)
* Ensure that Code Composer studio will be installed at ~/ti (or $HOME/ti or /home/<username>/ti) on linux, or C:/ti on Windows.
* Click the two checkboxes for SimpleLink MSP432 and SimpleLink CC13XX support.
* Perform all  other installation steps per TI's wiki as linked above
* On the "Getting Started" page, click the "Browse Examples" option to open the Resource Explorer. Alternatively, if the Resource Explorer is already open, navigate to that tab.
* Open the tree on the left side of the Resource Explorer titled "Software"
* Click the button that looks like a download button and download the MSP432 SDK v:1.60.00.12 "offline"

### FreeRTOS

FreeRTOS is used to manage task scheduling

* Download FreeRTOSv10.0.0 [here](http://www.freertos.org/a00104.html)
* Extract the archive to either your home directory on linux, or c:/ on Windows

### Adding the projects to Code Composer Studio

Steps to start working:
 
 * Clone the git repository to your favorite location

 * Download ccsv7 with MSP432 and CC13xx libraries
 	* /home/user/ti
 	* c:\ti
 
 * Install MSP432 simplelink sdk v1.60.00.12 from resource explorer
 
 * Install FreeRTOSv10.0.0
 	* /home/user/FreeRTOSv10.0.0
 	* c:\FreeRTOSv10.0.0
 
 * Open CCS and create a workspace in the same directory as the git repo
 
 * Go to Window-->Preferences-->General-->Workspace-->Linked Resources
     * New, Name = FREERTOS_INSTALL_DIR, Location = FreeRTOS Install Folder (with the vXX.XX.XX)
	 
 * Go to Project-->Import CCS Projects
     * Click Browse, Ok (Defaults to workspace directory)
	 * Check box next to all projects, click finish

 * Build the MSP432_example project, or your favorite application project	 
	
## Caveats
* Only configured to build for the MSP432. Additional tweaks and setup required to support CC13x0 boards. :(
