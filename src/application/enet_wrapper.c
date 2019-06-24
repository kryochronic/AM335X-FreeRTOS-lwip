/* 
 * \file   enet_wrapper.c
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
#ifndef ENET_WRAPPER_C
#define ENET_WRAPPER_C
/**************************************************************************************************************************/
/*                                                     INCLUDE FILES                                                      */
/**************************************************************************************************************************/

#define DHCP_TIMEOUT 10u


#include "consoleUtils.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "beaglebone.h"
#include "delay.h"
#include "mmu.h"
#include "cache.h"
#include "phy.h"
#include "app_InitOSEvents.h"
#include "app_utils.h"

#include "ports/cpsw/include/lwiplib.h"
#include "ports/cpsw/include/netif/cpswif.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h" /* Needed for DHCP Enums */
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "netif/cpswif.h"

#include "FreeRTOS.h"
#include "event_groups.h"

#include "enet_wrapper.h"

/**************************************************************************************************************************/
/*                                                     CONFIGURATIONS                                                     */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                         MACROS                                                         */
/**************************************************************************************************************************/

#define LEN_IP_ADDR                        (4u)
#define ASCII_NUM_IDX                      (48u) 


#define CONFIG_SWITCH_EXIT_CMD             (2)

#define DEF_SLV_PORT                       (1)
#define DEF_AUTONEG                        (1)
#define DEF_PHY_CONFIG                     (0x3f)
#define DEF_SPEED                          (1)
#define DEF_DUPLEX                         (1)


/**************************************************************************************************************************/
/*                                                        LITERALS                                                        */
/**************************************************************************************************************************/
/* Used by lwipInvokeHandler - START */
#define OS_FLAG_SET                 pdTRUE
#define OS_FLAG_CLR                 pdFALSE
/* Used by lwipInvokeHandler - END   */


#define LWIP_FLAGS_RX_INT_HLR_EN    BIT_0
#define LWIP_FLAGS_TX_INT_HLR_EN    BIT_1
#define LWIP_FLAGS_LMK_STATE        BIT_2
/**************************************************************************************************************************/
/*                                                       DATA TYPES                                                       */
/**************************************************************************************************************************/

typedef struct lwip_interrupt_handler_task_params_s_
{
    EventGroupHandle_t   group;
    EventBits_t          wait_bits;
    void (*pHandler)(uint32_t);
    uint32_t inst_num;
    const char * name;
    uint32_t irq_chan;

}lwip_interrupt_handler_task_params_s;
/**************************************************************************************************************************/
/*                                                  FUNCTION PROTOTYPES                                                   */
/**************************************************************************************************************************/
static void CPSWCore0RxIsr(void);
static void CPSWCore0TxIsr(void);
static void AintcCPSWIntrSetUp(void);
static void IpAddrDisplay(unsigned int ipAddr);


void InitLwipStack(void * arg);
/*
 * FreeRTOS hook (or callback) functions that are defined in this file.
 */

static EventGroupHandle_t create_lwip_EventGroup(void);

void enet_lwip_init_handler_params(lwip_interrupt_handler_task_params_s * pTaskParams,EventGroupHandle_t group, EventBits_t wait_bits, void *pHandler, uint32_t inst_num, const char * name,uint32_t chan);

void enet_lwip_create_handler(void(*pTask)(void *),void * pParams);

void lwipInvokeHandler(EventGroupHandle_t xEventGroup,EventBits_t ev,uint8_t SetReset);

static void lwIPDHCPComplete(unsigned int ifNum);


/* Lwip Handlers as tasks */

portTASK_FUNCTION_PROTO(lwip_cpsw_rx_tx_handler,pvParameters);

static void tcpecho_thread(void *arg);

void enet_lwip_netif_link_callback(struct netif *netif);

/**************************************************************************************************************************/
/*                                                    GLOBAL VARIABLES                                                    */
/**************************************************************************************************************************/


LWIP_IF lwipIfPort1, lwipIfPort2;
lwip_interrupt_handler_task_params_s lwip_cpsw_handler_task_params_rx;
lwip_interrupt_handler_task_params_s lwip_cpsw_handler_task_params_tx;
volatile EventGroupHandle_t lwip_cpsw_evflags_group;

