/* 
 * \file   app_MultiSinkLogger.c
 *
 * \brief  Logging Module for AM335X(CA8 + INTC) Port of FreeRTOS
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
#ifndef APP_MULTISINKLOGGER_C
#define APP_MULTISINKLOGGER_C
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/

#include "app_MultiSinkLogger.h"

#include <stdarg.h>
#include <stdio.h>

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include <string.h>

#include "inc/aws_bufferpool.h"
#include "inc/aws_bufferpool_config.h"
#include "enet_wrapper.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/

uint32_t SerialisedLogPrintf(QueueHandle_t InterafceQ,void * msg, va_list arg);

char * prvPrefixStringToMessage(char * msg);
char * prvPrefixDebugStringToMessage(char * msg,unsigned long ulLine, const char * const pcFileName,const char * const pcFuncName);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/
QueueHandle_t ConsoleInterafceQ;
/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          CODE                                                          */
/**************************************************************************************************************************/
portTASK_FUNCTION(vMultiSinkLoggerConsole_main,pvParameters)
{
    uint32_t Counter = 0;
    AppMultiSinkLogTaskParam_DSType * pParams = (AppMultiSinkLogTaskParam_DSType *)pvParameters;
    ConsoleInterafceQ = pParams->InQ;
    for(;;)
    {
        uint32_t rv = pdFAIL;
        const char * pMsg;
        uint8_t ipv4[4];
        enet_task_params_s_t * p_enet_task_params_s = (enet_task_params_s_t*)pParams->p_enet_task_params_s;
        // LWIP_IF* pLWIP_IF = p_enet_task_params_s->pLWIPIF;
        memcpy(&ipv4,&p_enet_task_params_s->pLWIP_IF->ipAddr,4);
        rv = xQueueReceive(pParams->InQ,&pMsg,portMAX_DELAY);
        if(pdPASS == rv)
        {
            uint32_t m     = bufferpoolconfigBUFFER_SIZE;
            char * q       = pMsg;
            pParams->pInterface("\n[%08X ",Counter ++);
            pParams->pInterface("%d.%d.%d.%d]",ipv4[0],ipv4[1],ipv4[2],ipv4[3]);
            while(*q && (m > 0))
            {
                if(('\n' == *q) || ('\r' == *q))
                    *q = ';';
                q ++;
                m --;
            }
            pParams->pInterface("%s",pMsg);
            BUFFERPOOL_ReturnBuffer((uint8_t *)pMsg);

        }
    }
}
#if 0
static uint32_t app_MultiSinkLogHook(void * msg,size_t n,QueueHandle_t InterafceQ)
{
    uint32_t rv = pdFAIL;
    uint32_t m     = n;
    uint8_t * pMsg = BUFFERPOOL_GetFreeBuffer(&m);
    if((void *)NULL != pMsg)
    {
        memcpy(pMsg,msg,n);
        rv = xQueueSend(InterafceQ,&pMsg,0);
        if(pdTRUE != rv)
        {
            /* Send the memory chunk back to the Pool */
            BUFFERPOOL_ReturnBuffer(pMsg);
            rv = pdFAIL;
        }
    }
    return rv;
}
#endif

uint32_t InitSerialisedLogPrintf(QueueHandle_t q)
{
    return ConsoleInterafceQ = q;
}



/* This task replaces the Message String with the task name prefixed to the same string and replaces the buffer */
char * prvPrefixStringToMessage(char * msg)
{
    char *pcTaskName = pcTaskGetName(NULL);
    uint32_t rv = pdFAIL;
    uint32_t m     = bufferpoolconfigBUFFER_SIZE;
    char * pMsg = (char *)BUFFERPOOL_GetFreeBuffer(&m);
    if((void *)NULL != pMsg)
    {
        snprintf(pMsg,m,"%s:%s",pcTaskName,msg);
        
    }
    return pMsg;
}

/* This task replaces the Message String with the task name prefixed to the same string and replaces the buffer */
char * prvPrefixDebugStringToMessage(char * msg,unsigned long ulLine, const char * const pcFileName,const char * const pcFuncName)
{
    char *pcTaskName = pcTaskGetName(NULL);
    uint32_t rv = pdFAIL;
    uint32_t m     = bufferpoolconfigBUFFER_SIZE;
    char * pMsg = (char *)BUFFERPOOL_GetFreeBuffer(&m);
    if((void *)NULL != pMsg)
    {
        snprintf(pMsg,m,"%s:%s:%s:%d:%s",pcTaskName,pcFileName,pcFuncName,ulLine,msg);
    }
    return pMsg;
}

uint32_t SerialisedConsolePrintfDebug(unsigned long ulLine, const char * const pcFileName,const char * const pcFuncName,char * msg,...)
{
    va_list arg;
    uint32_t rv = pdFAIL;
    /* Start the variable arguments processing. */
    char* pMsg = prvPrefixDebugStringToMessage(msg,ulLine,pcFileName,pcFuncName); /* Returns a new buffer from Buffer Pool */
    if(NULL != pMsg)
    {
        va_start (arg, msg);
        rv = SerialisedLogPrintf(ConsoleInterafceQ,pMsg,arg);
        /* End the variable arguments processing. */
        va_end(arg);
        BUFFERPOOL_ReturnBuffer((uint8_t *)pMsg);
    }
    return rv;
}
uint32_t SerialisedConsolePrintf(char * msg,...)
{
    va_list arg;
    uint32_t rv = pdFAIL;
    /* Start the variable arguments processing. */
    char* pMsg = prvPrefixStringToMessage(msg); /* Returns a new buffer from Buffer Pool */
    if(NULL != pMsg)
    {
        va_start (arg, msg);
        rv = SerialisedLogPrintf(ConsoleInterafceQ,pMsg,arg);
        /* End the variable arguments processing. */
        va_end(arg);
        BUFFERPOOL_ReturnBuffer((uint8_t *)pMsg);
    }
    return rv;
}

uint32_t SerialisedLogPrintf(QueueHandle_t InterafceQ,void * msg, va_list arg)
{
    /* Start the variable arguments processing. */
    uint32_t rv = pdFAIL;
    uint32_t m     = bufferpoolconfigBUFFER_SIZE;
    char * pMsg = BUFFERPOOL_GetFreeBuffer(&m);
    portBASE_TYPE xHigherPriorityTaskHasWoken = pdFALSE;
    extern uint32_t ulPortInterruptNesting;
    extern uint32_t ulPortYieldRequired;
    if((void *)NULL != pMsg)
    {
        vsnprintf(pMsg,m,msg,arg);
        if(ulPortInterruptNesting == 0)
            rv = xQueueSend(InterafceQ,&pMsg,0);
        else
        {
            rv = xQueueSendFromISR(InterafceQ,&pMsg,&xHigherPriorityTaskHasWoken);
            if(xHigherPriorityTaskHasWoken)
                    ulPortYieldRequired = pdTRUE;
            
        }
        if(pdTRUE != rv)
        {
            /* Send the memory chunk back to the Pool */
            BUFFERPOOL_ReturnBuffer(pMsg);
            rv = pdFAIL;

        }
    }
    return rv;
}

#endif /* #ifndef APP_MULTISINKLOGGER_C */
