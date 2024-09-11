#include "main.h"

#define NUM_DEV 4
#define DEBOUNCE_DELAY 100
#define BLINK_INTERVAL 1000 // Interval for blinking display in milliseconds

#define LONG_PRESS_DURATION 3000

uint8_t bufferCol[NUM_DEV * 8];

// Time and alarm variables
uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
uint8_t alarmHours = 0;
uint8_t alarmMinutes = 0;
bool alarmSet = false;
bool alarmActive = false;

bool alarmTriggered = false;

#define UART_BUFFER_SIZE 10

// Buffer to store incoming UART data
char uartBuffer[UART_BUFFER_SIZE];
uint8_t uartBufferIndex = 0;

// Function prototypes




// Variables to manage blinking
uint16_t blinkTimer = 0;
bool displayBlinking = false;

uint16_t button3PressTime = 0;
bool isButton3Pressed = false;

bool btTimeSet = false;

#define FLASH_BUFFER_SIZE ERASE_FLASH_BLOCKSIZE



void main(void)
{
    SYSTEM_Initialize(); // Initialize system and peripherals
    matrixInit();       // Initialize MAX7219 matrix display
    clearDisplay();     // Clear display at the beginning
    loadTimeFromFlash();
    // Start the timer
    TMR0_StartTimer();
    
    INTCONbits.GIE = 1; // Enable Global Interrupts
    INTCONbits.PEIE = 1; // Enable Peripheral Interrupts
    

    while(1)
    {
        if(TMR0_HasOverflowOccured())
        {
            displayTime();
            // Increment time
            seconds++;
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) {
                    minutes = 0;
                    hours++;
                    if (hours >= 24) {
                        hours = 0;
                    }
                }
            }
            TMR0_Reload();
            INTCONbits.TMR0IF = 0;
            TMR0_StartTimer();
        }
        displayTime();
        checkButtons();
        processAlarm();
        if(btTimeSet) {
            // Check for the command structure: "H12M34;"
            if (uartBuffer[0] == 'H' && uartBuffer[3] == 'M' && uartBuffer[6] == ';')
            {
                // Extract the hours (characters 1 and 2)
                uint8_t hours_pom = (uartBuffer[1] - '0') * 10 + (uartBuffer[2] - '0');

                // Extract the minutes (characters 4 and 5)
                uint8_t minutes_pom = (uartBuffer[4] - '0') * 10 + (uartBuffer[5] - '0');

                // Validate the extracted values
                if (hours_pom < 24 && minutes_pom < 60)
                {
                    // Now you have the valid hours and minutes as integers
                    btTimeSet = false;
                    hours = hours_pom;
                    minutes = minutes_pom;
                    displayTime(); // Display the time on your LED matrix or clock
                    saveTimeToFlash(); // Save the time in flash memory (if applicable)
                }
            }
        }
    }
}

// Display the current time on the LED matrix
void displayTime(void)
{
    char timeString[5]; // HH:MM format (including separator and spaces)

    // Format time into the char array
    timeString[0] = (hours / 10) + '0';       // Tens place of hours
    timeString[1] = (hours % 10) + '0';       // Ones place of hours                      // Separator
    timeString[2] = (minutes / 10) + '0';     // Tens place of minutes
    timeString[3] = (minutes % 10) + '0';     // Ones place of minutes
    timeString[4] = '\0';                    // Null-terminator for string functions

    // Convert char array to uint8_t array for printString
    uint8_t displayString[5];
    for (int i = 0; i < 5; i++) {
        displayString[i] = (uint8_t)timeString[i];
    }

    printString(displayString); // Show the time on the display
}

void displayAlarmTime(void)
{
    char alarmTimeString[5]; // HHMM format (no separator)

    // Format alarm time into the char array
    alarmTimeString[0] = (alarmHours / 10) + '0';    // Tens place of alarm hours
    alarmTimeString[1] = (alarmHours % 10) + '0';    // Ones place of alarm hours
    alarmTimeString[2] = (alarmMinutes / 10) + '0';  // Tens place of alarm minutes
    alarmTimeString[3] = (alarmMinutes % 10) + '0';  // Ones place of alarm minutes
    alarmTimeString[4] = '\0';                       // Null-terminator for string functions

    // Convert char array to uint8_t array for printString
    uint8_t displayString[5];
    for (int i = 0; i < 5; i++) {
        displayString[i] = (uint8_t)alarmTimeString[i];
    }

    printString(displayString); // Show the alarm time on the display
}

void processAlarm(void)
{
    // Only check the alarm if it's set and not already triggered
    if (alarmSet && !alarmTriggered)
    {
        // Compare current time with alarm time
        if (hours == alarmHours && minutes == alarmMinutes)
        {
            triggerAlarm(); // Alarm event
            alarmTriggered = true; // Mark alarm as triggered
        }
    }
}

