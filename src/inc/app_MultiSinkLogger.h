/* 
 * \file   app_MultiSinkLogger.h
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
#ifndef APP_MULTISINKLOGGER_H
#define APP_MULTISINKLOGGER_H
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/
#include "FreeRTOS.h"
#include "queue.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/
#define NAV_APP_LOG_PRINTF(...)                  SerialisedConsolePrintfDebug(__LINE__,__FILE__,__FUNCTION__,__VA_ARGS__)
/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/
typedef struct AppMultiSinkLogTaskParam_DSType_
{
	QueueHandle_t InQ;
    void (*pInterface)(const char *string, ...);
    void (*EntryProgram) (void);
    void *p_enet_task_params_s;
}  AppMultiSinkLogTaskParam_DSType;

/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/
portTASK_FUNCTION_PROTO( vMultiSinkLoggerConsole_main, pvParameters );
uint32_t app_MultiSinkLogHook(void * msg,size_t n,QueueHandle_t InterafceQ);
uint32_t InitSerialisedLogPrintf(QueueHandle_t q);
uint32_t SerialisedConsolePrintf(char * msg,...);

uint32_t SerialisedConsolePrintfDebug(unsigned long ulLine, const char * const pcFileName,const char * const pcFuncName,char * msg,...);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


#endif /* #ifndef APP_MULTISINKLOGGER_H */