#ifdef CPSW_DUAL_MAC_MODE
    static struct netif cpswNetIF[MAX_CPSW_INST * MAX_SLAVEPORT_PER_INST];
#else
    static struct netif cpswNetIF[MAX_CPSW_INST];
#endif

/*
** Helper to identify ports
*/
static struct cpswportif cpswPortIf[MAX_CPSW_INST * MAX_SLAVEPORT_PER_INST];

/**************************************************************************************************************************/
/*                                                          LUTS                                                          */
/**************************************************************************************************************************/


/**************************************************************************************************************************/
/*                                                          CODE                                                          */
/**************************************************************************************************************************/




void InitBspEthHW(void)
{
    CPSWPinMuxSetup();
    CPSWClkEnable();

    /* Chip configuration RGMII selection */
    EVMPortMIIModeSelect();

    /* Get the MAC address */
    EVMMACAddrGet(0, lwipIfPort1.macArray); 
    EVMMACAddrGet(1, lwipIfPort2.macArray); 

    AintcCPSWIntrSetUp();
}
#if 0
uint32_t AcquireIP(LWIP_IF *plwipIfPort)
{
    unsigned int ipAddr;
    ip4_addr_t addr;
    ip4_addr_t netmask;
    ip4_addr_t gw;
    IP4_ADDR(&addr, 84, 115, 30, 172);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 1, 115, 30, 172);
#if STATIC_IP_ADDRESS_PORT1

    plwipIfPort->instNum = 0;
    plwipIfPort->slvPortNum = 1; 
    plwipIfPort->ipAddr  = addr.addr; 
    plwipIfPort->netMask = netmask.addr; 
    plwipIfPort->gwAddr  = gw.addr;
    plwipIfPort->ipMode = IPADDR_USE_STATIC; 

#else

    plwipIfPort->instNum = 0;
    plwipIfPort->slvPortNum = 1; 
    plwipIfPort->ipAddr = 0; 
    plwipIfPort->netMask = 0; 
    plwipIfPort->gwAddr = 0; 
    plwipIfPort->ipMode = IPADDR_USE_DHCP; 

