/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.35
        Device            :  PIC18F26K20
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set TIME_UP aliases
#define TIME_UP_TRIS               TRISBbits.TRISB0
#define TIME_UP_LAT                LATBbits.LATB0
#define TIME_UP_PORT               PORTBbits.RB0
#define TIME_UP_WPU                WPUBbits.WPUB0
#define TIME_UP_ANS                ANSELHbits.ANS12
#define TIME_UP_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define TIME_UP_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define TIME_UP_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define TIME_UP_GetValue()           PORTBbits.RB0
#define TIME_UP_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define TIME_UP_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define TIME_UP_SetPullup()      do { WPUBbits.WPUB0 = 1; } while(0)
#define TIME_UP_ResetPullup()    do { WPUBbits.WPUB0 = 0; } while(0)
#define TIME_UP_SetAnalogMode()  do { ANSELHbits.ANS12 = 1; } while(0)
#define TIME_UP_SetDigitalMode() do { ANSELHbits.ANS12 = 0; } while(0)

// get/set TIME_DOWN aliases
#define TIME_DOWN_TRIS               TRISBbits.TRISB1
#define TIME_DOWN_LAT                LATBbits.LATB1
#define TIME_DOWN_PORT               PORTBbits.RB1
#define TIME_DOWN_WPU                WPUBbits.WPUB1
#define TIME_DOWN_ANS                ANSELHbits.ANS10
#define TIME_DOWN_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define TIME_DOWN_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define TIME_DOWN_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define TIME_DOWN_GetValue()           PORTBbits.RB1
#define TIME_DOWN_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define TIME_DOWN_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define TIME_DOWN_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define TIME_DOWN_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define TIME_DOWN_SetAnalogMode()  do { ANSELHbits.ANS10 = 1; } while(0)
#define TIME_DOWN_SetDigitalMode() do { ANSELHbits.ANS10 = 0; } while(0)

// get/set TIME_SET aliases
#define TIME_SET_TRIS               TRISBbits.TRISB2
#define TIME_SET_LAT                LATBbits.LATB2
#define TIME_SET_PORT               PORTBbits.RB2
#define TIME_SET_WPU                WPUBbits.WPUB2
#define TIME_SET_ANS                ANSELHbits.ANS8
#define TIME_SET_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define TIME_SET_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define TIME_SET_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define TIME_SET_GetValue()           PORTBbits.RB2
#define TIME_SET_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define TIME_SET_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define TIME_SET_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define TIME_SET_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define TIME_SET_SetAnalogMode()  do { ANSELHbits.ANS8 = 1; } while(0)
#define TIME_SET_SetDigitalMode() do { ANSELHbits.ANS8 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()    do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()   do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()   do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()         PORTCbits.RC1
#define RC1_SetDigitalInput()   do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()  do { TRISCbits.TRISC1 = 0; } while(0)

// get/set CS aliases
#define CS_TRIS               TRISCbits.TRISC2
#define CS_LAT                LATCbits.LATC2
#define CS_PORT               PORTCbits.RC2
#define CS_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define CS_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define CS_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define CS_GetValue()           PORTCbits.RC2
#define CS_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define CS_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)

// get/set SCK aliases
#define SCK_TRIS               TRISCbits.TRISC3
#define SCK_LAT                LATCbits.LATC3
#define SCK_PORT               PORTCbits.RC3
#define SCK_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SCK_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SCK_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SCK_GetValue()           PORTCbits.RC3
#define SCK_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SCK_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)

// get/set SDI aliases
#define SDI_TRIS               TRISCbits.TRISC4
#define SDI_LAT                LATCbits.LATC4
#define SDI_PORT               PORTCbits.RC4
#define SDI_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SDI_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SDI_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SDI_GetValue()           PORTCbits.RC4
#define SDI_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SDI_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set SDO aliases
#define SDO_TRIS               TRISCbits.TRISC5
#define SDO_LAT                LATCbits.LATC5
#define SDO_PORT               PORTCbits.RC5
#define SDO_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SDO_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SDO_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SDO_GetValue()           PORTCbits.RC5
#define SDO_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SDO_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/