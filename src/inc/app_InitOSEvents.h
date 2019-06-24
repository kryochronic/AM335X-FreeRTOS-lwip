/* 
 * \file   app_InitOSEvents.h
 *
 * \brief  OS Events Init Module for AM335X(CA8 + INTC) Port of FreeRTOS
*/
/*
    Author: Abhinav Tripathi <mr dot a dot tripathi at gmail dot com>

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
    SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE.

    Copyright (c) 2018 Abhinav Tripathi.
    All Rights Reserved.
*/
#ifndef APP_INITOSEVENTS_H
#define APP_INITOSEVENTS_H
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/
#include "stdio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/
#define BIT_0           ((BaseType_t)(1ul <<  0ul))
#define BIT_1           ((BaseType_t)(1ul <<  1ul))
#define BIT_2           ((BaseType_t)(1ul <<  2ul))
#define BIT_3           ((BaseType_t)(1ul <<  3ul))
#define BIT_4           ((BaseType_t)(1ul <<  4ul))
#define BIT_5           ((BaseType_t)(1ul <<  5ul))
#define BIT_6           ((BaseType_t)(1ul <<  6ul))
#define BIT_7           ((BaseType_t)(1ul <<  7ul))

#define BIT_8           ((BaseType_t)(1ul <<  8ul))
#define BIT_9           ((BaseType_t)(1ul <<  9ul))
#define BIT_10          ((BaseType_t)(1ul << 10ul))
#define BIT_11          ((BaseType_t)(1ul << 11ul))
#define BIT_12          ((BaseType_t)(1ul << 12ul))
#define BIT_13          ((BaseType_t)(1ul << 13ul))
#define BIT_14          ((BaseType_t)(1ul << 14ul))
#define BIT_15          ((BaseType_t)(1ul << 15ul))

#define BIT_16          ((BaseType_t)(1ul << 16ul))
#define BIT_17          ((BaseType_t)(1ul << 17ul))
#define BIT_18          ((BaseType_t)(1ul << 18ul))
#define BIT_19          ((BaseType_t)(1ul << 19ul))
#define BIT_20          ((BaseType_t)(1ul << 20ul))
#define BIT_21          ((BaseType_t)(1ul << 21ul))
#define BIT_22          ((BaseType_t)(1ul << 22ul))
#define BIT_23          ((BaseType_t)(1ul << 23ul))

#define BIT_24          ((BaseType_t)(1ul << 24ul))
#define BIT_25          ((BaseType_t)(1ul << 25ul))
#define BIT_26          ((BaseType_t)(1ul << 26ul))
#define BIT_27          ((BaseType_t)(1ul << 27ul))
#define BIT_28          ((BaseType_t)(1ul << 28ul))
#define BIT_29          ((BaseType_t)(1ul << 29ul))
#define BIT_30          ((BaseType_t)(1ul << 30ul))
#define BIT_31          ((BaseType_t)(1ul << 31ul))

/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/


#define PRIORITY_LOGGER                              1uL
#define PRIORITY_LWIP                                2uL
#define PRIORITY_LWIP_INT_HLR                        2uL
#define PRIORITY_TASK_LED_23                         5uL
#define PRIORITY_TASK_LED_24                         5uL
#define pdAPP_TASK_STACK_SIZE_1KW_UL         (4ul*1024ul)   //1K Stack Entries, 4 Bytes / Entry

/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/
typedef struct app_InitOSEventsList_DSType_
{
    QueueHandle_t pLoggerSinkUartQ;

}app_InitOSEventsList_DSType;


/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/
uint8_t app_InitOSEvents(void);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/

extern app_InitOSEventsList_DSType app_InitOSEventsList_DS;
extern EventGroupHandle_t xApplicationEventGroup_001;

/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/

#endif /* #ifndef APP_INITOSEVENTS_H */