#endif

    ipAddr = lwIPInit(plwipIfPort);
    if(ipAddr)
    {
        NAV_APP_LOG_PRINTF("\n\r\n\rPort 1 IP Address Assigned: ");
        IpAddrDisplay(ipAddr);
    }
    else
    {
        NAV_APP_LOG_PRINTF("\n\r\n\rPort 1 IP Address Acquisition Failed.");
    }
    
    return ipAddr;
}
#endif
void InitLwipStack(void * arg)
{
    LWIP_IF *plwipIfPort = (LWIP_IF *)arg;
    ip_addr_t _ip_addr;
    ip_addr_t _net_mask;
    ip_addr_t _gw_addr;
    #if 0
    unsigned int temp;
    unsigned int *ipAddrPtr;
    static unsigned int lwipInitFlag = 0;
    #endif
    unsigned int ifNum;
    
#ifdef CPSW_DUAL_MAC_MODE
    ifNum = (plwipIfPort->instNum * MAX_SLAVEPORT_PER_INST) + plwipIfPort->slvPortNum - 1;
#else
    ifNum = lwipIf->instNum;
#endif
    
    struct netif *pDefaultNetif = &cpswNetIF[ifNum];
    struct dhcp * pdhcp;
    /* Setup Ethernet Phy Port - START*/
    cpswPortIf[ifNum].inst_num = plwipIfPort->instNum;
    cpswPortIf[ifNum].port_num = plwipIfPort->slvPortNum;
    /* set MAC hardware address */
    for(uint32_t temp = 0; temp < LEN_MAC_ADDRESS; temp++) 
    {
        cpswPortIf[ifNum].eth_addr[temp] =
                         plwipIfPort->macArray[(LEN_MAC_ADDRESS - 1) - temp];
    }
    /* Setup Ethernet Phy Port - END*/
    /* Add a network interface to the list of lwIP netifs. */
    // Setup IP Config for DHCP ...
    // IP4_ADDR(&_ip_addr, 10,1,1,2);
    // IP4_ADDR(&_net_mask, 255,0,0,0);
    // IP4_ADDR(&_gw_addr, 10,1,1,1);
    // IP4_ADDR(&_ip_addr, 0,0,0,0);
    // IP4_ADDR(&_net_mask, 255,255,255,255);
    // IP4_ADDR(&_gw_addr, 10,1,1,1);
#if enet_lwip_netif_link_callback
    pDefaultNetif->link_callback = enet_lwip_netif_link_callback;
#endif
    while(pdTRUE)
    {
        if(NULL == netif_add(pDefaultNetif, &_ip_addr, &_net_mask, &_gw_addr,&cpswPortIf[ifNum], cpswif_init, ip_input))
        {
            LWIP_PRINTF("\n\rUnable to add interface for interface %d", ifNum);
            LWIP_PRINTF("\n\rPerforming PHY Reset for interface %d", ifNum);
            uint8_t rv = PhyReset(CPSW0_MDIO_REGS, ifNum);

        }
        else
        {
            LWIP_PRINTF("\n\rnetif_add Succesful to add interface for interface %d", ifNum);
            break;
        }
    }
    
    /*
    If your port does not support link down/up events, you also need to call 
    netif_set_linkup() on your netif, but you'll lose the link status 
    signaling which makes DHCP fast.
        -- https://lists.gnu.org/archive/html/lwip-users/2015-09/msg00011.html
    */

    netif_set_link_up(pDefaultNetif); 
    /* Set the network interface as the default network interface. */
    netif_set_default(pDefaultNetif);
    
    /* netif up = software administrative state, whether interface should be 
           used for routing or not
    */
    
    /* Make netif active ... and proceed for DHCP*/
    netif_set_up(pDefaultNetif);

#if LWIP_DHCP
    // if(plwipIfPort->ipMode == IPADDR_USE_DHCP)
    // {
    //     lwIPDHCPComplete(ifNum);
    // }
    /* obtain the IP address, default gateway and subnet mask by using DHCP*/
    uint8_t err = dhcp_start(pDefaultNetif);
    NAV_APP_LOG_PRINTF("%s : Started DCHP request (%s)", __FUNCTION__, lwip_strerr(err));
    
    for(int i=0; i < DHCP_TIMEOUT; i++)
    {
        
        pdhcp = netif_dhcp_data(pDefaultNetif);
        if(DHCP_STATE_BOUND == pdhcp->state)
            break;

        NAV_APP_LOG_PRINTF("%s : Current DHCP State : %d", __FUNCTION__, pdhcp->state);
        // Wait a second
        Delay_HMSM(0,0,1,0);
    }
#endif
    NAV_APP_LOG_PRINTF("%s : Interface is up : %d", __FUNCTION__, pdhcp->state);
    NAV_APP_LOG_PRINTF("%s : IP %s", __FUNCTION__, ipaddr_ntoa(&(pDefaultNetif->ip_addr)));
    NAV_APP_LOG_PRINTF("%s : NM %s", __FUNCTION__, ipaddr_ntoa(&(pDefaultNetif->netmask)));
    NAV_APP_LOG_PRINTF("%s : GW %s", __FUNCTION__, ipaddr_ntoa(&(pDefaultNetif->gw)));
    plwipIfPort->gwAddr  = pDefaultNetif->gw.addr;
    plwipIfPort->ipAddr  = pDefaultNetif->ip_addr.addr;
    plwipIfPort->netMask = pDefaultNetif->netmask.addr;
    
}

void enet_lwip_init_handler_params(lwip_interrupt_handler_task_params_s * pTaskParams,EventGroupHandle_t group, EventBits_t wait_bits, void *pHandler, uint32_t inst_num, const char * name,uint32_t chan)
{
    pTaskParams->group     = group;
    pTaskParams->wait_bits = wait_bits;
    pTaskParams->pHandler  = ( void (*) (uint32_t) )pHandler;
    pTaskParams->inst_num  = inst_num;
    pTaskParams->name      = name;
    pTaskParams->irq_chan  = chan;
}
void enet_lwip_create_handler(void(*pTask)(void *),void * pParams)
{
    uint32_t err;
    lwip_interrupt_handler_task_params_s * p = (lwip_interrupt_handler_task_params_s *)pParams;
    err = xTaskCreate(pTask, p->name, pdAPP_TASK_STACK_SIZE_1KW_UL, p, PRIORITY_LWIP_INT_HLR, NULL );
    if (pdPASS == err)
    {
        NAV_APP_LOG_PRINTF("%s: ev:0x%08lX,wait:0x%08lX,hlr:0x%08lX\n\r",p->name,p->group,p->wait_bits,p->pHandler);
    }
    else
    {
        while(1)
        {
            NAV_APP_LOG_PRINTF("FAILURE ....... Starting Tasks for lwip Handlers...%s\n\r",p->name);
            Delay_HMSM(0,0,10,0);
        }
    }
    
}

