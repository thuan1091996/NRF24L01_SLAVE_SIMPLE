/*
 * DELAY.c
 *
 *  Created on: Oct 26, 2018
 *      Author: Itachi
 */
#include "include.h"
#include "DELAY.h"


void Systick_ISR(){
    Tick++; //Increase every 1 us corresponding to Reload value
};

void Systick_ISR();
/* --------Delay 1 micro second corresponding to "usec"----------
 * Initialization for Systick timer then load 80
   (80 * 1/(80*10^6) = 1us)
 * Change the Load value to create different time delay interval
 * Change the interrupt handler by SystickIntRegister function
 * STAY IN LOOP until delay enough amount of time except when Systick interrupt execute
 * Use "HWREG" to change register value (more info in HW_type.h)
-------------------------------------------------------*/
void delay_us(unsigned int microseconds) {
    SysTickIntRegister(Systick_ISR);    //Define  Systick ISR
    HWREG(NVIC_ST_CTRL)&=~0x07;         //Disable Systick Timer during initialization
    HWREG(NVIC_ST_RELOAD)=80-1;         //1us
    Tick=0;                             //Reset count value
    HWREG(NVIC_ST_CURRENT)=0;           //Write any value to clear the counter
    HWREG(NVIC_ST_CTRL)|=0x07;          //Enable Systick Timer and ARM for Systick interrupt
    while(Tick < microseconds);
    HWREG(NVIC_ST_CTRL)&=~0x07;         //Disable Systick Timer after delay time
}
