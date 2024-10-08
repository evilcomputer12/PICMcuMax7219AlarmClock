/**
  MSSP Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    spi.c

  @Summary
    This is the generated driver implementation file for the MSSP driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This source file provides APIs for MSSP.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.55
        Device            :  PIC18F26K20
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "spi.h"

/**
  Section: Macro Declarations
*/

#define SPI_RX_IN_PROGRESS 0x0

/**
  Section: Module APIs
*/

void SPI_Initialize(void)
{
    // Set the SPI module to the options selected in the User Interface
    
    // R_nW write_noTX; P stopbit_notdetected; S startbit_notdetected; BF RCinprocess_TXcomplete; SMP End; UA dontupdate; CKE Active to Idle; D_nA lastbyte_address; 
    SSPSTAT = 0xC0;
    
    // SSPEN enabled; WCOL no_collision; CKP Idle:High, Active:Low; SSPM FOSC/4; SSPOV no_overflow; 
    SSPCON1 = 0x30;
    
    // SSPADD 0; 
    SSPADD = 0x00;
}

uint8_t SPI_Exchange8bit(uint8_t data)
{
    // Clear the Write Collision flag, to allow writing
    SSPCON1bits.WCOL = 0;

    SSPBUF = data;

    while(SSPSTATbits.BF == SPI_RX_IN_PROGRESS)
    {
    }

    return (SSPBUF);
}

uint8_t SPI_Exchange8bitBuffer(uint8_t *dataIn, uint8_t bufLen, uint8_t *dataOut)
{
    uint8_t bytesWritten = 0;

    if(bufLen != 0)
    {
        if(dataIn != NULL)
        {
            while(bytesWritten < bufLen)
            {
                if(dataOut == NULL)
                {
                    SPI_Exchange8bit(dataIn[bytesWritten]);
                }
                else
                {
                    dataOut[bytesWritten] = SPI_Exchange8bit(dataIn[bytesWritten]);
                }

                bytesWritten++;
            }
        }
        else
        {
            if(dataOut != NULL)
            {
                while(bytesWritten < bufLen )
                {
                    dataOut[bytesWritten] = SPI_Exchange8bit(DUMMY_DATA);

                    bytesWritten++;
                }
            }
        }
    }

    return bytesWritten;
}

bool SPI_IsBufferFull(void)
{
    return (SSPSTATbits.BF);
}

bool SPI_HasWriteCollisionOccured(void)
{
    return (SSPCON1bits.WCOL);
}

void SPI_ClearWriteCollisionStatus(void)
{
    SSPCON1bits.WCOL = 0;
}
/**
 End of File
*/