portTASK_FUNCTION(lwip_main,pvParameters)
{
    unsigned int ipAddr;
    enet_task_params_s_t *p_enet_task_params_s = (enet_task_params_s_t *)pvParameters;
    /* configure Ethernet (GPIOs, clocks, MAC, DMA) */
    InitBspEthHW();
    /* Setup the Interrupt Handlers */
    lwip_cpsw_evflags_group = create_lwip_EventGroup();
    
    /* Init the Parameters for the tasks */
    /* RX Handler */
    enet_lwip_init_handler_params(&lwip_cpsw_handler_task_params_rx,(EventGroupHandle_t)lwip_cpsw_evflags_group,LWIP_FLAGS_RX_INT_HLR_EN,lwIPRxIntHandler,0,"lwip rx hlr",SYS_INT_3PGSWRXINT0);
    enet_lwip_create_handler(&lwip_cpsw_rx_tx_handler,&lwip_cpsw_handler_task_params_rx);
    /* TX Handler */
    enet_lwip_init_handler_params(&lwip_cpsw_handler_task_params_tx,(EventGroupHandle_t)lwip_cpsw_evflags_group,LWIP_FLAGS_TX_INT_HLR_EN,lwIPTxIntHandler,0,"lwip tx hlr",SYS_INT_3PGSWTXINT0);
    enet_lwip_create_handler(&lwip_cpsw_rx_tx_handler,&lwip_cpsw_handler_task_params_tx);
                                  
    /* Acquire IP  & */
#if 0
    /* Initilaize the LwIP stack */
    NAV_APP_LOG_PRINTF("Acquiring IP Address for Port 1... \n\r" );
    ipAddr = AcquireIP(&lwipIfPort1);
#endif
    
    /*EnetLWIPSwitchConfiguration(1);*/
    /* Loop forever.  For Switch Condigraton and interrupt handlers. */
    NAV_APP_LOG_PRINTF("Acquiring IP Address for Port 1... \n\r" );
    LWIP_IF *plwipIfPort = &lwipIfPort1;
    p_enet_task_params_s->pLWIP_IF = &lwipIfPort1;
    
    plwipIfPort->instNum = 0;
    plwipIfPort->slvPortNum = 1; 
    plwipIfPort->ipAddr = 0; 
    plwipIfPort->netMask = 0; 
    plwipIfPort->gwAddr = 0; 
    plwipIfPort->ipMode = IPADDR_USE_STATIC; 

    tcpip_init(InitLwipStack,plwipIfPort);
    // Delay_HMSM(0,0,50,0);
    while(1)
    {
        tcpecho_thread((void * )0);
    }
}

portTASK_FUNCTION(lwip_cpsw_rx_tx_handler,pvParameters)
{
    //TODO: Add OS Flags / Message Pend here for Exec
    lwip_interrupt_handler_task_params_s * ptask_params = (lwip_interrupt_handler_task_params_s *)pvParameters;
    NAV_APP_LOG_PRINTF("%s: ev:0x%08lX,wait:0x%08lX,hlr:0x%08lX\n\r",ptask_params->name,ptask_params->group,ptask_params->wait_bits,ptask_params->pHandler);
    while(TRUE)
    {
        EventBits_t ActiveFlags;
        ActiveFlags = xEventGroupWaitBits(ptask_params->group,ptask_params->wait_bits,pdTRUE,pdTRUE,portMAX_DELAY);
        /* Although we have set the Wait Period for indefinite, the following check is just for sanity */
        if(ActiveFlags & ptask_params->wait_bits)
        {
            NAV_APP_LOG_PRINTF("Executing lwip Handler...%s\n\r",ptask_params->name);
            ptask_params->pHandler(ptask_params->inst_num);
            IntSystemEnable(ptask_params->irq_chan);
        }
    }
}

