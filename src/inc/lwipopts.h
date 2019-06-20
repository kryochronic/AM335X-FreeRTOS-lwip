/**
 * \file lwipopts.h - Configuration options for lwIP
 *
 * Copyright (c) 2010 Texas Instruments Incorporated
 */
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__
#include "app_MultiSinkLogger.h"
#include "inttypes.h"
#include "lwipopts_freertos.h"
/*****************************************************************************
**                           CONFIGURATIONS
*****************************************************************************/
/*
** If Static IP address to be used, give it here. This value shall be 0 if 
** dynamic IP address is to be used.
** For Example, for IP Address 192.168.247.1, use the corresponding hex
** value 0xC0A8F701.
*/
// #define STATIC_IP_ADDRESS_PORT1         0x0A000101ul /* 10.0.1.1 */
#define STATIC_IP_ADDRESS_PORT2         0x0A000102ul /* 10.0.1.2 */

/*
** The macro CPSW_DUAL_MAC_MODE shall be defined for using CPSW ports in 
** Dual MAC mode.
*/
#define CPSW_DUAL_MAC_MODE

/*
** The below macro should be defined for using lwIP with cache. For cache
** enabling, pbuf pool shall be cache line aligned. This is done by using
** separate pool for each memory. The alignment of pbuf pool to cache line 
** size is done in /ports/cpsw/include/arch/cc.h. 
*/
#define LWIP_CACHE_ENABLED              

#define SOC_CACHELINE_SIZE_BYTES        64            /* Number of bytes in
                                                         a cache line */
/*
** The timeout for DHCP completion. lwIP library will wait for DHCP 
** completion for (LWIP_DHCP_TIMEOUT / 100) seconds.
*/
#define LWIP_DHCP_TIMEOUT               500

/*
** The number of times DHCP is attempted. Each time, the library will wait
** for (LWIP_DHCP_TIMEOUT / 100) seconds for DHCP completion.
*/
#define NUM_DHCP_TRIES                  5

/*****************************************************************************
**            lwIP SPECIFIC DEFINITIONS - To be used by lwIP stack
*****************************************************************************/
#define HOST_TMR_INTERVAL               0         
#define DYNAMIC_HTTP_HEADERS

/*****************************************************************************
**                    Platform specific locking 
*****************************************************************************/   
#define NO_SYS                          0          
#define NO_SYS_NO_TIMERS                0

// /*****************************************************************************
// **                          Memory Options
// *****************************************************************************/
// #define MEM_USE_POOLS                   0u
// #define MEM_ALIGNMENT                   4         
// #define MEM_SIZE                        (128 * 1024) /* 128K */

// #define MEMP_NUM_PBUF                   96     
// #define MEMP_NUM_TCP_PCB                32    
// #define PBUF_POOL_SIZE                  210    

// #define MEMP_SEPARATE_POOLS             1            /* We want the pbuf 
//                                                         pool cache line
//                                                         aligned*/
// #endif

/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */
// #define MEMP_NUM_NETBUF                 8
/*****************************************************************************
**                           IP Options
*****************************************************************************/
#define IP_FRAG                         0    
#define LWIP_IPV4                       1      

/*****************************************************************************
**                           DHCP Options
*****************************************************************************/
#define LWIP_DHCP                       1
#define DHCP_DOES_ARP_CHECK             0

/*****************************************************************************
**                           Auto IP  Options
*****************************************************************************/
#define LWIP_AUTOIP                     0            
#define LWIP_DHCP_AUTOIP_COOP           ((LWIP_DHCP) && (LWIP_AUTOIP))

/*****************************************************************************
**                           TCP  Options
*****************************************************************************/
/* Defined in amazon-freertos/lib/third_party/lwip/src/portable/arch/lwipopts_freertos.h */
/*****************************************************************************
**                           PBUF  Options
*****************************************************************************/
/* Defined in amazon-freertos/lib/third_party/lwip/src/portable/arch/lwipopts_freertos.h */

/*****************************************************************************
**                           Socket  Options
*****************************************************************************/
/* Defined in amazon-freertos/lib/third_party/lwip/src/portable/arch/lwipopts_freertos.h */
/*
 * MEMP_NUM_SYS_TIMEOUT: the number of simulateously active timeouts.
 * (requires NO_SYS==0)
 */
#define MEMP_NUM_SYS_TIMEOUT            32
/*****************************************************************************
**                          Debugging options
*****************************************************************************/
#ifdef NETIF_DEBUG
   #undef NETIF_DEBUG
#endif

// #define LWIP_PROVIDE_ERRNO              1u /* Defined in amazon-freertos\lib\third_party\lwip\src\portable/arch/cc.h:62:0*/
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL
#define LWIP_DBG_TYPES_ON               (LWIP_DBG_ON | LWIP_DBG_TRACE \
                                         |LWIP_DBG_STATE | LWIP_DBG_FRESH)

#define LWIP_DEBUG                      1
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL

#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_OFF
#define MEMP_DEBUG                      LWIP_DBG_OFF
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_ON
#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     (LWIP_DBG_ON | LWIP_DBG_TYPES_ON)
#define PPP_DEBUG                       LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define DHCP_DEBUG                      (LWIP_DBG_ON | LWIP_DBG_TYPES_ON)
#define AUTOIP_DEBUG                    (LWIP_DBG_ON | LWIP_DBG_TYPES_ON)
#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                       LWIP_DBG_OFF
#define HTTPD_DEBUG                     LWIP_DBG_OFF
/*****************************************************************************
**                          Netcon options
*****************************************************************************/

/* Defined in lwipopts_freertos.h */

/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/

/* Defined in lwipopts_freertos.h */

/*****************************************************************************
**                       Stats & Misc options
*****************************************************************************/

#define LWIP_STATS                      1
#define LWIP_STATS_DISPLAY              1
#define LWIP_STATS_LARGE                1

#if LWIP_STATS
#define LINK_STATS 1
#define IP_STATS   1
#define IPFRAG_STATS 0
#define ICMP_STATS 0
#define IGMP_STATS 0
#define UDP_STATS  0
#define TCP_STATS  1
#define MEM_STATS  1
#define MEMP_STATS 1
#define SYS_STATS  1
#endif



#ifdef LWIP_DEBUG
   #ifndef configPRINTF
      extern void __error__(char *pcFilename, unsigned long ulLine);
      #define configPRINTF(x)   {SerialisedConsolePrintf x;}
   #endif /* #ifndef configPRINTF */
#else

   #define configPRINTF(X)
   #define LWIP_PLATFORM_ASSERT(expr)
#endif                      


#endif /* __LWIPOPTS_H__ */
