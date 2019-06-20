# AM335X-FreeRTOS-lwip
A port of the popular FreeRTOS with lwip(netconn)
Based on the Cortex A9 port, with replacements for INTC of the AM335X instead of the GIC!
* Fully nested Interrupts!
* Critical Sections working!
* lwip(nteconn) ported, though not rigourously tested!
# Pre-requisites
* CMake
* GNU arm Toolchain (built on gcc-arm-8.2-2019.01-i686-mingw32-arm-eabi)
* Python (2 or 3)
# Optional Tools
* CCS
* VSCode
* Terminal Emulator

# Steps to build
* After a checkout & update of all submodules, navigate to the folder and invoke the _AM335xFreeRTOS_cmake_makefile_args.py_
* This should generate your Cmake Files in the necessary places.
* Configure the _build_ folder with Cmake, use _gcc-a.toolchain.win10.sample.cmake_ as a reference if required.
* navigate to the _build_ folder & make!
