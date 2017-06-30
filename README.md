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
* Click on the name text (not the folder) for "SimpleLink MSP432 SDK - v:1.40.00.28" If a newer version is available, use version 1.40.00.28 instead.
* The right pane should show the 


### FreeRTOS

FreeRTOS is used to manage task scheduling

* Download FreeRTOSv9.0.0 [here](http://www.freertos.org/a00104.html)
* Extract the archive to either your home directory on linux, or c:/ on Windows


## Adding the project to Code Composer Studio

* Click the "new" icon in the top left of CCS, directly under the "file" menu
* In the window that pops up, expand the C/C++ folder and click the option "Makefile Project with Existing Code"
* Give the project a descriptive name, and select the exisitng code location as the folder for this project (e.g. C:\Users\Andrew\Git\eece-capstone, or /home/andrew/git/eece-capstone)
* Open the project explorer on the left side-bar
* Right click the name of your workspace project and open the preferences (alt+enter)
* Click on 'C/C++ Build' and uncheck the box labeled 'Use default build command'
* In the build command text box, change the command to ${workspace_loc:/<project name>/build.sh}
* Click apply, then ok
* Happy buidling!
