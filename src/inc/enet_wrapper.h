/* 
 * \file   enet_wrapper.h
 *
 * \brief  Wrapper for abstracting TCP/IP(lwip)
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

    Copyright (c) 2019 Abhinav Tripathi.
    All Rights Reserved.
*/
#ifndef ENET_WRAPPER_H
#define ENET_WRAPPER_H
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/
#include "lwiplib.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/

typedef struct enet_task_params_s_t_
{
    LWIP_IF *pLWIP_IF;
}enet_task_params_s_t;

/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/

portTASK_FUNCTION_PROTO(lwip_main,pvParameters);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


#endif /* #ifndef ENET_WRAPPER_H */