/*
** Set up the ARM Interrupt Controller for generating timer interrupt
*/
static void AintcCPSWIntrSetUp(void)
{
    
    /* Register the Receive ISR for Core 0 */
    IntRegister(SYS_INT_3PGSWRXINT0, CPSWCore0RxIsr);
  
    /* Register the Transmit ISR for Core 0 */
    IntRegister(SYS_INT_3PGSWTXINT0, CPSWCore0TxIsr);
    
    /* Set the priority */
    uint32_t prio = configMAX_IRQ_VECTORS - 2; /*(configKERNEL_INTERRUPT_PRIORITY)*/
    IntPrioritySet(SYS_INT_3PGSWTXINT0, prio, AINTC_HOSTINT_ROUTE_IRQ); /* Lowering Priority here */
    IntPrioritySet(SYS_INT_3PGSWRXINT0, prio, AINTC_HOSTINT_ROUTE_IRQ); /* Lowering Priority here */

    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_3PGSWTXINT0);
    IntSystemEnable(SYS_INT_3PGSWRXINT0);
}


static EventGroupHandle_t create_lwip_EventGroup(void)
{
    /* Declare a variable to hold the created event group. */
    EventGroupHandle_t xCreatedEventGroup;

    /* Attempt to create the event group. */
    xCreatedEventGroup = xEventGroupCreate();

    /* Was the event group created successfully? */
    if( xCreatedEventGroup == NULL )
    {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
        return NULL;
    }
    else
    {
        /* The event group was created. */
        return xCreatedEventGroup;
    }
    
}

static void tcpecho_thread(void *arg)
{
    struct netconn *conn, *newconn;
    err_t err;
    LWIP_UNUSED_ARG(arg);
    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);
    if (conn != NULL)
    {
        uint16_t port = 10000;
        NAV_APP_LOG_PRINTF("conn = 0x%08x, binding to Port %d",conn,port);
        /* Bind connection to well known port number 10000. */
        err = netconn_bind(conn, IP_ADDR_ANY, port);

        if (err == ERR_OK)
        {
            
            NAV_APP_LOG_PRINTF("Bind Successful, Listening For Connections",conn);
            /* Tell connection to go into listening mode. */
            netconn_listen(conn);

            while (1)
            {
                /* Grab new connection. */
                newconn = 0;
                err = netconn_accept(conn,&newconn);
                
                
                /* Process the new connection. */
                if (newconn)
                {
                    NAV_APP_LOG_PRINTF("Accepted connection:",newconn);
                    struct netbuf *buf;
                    void *data;
                    u16_t len;
                    
                    while (ERR_OK == netconn_recv(newconn,&buf))
                    {
                        do
                        {
                            netbuf_data(buf, &data, &len);
                            NAV_APP_LOG_PRINTF("Received %02d bytes: %s\n\r",len,data);
                            netconn_write(newconn, data, len, NETCONN_COPY);
                        } while (netbuf_next(buf) >= 0);

                        netbuf_delete(buf);
                    }

                    /* Close connection and discard connection identifier. */
                    netconn_close(newconn);
                    netconn_delete(newconn);
                }
            }
        }
        else
        {
            NAV_APP_LOG_PRINTF(" can not bind TCP netconn");
        }
    }
    else
    {
        NAV_APP_LOG_PRINTF("can not create TCP netconn");
    }
}


