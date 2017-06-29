# eece-capstone
Repository to store software work products for capstone

## Configuration

Initial compilation and environment setup was done using Ubuntu 14.04 LTS in an Oracle 
VirtualBox Virtual Machine.

Theere are two main dependencies for this project. Code Composer Studio, and FreeRTOS.

### Code Composer Studio

The project requires TI's Code Composer Stuido Version 7 (ccsv7). This is necessary for
the cross-compiler as well as TI's SimpleLink SDK.

* Download Code composer studio [here](http://processors.wiki.ti.com/index.php/Download_CCS)
* Ensure that Code Composer studio will be installed at ~/ti (or $HOME/ti or /home/<username>/ti)
* Perform all installation steps per TI's wiki as linked above
* On the "Getting Started" page, click the "Browse Examples" option to open the Resource Explorer. Alternatively, if the Resource Explorer is already open, navigate to that tab.
* Open the tree on the left side of the Resource Explorer titled "Software"
* Click on the name text (not the folder) for "SimpleLink MSP432 SDK - v:1.30.00.40" If a newer version is available, use version 1.30.00.40 instead.
* The right pane should show the 


