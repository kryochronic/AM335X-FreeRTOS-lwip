# AM335X-FreeRTOS-lwip netconn API with FDS
A port of the popular FreeRTOS with lwip(netconn)
Based on the Cortex A9 port, with replacements for INTC of the AM335X instead of the GIC!
* Fully nested Interrupts!
* Critical Sections working!
* lwip(nteconn) ported, though not rigourously tested!
## Pre-requisites
* CMake
* GNU arm Toolchain (built on gcc-arm-8.2-2019.01-i686-mingw32-arm-eabi)
* Python (2 or 3)
## Optional Tools
* CCS
* VSCode
* Terminal Emulator

## Steps to build
* After a checkout & update of all submodules, navigate to the folder and invoke the _AM335xFreeRTOS_cmake_makefile_args.py_
* This should generate your Cmake Files in the necessary places.
* Configure the _build_ folder with Cmake, use _gcc-a.toolchain.win10.sample.cmake_ as a reference if required.
* navigate to the _build_ folder & make!

## Dealing with the lwip netconn and the SEND/RECV_MINUS/PLUS kerfuffle
The lwip netconnn APIs while charming, are a bit of kerfuffle with the NETCONN events aka RECV_PLUS/MINUS & SEND_PLUS/MINUS.
The events them selves are state-less & have to be added to context of the event to make sense of them.
Below is an attempt on the classification of the same for a 3-thread model using the FDS, for a tcp client:
`Inputs	len	Type	API	From	Context	WRITE_ENABLED			Resulting_Context	FLAGS-OUTPUTS`

|Inputs|||||||Outputs|||
|---|---|---|---|---|---|---|---|---|---|---|---|
**Context**|**Netconn Event**|**len**|**Type**|**API**|**From**|**WRITE_ENABLED**|**Resulting_Context**|**Read/Wrtite Thread Enable?**|**lwip_state**

**Context**|**Netconn Event**|**len**|**Type**|**API**|**From**|**WRITE_ENABLED**|**Resulting_Context**|**Read/Wrtite Thread Enable?**|**lwip_state**
|---|---|---|---|---|---|---|---|---|---|---|---|
SHUTDN|R+|0|Client-TCP|netconn|`lwip::err_tcp`|0|SHUTDN|NA|ERR
SHUTDN|RESET|0|Client-TCP|App|`Application`|0|IDLE|NA|IDLE
IDLE|S+|0|Client-TCP|all|`lwip::lwip_netconn_do_connected`|0|RUNNING|TRC_TX_HANDLER_ACTIVATE|RUNNING
IDLE|ERR|0|Client-TCP|App|`Application`|0|SHUTDN|NA|ERR
RUNNING|ERR|0|Client-TCP|netconn|`lwip::err_tcp`|0|SHUTDN|NA|ERR
RUNNING|ERR|0|Client-TCP|netconn|`lwip::lwip_netconn_do_close_internal`|0|SHUTDN|NA|ERR
RUNNING|R-|0|Client-TCP|netconn|`lwip::netconn_recv_data_tcp`|0|SHUTDN|NA|CLOSE::RX
RUNNING|R+|0|Client-TCP|netconn|`lwip::lwip_netconn_do_delconn`|0|SHUTDN|NA|CLOSE
RUNNING|R+|0|Client-TCP|netconn|`lwip::lwip_netconn_do_close_internal`|0|SHUTDN|NA|CLOSE::RX
RUNNING|R+|0|Client-TCP|netconn|`lwip::err_tcp`|0|SHUTDN|NA|ERR
RUNNING|S-|0|Client-TCP|netconn|`lwip::lwip_netconn_do_writemore`|1|RUNNING|NA|HOLD WRITE
RUNNING|S-|0|Client-TCP|netconn|`lwip::lwip_netconn_do_writemore`|1|RUNNING|NA|HOLD WRITE
RUNNING|S+|0|Client-TCP|common|`poll_tcp`|0|RUNNING|TRC_TX_HANDLER_ACTIVATE|WRITE MORE
RUNNING|S+|0|Client-TCP|netconn|`lwip::lwip_netconn_do_close_internal`|1|SHUTDN|NA|CLOSE::TX
RUNNING|S+|0|Client-TCP|netconn|`lwip::lwip_netconn_do_delconn`|1|SHUTDN|NA|CLOSE
RUNNING|S+|0|Client-TCP|netconn|`err_tcp`|1|SHUTDN|NA|CLOSED::TX
RUNNING|S+|1|Client-TCP|netconn|`lwip::sent_tcp`|0|RUNNING|TRC_TX_HANDLER_ACTIVATE|WRITE MORE
RUNNING|R-|1|Client-TCP|netconn|`lwip::netconn_recv_data`|X|RUNNING|TRC_RX_HANDLER_ACTIVATE|Data Arrived
RUNNING|R+|1|Client-TCP|netconn|`lwip::recv_tcp`|X|RUNNING::Data Arrived|TRC_RX_HANDLER_ACTIVATE|Data Arrived
RUNNING|R+|1|Client-TCP|raw|`lwip::recv_raw`|X|RUNNING::Data Arrived|TRC_RX_HANDLER_ACTIVATE|Data Arrived
RUNNING|R+|1|Client-TCP|netconn|`lwip::recv_udp`|X|RUNNING::Data Arrived|TRC_RX_HANDLER_ACTIVATE|Data Arrived
|||||``||||
RUNNING|R+|0|Server-TCP|socket|`lwip::accept_function`|X|ERR|NotMapped|NotMapped
RUNNING|R+|0|Server-TCP|socket|`lwip::accept_function`|X|ERR|NotMapped|NotMapped
RUNNING|R+|0|Server-TCP|netconn|`lwip::netconn_accept`|X|ERR|NotMapped|NotMapped
RUNNING|R-|0|Server-TCP|netconn|`lwip::netconn_accept`|X|ACCEPTED|NotMapped|NotMapped
RUNNING|R+|0|Server-TCP|socket|`lwwip::lwip_accept`|X|??|NotMapped|NotMapped