void lwipInvokeHandler(EventGroupHandle_t xEventGroup,EventBits_t ev,uint8_t SetReset)
{

    BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Set / Reset in xEventGroup. */
    if (OS_FLAG_SET == SetReset)
    {
        xResult = xEventGroupSetBitsFromISR(
                                    xEventGroup,   /* The event group being updated. */
                                    ev, /* The bits being set. */
                                    &xHigherPriorityTaskWoken );
    }
    else
    {
        xResult = xEventGroupClearBitsFromISR(
                                    xEventGroup,   /* The event group being updated. */
                                    ev /* The bits being set. */);
        xHigherPriorityTaskWoken = pdFALSE;

    }
    

    /* Was the message posted successfully? */
    if( xResult == pdFREERTOS_ERRNO_NONE )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        if(xHigherPriorityTaskWoken)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*
** Interrupt Handler for Core 0 Receive interrupt
*/
static void CPSWCore0RxIsr(void)
{
    lwipInvokeHandler(lwip_cpsw_evflags_group,LWIP_FLAGS_RX_INT_HLR_EN,OS_FLAG_SET);
    // lwIPRxIntHandler(0);
    // IntSystemEnable(SYS_INT_3PGSWRXINT0);
}

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
static void CPSWCore0TxIsr(void)
{
    lwipInvokeHandler(lwip_cpsw_evflags_group, LWIP_FLAGS_TX_INT_HLR_EN,OS_FLAG_SET);
    // lwIPTxIntHandler(0);
    // IntSystemEnable(SYS_INT_3PGSWTXINT0);
}

void enet_lwip_netif_link_callback(struct netif *netif)
{
    BaseType_t link_state = netif_is_link_up(netif);
    if(pdTRUE == link_state)
    {
        lwipInvokeHandler(lwip_cpsw_evflags_group, LWIP_FLAGS_LMK_STATE,OS_FLAG_SET);
        netif_set_link_up(netif);
    }
    else
    {
        lwipInvokeHandler(lwip_cpsw_evflags_group, LWIP_FLAGS_LMK_STATE,OS_FLAG_CLR);
        netif_set_link_down(netif);
    }
}
/*
** Displays the IP addrss on the Console
*/
static void IpAddrDisplay(unsigned int ipAddr) 
{
    NAV_APP_LOG_PRINTF("%d.%d.%d.%d", (ipAddr & 0xFF), ((ipAddr >> 8) & 0xFF),
                       ((ipAddr >> 16) & 0xFF), ((ipAddr >> 24) & 0xFF));
}
/**
 * \brief   Interrupt handler for Receive Interrupt. Directly calls the
 *          cpsw interface receive interrupt handler.
 *
 * \param   instNum  The instance number of CPSW module for which receive 
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPRxIntHandler(unsigned int instNum) 
{
    cpswif_rx_inthandler(instNum, &cpswNetIF[0]);
}

/**
 * \brief   Interrupt handler for Transmit Interrupt. Directly calls the 
 *          cpsw interface transmit interrupt handler.
 *
 * \param   instNum  The instance number of CPSW module for which transmit
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPTxIntHandler(unsigned int instNum)
{
    cpswif_tx_inthandler(instNum);
}




static void lwIPDHCPComplete(unsigned int ifNum)
{
    unsigned int dhcpTries = NUM_DHCP_TRIES;
    int cnt;
    volatile unsigned char *state;

    while(dhcpTries--)
    {
        LWIP_PRINTF("\n\rDHCP Trial %d...", (NUM_DHCP_TRIES - dhcpTries));
        dhcp_start(&cpswNetIF[ifNum]);

        cnt = LWIP_DHCP_TIMEOUT;

        /* Check for DHCP completion for 'cnt' number of times, each 10ms */
        while(cnt--)
        {
            
            Delay_HMSM(0,0,10,0);
            state = &(cpswNetIF[ifNum].state);
            if(DHCP_STATE_BOUND == *state)
            {
                return;
            }
        }
    }

    LWIP_PRINTF("\n\rUnable to complete DHCP! \n\r");
}
#if 0
static unsigned int EnetLWIPSwitchConfiguration(unsigned int switchConfig)
{
    cpswConfig.cmd = 1;
    cpswConfig.cpsw_inst = 0;
    cpswConfig.ret = -2;
    cpswConfig.phy_param->slv_port_num = 1;
    cpswConfig.phy_param->autoneg = 1;
    cpswConfig.phy_param->config = 0x3f;
    cpswConfig.phy_param->speed = 1;
    cpswConfig.phy_param->duplex = 1;
    cpsw_switch_configuration(&cpswConfig);
    return FALSE;
}
#endif
/***************************** End Of File ***********************************/
#endif /* #ifndef ENET_WRAPPER_C */
