/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.55
        Device            :  PIC18F26K20
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.43 or later
        MPLAB 	          :  MPLAB X 4.00
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "interrupt_manager.h"
#include "mcc.h"
#include "../main.h"

void  INTERRUPT_Initialize (void)
{
    // Disable Interrupt Priority Vectors (16CXXX Compatibility Mode)
    RCONbits.IPEN = 0;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
//    // interrupt handler
//    if(INTCONbits.PEIE == 1 && PIE1bits.TX1IE == 1 && PIR1bits.TX1IF == 1)
//    {
//        EUSART_TxDefaultInterruptHandler();
//    }
//    else if(INTCONbits.PEIE == 1 && PIE1bits.RC1IE == 1 && PIR1bits.RC1IF == 1)
//    {
//        EUSART_RxDefaultInterruptHandler();
//    }
//    else
//    {
//        //Unhandled Interrupt
//    }
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    // Check if the EUSART Transmit interrupt is active
    else if (PIE1bits.TX1IE && PIR1bits.TX1IF)
    {
        // Call the transmit interrupt handler
        EUSART_TxDefaultInterruptHandler();
        
        // Clear the TX interrupt flag (optional, usually handled by hardware)
        PIR1bits.TX1IF = 0;
    }
    
    // Check if the EUSART Receive interrupt is active
    else if (PIE1bits.RC1IE && PIR1bits.RC1IF)
    {
        // Call the receive interrupt handler
        btGetData(RCREG);
        
        // Clear the RX interrupt flag (optional, usually handled by hardware)
        PIR1bits.RC1IF = 0;
    }
    
}
/**
 End of File
*/
