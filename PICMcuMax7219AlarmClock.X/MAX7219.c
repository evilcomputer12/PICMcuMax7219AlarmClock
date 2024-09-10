
#include "xc.h"
#include "MAX7219.h"
#include "Fonts.h"

#define NUM_DEV 4
uint8_t bufferCol[NUM_DEV*8];


void max_write(int row, uint8_t data)
{
	int devTarget = (row - 1) / 8;  // find out which is the actual max, where we need to write the data
	int offset = devTarget * 8;  // The offset of the start byte for the devTarget in the buffer
	uint16_t writeData = 0;

//	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);  // Select the slave
//    LATBbits.LATB3 = 0;
    CS_SetLow();
	for (int dev = 0; dev < NUM_DEV; dev++)   // for loop for all the max connected
	{
		if (dev == devTarget)  // if this the target
		{
			writeData = ((row - offset)<<8)|data;  // send the column number and the data byte
//			HAL_SPI_Transmit(&hspi1, (uint8_t *)&writeData, 1, 1000);
//            SPI1_Exchange16bit(writeData);
            // Send the high byte (most significant 8 bits)
            SPI_Exchange8bit((writeData >> 8) & 0xFF);
            // Send the low byte (least significant 8 bits)
            SPI_Exchange8bit(writeData & 0xFF);
            
		}
		else
		{
			writeData = 0;  // else send NOOP
//			HAL_SPI_Transmit(&hspi1, (uint8_t *)&writeData, 1, 1000);
//            SPI1_Exchange16bit(writeData);
//            // Send the high byte (most significant 8 bits)
//            SPI_WriteByte((writeData >> 8) & 0xFF);
//            // Send the low byte (least significant 8 bits)
//            SPI_WriteByte(writeData & 0xFF);
            // Send the high byte (most significant 8 bits)
            SPI_Exchange8bit((writeData >> 8) & 0xFF);
            // Send the low byte (least significant 8 bits)
            SPI_Exchange8bit(writeData & 0xFF);
		}
	}
//	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);  // disable the slave
//    LATBbits.LATB3 = 1;
    CS_SetHigh();
}

void flushBuffer (void)
{
	uint8_t bufferRow[NUM_DEV*8] = {0};  // buffer to store data column wise

	/* Convert Cols to Rows */
	for (int i=0; i<NUM_DEV*8; i++)  // for loop for all the bytes
	{
		int dev = i/8;  // 0,1,2,3..  // keep track of which max is being written
		for (int j=0; j<8; j++)  // for loop to extract bits
		{
			if ((bufferCol[i])&(1<<(j)))  // if the bit is 1 // start extracting from the 0th bit of C0
			{
				bufferRow[j+(8*dev)] |= (1<<(7-(i-(8*dev))));  // start writing it from the 7th bit of R0
			}
		}
	}


	for (int row=1; row<=(NUM_DEV*8); row++)  // write the column data into the columns
	{
		max_write(row, bufferRow[row-1]);
	}
}

void max7219_cmd (uint8_t Addr, uint8_t data)
{
	uint16_t writeData = (Addr<<8)|data;
//	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);  // enable slave
//    LATBbits.LATB3 = 0;
    CS_SetLow();
	for (int i=0; i<NUM_DEV; i++)
	{
//		HAL_SPI_Transmit(&hspi1, (uint8_t *)&writeData, 1, 100);
//        SPI1_Exchange16bit(writeData);
        // Send the high byte (most significant 8 bits)
        SPI_Exchange8bit((writeData >> 8) & 0xFF);
            // Send the low byte (least significant 8 bits)
        SPI_Exchange8bit(writeData & 0xFF);
	}
//	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);  // disable slave
//    LATBbits.LATB3 = 1;
    CS_SetHigh();
}

void matrixInit (void)
{
	max7219_cmd(0x09, 0);  // no decoding
	max7219_cmd(0x0a, 0x01);  // 3/32 intensity
	max7219_cmd(0x0B, 0x07);  // scan all 7 columns
	max7219_cmd(0x0C, 0x01);  // normal operation
	max7219_cmd(0x0F, 0);     // No display test
}

void clearDisplay (void)
{
	for (int i=0; i<NUM_DEV*8-1; i++)
	{
		bufferCol[i] = 0;
	}
	flushBuffer();
}

void shiftLeft (void)
{
	for (int cnt=NUM_DEV*8-2; cnt>=0; cnt--)
	{
		bufferCol[cnt+1] = bufferCol[cnt];
	}
	bufferCol[0] = 0;
	flushBuffer();
}

void HAL_Delay(int delay) {
    while(delay--){
        __delay_ms(1);
    }
}

void shiftchar (uint8_t ch, int delay)
{
	int indx=0;
	for (int i=0; i<FONT_WIDTH-1; i++)  // loop for all the bytes of the font
	{
		uint8_t data = 0;
		/* Chnage the order of the bits */
		for (int j=7; j>=0; j--)  // extract bits from a single byte
		{
			if ((MAX7219_Dot_Matrix_font[ch][indx])&(1<<j))  // if the bit is 1 // start extracting from the 7th bit of byte
			{
				data |= (1<<(7-j));  // start writing it from the 0th bit of data
			}
		}
		bufferCol[0] = data;  // store the modified byte to the first element only. It will shift later
		flushBuffer();
		shiftLeft();
		indx++;
		HAL_Delay(delay);
	}
}

void scrollString (char *str, int delay)
{
	while (*str)
	{
		shiftchar(*str, delay);
		*str++;
	}
}

void printString(uint8_t *str)
{
    int strindx = 0;
    int k = NUM_DEV * 8 - 1; // Initialize index for bufferCol
    
    while (str[strindx] != '\0') // Ensure we process until end of string
    {
        int indx = 0;
        for (int i = 0; i < FONT_WIDTH; i++) // Loop for each byte of the font
        {
            uint8_t data = 0;
            // Change the order of the bits
            for (int j = 7; j >= 0; j--) // Extract bits from a single byte
            {
                if ((MAX7219_Dot_Matrix_font[str[strindx]][indx]) & (1 << j)) // Check if bit is 1
                {
                    data |= (1 << (7 - j)); // Write it in reversed order
                }
            }
            if (k < 0) break; // Prevent buffer overflow
            bufferCol[k--] = data; // Store modified byte
            indx++;
        }
        strindx++; // Move to next character
        if (k < 0) break; // Prevent buffer overflow
    }
    
    flushBuffer(); // Send data to display
}