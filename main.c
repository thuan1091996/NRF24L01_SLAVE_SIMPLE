/* --------0.Project information--------------------
 * NRF24L01 Master
 * Protocol communication: SPI (SSI0)
 * Debug through UART0 (PC)
 * Send data to slave, band width 1 byte
 * Author : TRAN MINH THUAN
 * -------------------------------------------------*/

/* --------1.System requirement--------------------
 * 1.Create a simple SSI communication system
 * 2.Create a simple UART0 communication to debug
 * 3.Create a protocol to send data through a Master NRF24L01 to NRF24L01 slave
 * -------------------------------------------------*/

/* --------2.Pre-processor Directives Section-------------------*/
#include "include.h"

/*-------------------------Define value------------------------*/
#define LED             GPIO_PIN_2
#define LED_BASE        GPIO_PORTF_BASE
void GPIOF_Init(void);
void GPIOA_Init();
void UART0_Init(void);

unsigned char ui8Data_Slave=0; //register to hold letter received and sent
unsigned char Data_NRF[50]={0};

void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80MHz
    GPIOF_Init();
    GPIOA_Init();
    SSI0_Init();
    UART0_Init();
    nrf24l01_initialize_debug(true, 1, false); //RX,1 byte, Disable Enhanced ShockBrust
    while(true)
    {
        nrf24l01_get_all_registers(&Data_NRF);
        while(!nrf24l01_irq_rx_dr_active());
        nrf24l01_read_rx_payload(&ui8Data_Slave, 1); //read the packet into data
        if(ui8Data_Slave==0x01)
        {
            if(GPIOPinRead(LED_BASE, LED)==0) GPIOPinWrite(LED_BASE, LED, 0x04);
            else GPIOPinWrite(LED_BASE, LED, 0);
            ui8Data_Slave=0;
        }
        nrf24l01_irq_clear_all();                    //clear all interrupts in the 24L01
    }
}

/* GPIO PORTF Initialization
 * Function: Define PF4 as input pin, 4MA, Pull-up resistor (For Testing SPI communication)
 * Input: No
 * Output: No
 * Changing this function:
       + Changing GPIO Port (SysCtlEn & SysCtlReady)
       + Changing GPIO Pin function (GPIOPinType)
       + Changing Other Input pin characteristic (GPIOPadConfig)
 * Associate with:
       + PF4
       + No variable
 * Testing this function: Click Button and LED on PF1 will bright until release
*/
void GPIOF_Init(void) {
    //Enable peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        //Enable clock for PortF
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); //Wait for PortF ready
    //Configure for pins
    GPIOPinTypeGPIOOutput(LED_BASE, LED); //Configure red LED for testing
}
void GPIOA_Init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_3);
}
/* UART Initialization
 * Function: Initialization UART0: Use System Clock, 115200, 8-bit data, 1 stop, no parity bit
 * Input: No
 * Output: No
 * Change this function:
       + Change UART Port by changing GPIO port, UART base (SysCtlEn & SysCtlReady)
         then change appropriate pins (GPIOPinConfigure,GPIOPinTypeUART)
       + Changing Other UART characteristic (UARTConfigSetExpClk)
 * Associate with
       + PA0,PA1
       + No variable
*/
void UART0_Init(void) {
    //Enable peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);             //Enable Clock for UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);             //Clock for PortA
    //PORT Pin configure for UART
    GPIOPinConfigure(GPIO_PA0_U0RX);                         //PIN mux for UART0
    GPIOPinConfigure(GPIO_PA1_U0TX);                         //PIN mux for UART0
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1);//PIN mux for UART0
    //Configure for UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