// Function to trigger the alarm event
void triggerAlarm(void)
{
    // Define the delay values
    #define FLASH_INTERVAL_MS 500
    #define ALARM_DURATION_MS 60000

    // Calculate the number of flash cycles needed for 1 minute
    unsigned int numCycles = ALARM_DURATION_MS / (2 * FLASH_INTERVAL_MS); // Each cycle includes on and off

    for (unsigned int i = 0; i < numCycles; i++)
    {
        // Check if the alarm has been cleared
        if (!PORTBbits.RB2)
        {
            __delay_ms(DEBOUNCE_DELAY);
            if (!PORTBbits.RB2)
            {
                clearDisplay();  // Optionally clear the display
                return;
            }
        }
        
        // Flash the display with "ALARM"
        printString("ALARM");
        __delay_ms(FLASH_INTERVAL_MS);
        clearDisplay();
        __delay_ms(FLASH_INTERVAL_MS);
    }

    // Optionally, turn off alarm buzzer/LED if used
    // PORTAbits.RA0 = 0;  // Turn off alarm buzzer/LED
}
// Function to clear the alarm once the user acknowledges it
void clearAlarm(void)
{
    alarmTriggered = false;    // Reset the triggered flag
    alarmSet = false;          // Optionally clear the alarm set flag (if you want the user to set it again)
    
    // Turn off the buzzer/LED if it's on
    // PORTAbits.RA0 = 0;  // Turn off the alarm buzzer/LED
}


// Function to check button presses and debounce them
void checkButtons(void)
{
    static uint8_t button3PressCount = 0;
    static uint16_t button3HoldTime = 0;
    static bool alarmSettingMode = false;

    // Check for Button 1 (Increase hours for time or alarm)
    if (!PORTBbits.RB0) // Button 1 pressed
    {
        __delay_ms(DEBOUNCE_DELAY);
        if (!PORTBbits.RB0)
        {
            if (alarmSettingMode)
            {
                alarmHours = (alarmHours + 1) % 24;
                displayAlarmTime();
            }
            else
            {
                hours = (hours + 1) % 24;
                displayTime();
            }
        }
    }

    // Check for Button 2 (Increase minutes for time or alarm)
    if (!PORTBbits.RB1) // Button 2 pressed
    {
        __delay_ms(DEBOUNCE_DELAY);
        if (!PORTBbits.RB1)
        {
            if (alarmSettingMode)
            {
                alarmMinutes = (alarmMinutes + 1) % 60;
                displayAlarmTime();
            }
            else
            {
                minutes = (minutes + 1) % 60;
                displayTime();
            }
        }
    }

    // Check for Button 3 (Set alarm, save time to flash, or clear the alarm)
    if (!PORTBbits.RB2) // Button 3 pressed
    {
        button3HoldTime += DEBOUNCE_DELAY;
        __delay_ms(DEBOUNCE_DELAY);

        if (button3HoldTime >= LONG_PRESS_DURATION)
        {
            if (!alarmTriggered)
            {
                alarmSettingMode = true;
                displayAlarmTime();

                while (!PORTBbits.RB2)
                {
                    if (!PORTBbits.RB0) // Button 1 (Increase alarm hours)
                    {
                        __delay_ms(DEBOUNCE_DELAY);
                        alarmHours = (alarmHours + 1) % 24;
                        displayAlarmTime();
                    }
                    if (!PORTBbits.RB1) // Button 2 (Increase alarm minutes)
                    {
                        __delay_ms(DEBOUNCE_DELAY);
                        alarmMinutes = (alarmMinutes + 1) % 60;
                        displayAlarmTime();
                    }
                }

                alarmSettingMode = false;
                alarmSet = true;
                displayTime();
            }
        }
        else
        {
            if (alarmTriggered)
            {
                clearAlarm();
            }
        }
    }
    else
    {
        if (button3HoldTime < LONG_PRESS_DURATION)
        {
            button3PressCount++;
            if (button3PressCount == 5)
            {
                saveTimeToFlash();
                button3PressCount = 0;
            }
        }
        button3HoldTime = 0;
    }
}


// Save the current time (hours and minutes) to flash memory
void saveTimeToFlash(void)
{
    uint32_t flashAddress = 0x1F80; // Choose a suitable address for storage
    uint8_t flashBuffer[FLASH_BUFFER_SIZE];

    // Load current flash block into buffer
    for (uint8_t i = 0; i < FLASH_BUFFER_SIZE; i++) {
        flashBuffer[i] = FLASH_ReadByte(flashAddress + i);
    }

    // Store hours and minutes into the buffer
    flashBuffer[0] = hours;
    flashBuffer[1] = minutes;

    // Write the block back to flash
    FLASH_WriteBlock(flashAddress, flashBuffer);
}

// Load the time from flash memory on startup
void loadTimeFromFlash(void)
{
    uint32_t flashAddress = 0x1F80; // Same address as in saveTimeToFlash
    hours = FLASH_ReadByte(flashAddress);      // Read hours
    minutes = FLASH_ReadByte(flashAddress + 1); // Read minutes

    // Validate read values to ensure they're within range
    if (hours >= 24) hours = 0;
    if (minutes >= 60) minutes = 0;
}

void btGetData(char rcv) {
    if(rcv == 'H' && uartBufferIndex == 0) {
        uartBuffer[uartBufferIndex] = 'H';
        uartBufferIndex++;
    }
    else if(uartBufferIndex != 0 && uartBufferIndex < 7) {
        uartBuffer[uartBufferIndex++] = rcv;
    } 
    else if(uartBufferIndex == 7) {
        if(rcv == ';') {
            uartBuffer[6] = ';';
            uartBufferIndex = 0;
            btTimeSet = true;
        }
    }
}