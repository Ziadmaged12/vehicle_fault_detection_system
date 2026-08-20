/******************************************************************************
 *
 * Module: HMI_ECU_MAIN
 *
 * File Name: main.
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/
#include "../MACROS/STD_types.h"
#include "../HAL/LCD_driver.h"
#include "../HAL/keypad.h"
#include "../MCAL/uart.h"
#include "../MCAL/Timers_Interface.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// --- Definitions ---

// UART Commands to send to Control ECU
#define HMI_CMD_START_MONITORING   0x01
#define HMI_CMD_SEND_LIVE_VALUES   0x02
#define HMI_CMD_SEND_LOGGED_FAULTS 0x03
#define HMI_CMD_STOP_MONITORING    0x04

// Fault DTCs (Diagnostic Trouble Codes)
#define DTC_P001_DISTANCE_CLOSE    0x01
#define DTC_P002_ENGINE_OVERHEAT   0x02

// UART Markers
#define END_OF_FAULTS_MARKER       0xFF // Received from Control ECU

// UI Delays
#define DISPLAY_TIME_SECONDS       3000 // Delay for showing messages (in ms)

// --- Global Variables ---

// Timer1 configuration for 1-second compare match
Timer_ConfigType g_timerConfig = {
		.timer_InitialValue = 0,
		.timer_Compare_MatchValue = 7812, // For 1 sec at 8MHz F_CPU/1024
		.timer_ID = TIMER1_ID,
		.timer_clock = F_CPU_1024,
		.timer_mode = COMPARE_MODE
};

// Flag set by Timer ISR
volatile u8 g_timerFlag = 0;

// --- Function Prototypes ---
void  DisplayMainMenu(void);
void handle_start_operation(void);
void handle_display_values(void);
void handle_retrieve_faults(void);
void handle_stop_monitoring(void);
u16 UART_recieveWord(void);
void Timer_Callback(void);

// --- Functions Definitions ---

// ISR Callback function for Timer1
void Timer_Callback(void)
{
	g_timerFlag = 1;
}

// Initializes all HMI peripherals
void HMI_Init(void)
{
	LCD_init();
	keypad_init();

	// Configure UART settings
	UART_ConfigType uart_config = {
			.bit_data = UART_8_BIT_DATA,
			.parity = UART_NO_PARITY,
			.stop_bit = UART_1_STOP_BIT,
			.baud_rate = 9600
	};
	UART_init(&uart_config);

	// Initialize Timer1
	Timer_setCallBack(Timer_Callback, TIMER1_ID);
	Timer_init(&g_timerConfig);
}

// Clears LCD and displays the main menu options
void DisplayMainMenu(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "1-Start Operation");
	LCD_displayStringRowColumn(1, 0, "2-Display Values");
	LCD_displayStringRowColumn(2, 0, "3-Retrieve Faults");
	LCD_displayStringRowColumn(3, 0, "4-Stop Monitoring");
}

// Sends a single byte command to the Control ECU
void SendCommandToControlECU(u8 command)
{
	UART_sendByte(command);
	_delay_ms(10); // Short delay for processing
}

// Receives a generic data buffer from UART with timeout (NOT USED)
u8 ReceiveDataFromControlECU(u8* data, u16 timeoutMs)
{
	u16 timeElapsed = 0;
	u8 index = 0;
	u8 receivedByte;

	for(u8 i = 0; i < 16; i++) { // Clear the buffer
		data[i] = 0;
	}

	while(timeElapsed < timeoutMs)
	{
		receivedByte = UART_recieveByte();
		if(receivedByte != 0xFF) // Ignore 0xFF
		{
			data[index++] = receivedByte;

			if(receivedByte == '#') // Check for end marker
			{
				data[index] = '\0';
				return 1;  // Success
			}

			if(index >= 15) // Prevent buffer overflow
			{
				data[index] = '\0';
				return 1;
			}
		}
		_delay_ms(10);
		timeElapsed += 10;
	}

	if(index > 0) // Return partial data on timeout
	{
		data[index] = '\0';
		return 1;
	}

	return 0;  // No data
}

// Handles '1-Start Operation' menu selection
void handle_start_operation(void)
{
	UART_sendByte(HMI_CMD_START_MONITORING); // Send command

	LCD_clearScreen(); // Show confirmation
	LCD_displayStringRowColumn(0, 0, "Operation       ");
	LCD_displayStringRowColumn(1, 0, "Started!        ");
	LCD_displayStringRowColumn(2, 0, "Monitoring      ");
	LCD_displayStringRowColumn(3, 0, "Active...       ");

	_delay_ms(DISPLAY_TIME_SECONDS);
}

// Handles '2-Display Values' menu selection
void handle_display_values(void)
{
	u8 key;
	u8 temperature;
	u16 distance;
	u8 window_states;
	u8 w1_open;
	u8 w2_open;

	do {
		UART_sendByte(HMI_CMD_SEND_LIVE_VALUES); // 1. Send command

		// 2. Receive data in exact order from Control ECU
		temperature = UART_recieveByte();
		distance = UART_recieveWord();
		window_states = UART_recieveByte();

		// 3. Parse the packed window states byte
		w1_open = window_states & 0x01;
		w2_open = (window_states >> 1) & 0x01;

		// 4. Display data on LCD
		LCD_clearScreen();

		LCD_displayStringRowColumn(0, 0, "Temp: ");
		LCD_intgerToString(temperature);
		LCD_displayString(" C      ");

		LCD_displayStringRowColumn(1, 0, "Dist: ");
		LCD_intgerToString(distance);
		LCD_displayString(" cm     ");

		LCD_displayStringRowColumn(2, 0, "W1:");
		LCD_displayString(w1_open ? "Open   " : "Closed ");

		LCD_displayStringRowColumn(3, 0, "W2:");
		LCD_displayString(w2_open ? "Open   " : "Closed ");

		_delay_ms(DISPLAY_TIME_SECONDS); // 5. Wait

		// 6. Ask to repeat
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Display again?  ");
		LCD_displayStringRowColumn(1, 0, "Press 2 = YES   ");
		LCD_displayStringRowColumn(2, 0, "Other = MENU    ");

		key = KEYPAD_getPressedKey();
		_delay_ms(50);

	} while (key == '2');
}

// Handles '3-Retrieve Faults' menu selection
void handle_retrieve_faults(void)
{
	u8 key;
	u8 fault_code;
	u8 line_num;
	u8 faults_found;

	do {
		UART_sendByte(HMI_CMD_SEND_LOGGED_FAULTS); // 1. Send command

		// 2. Receive and display faults
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Logged Faults:  ");

		line_num = 1;
		faults_found = 0;

		// Loop until end marker is received
		while( (fault_code = UART_recieveByte()) != END_OF_FAULTS_MARKER )
		{
			faults_found = 1;

			if (line_num > 3) // Scroll LCD if full
			{
				_delay_ms(DISPLAY_TIME_SECONDS);
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Logged Faults:  ");
				line_num = 1;
			}

			// Display the DTC string
			LCD_moveCursor(line_num, 0);
			if (fault_code == DTC_P001_DISTANCE_CLOSE)
			{
				LCD_displayString("P001:Dist Close ");
			}
			else if (fault_code == DTC_P002_ENGINE_OVERHEAT)
			{
				LCD_displayString("P002:Overheat   ");
			}
			else // Handle unknown codes
			{
				LCD_displayString("P-Unk: ");
				LCD_intgerToString(fault_code);
				LCD_displayString("        ");
			}
			line_num++;
		}

		// 3. Display status if no faults found
		if (!faults_found)
		{
			LCD_displayStringRowColumn(1, 0, "No faults found.");
		}
		else // Show end of list
		{
			if (line_num > 3) // Scroll if needed
			{
				_delay_ms(DISPLAY_TIME_SECONDS);
				LCD_clearScreen();
				line_num = 0;
			}
			LCD_displayStringRowColumn(line_num, 0, "--End of List-- ");
		}

		_delay_ms(DISPLAY_TIME_SECONDS); // 4. Wait

		// 5. Ask to repeat
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Display again?  ");
		LCD_displayStringRowColumn(1, 0, "Press 3 = YES   ");
		LCD_displayStringRowColumn(2, 0, "Other = MENU    ");

		key = KEYPAD_getPressedKey();
		_delay_ms(50);

	} while (key == '3');
}

// Handles '4-Stop Monitoring' menu selection
void handle_stop_monitoring(void)
{
	UART_sendByte(HMI_CMD_STOP_MONITORING); // Send command

	LCD_clearScreen(); // Show confirmation
	LCD_displayStringRowColumn(0, 0, "System          ");
	LCD_displayStringRowColumn(1, 0, "Monitoring      ");
	LCD_displayStringRowColumn(2, 0, "Stopped!        ");

	_delay_ms(DISPLAY_TIME_SECONDS);
}

// Receives a 16-bit word (u16) from UART
u16 UART_recieveWord(void)
{
	u16 high_byte = UART_recieveByte(); // High byte is sent first
	u16 low_byte  = UART_recieveByte(); // Low byte is sent second

	return ( (high_byte << 8) | low_byte ); // Combine bytes
}

// Main function
int main(void)
{
	u8 key_pressed = 0;

	HMI_Init(); // Initialize all peripherals
	DisplayMainMenu(); // Show menu first

	sei(); // Enable global interrupts

	// Welcome Message
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Vehicle Fault   ");
	LCD_displayStringRowColumn(1, 0, "Detection System");
	LCD_displayStringRowColumn(2, 0, "HMI Ready...    ");
	_delay_ms(DISPLAY_TIME_SECONDS);

	// Main loop
	while(1)
	{
		DisplayMainMenu(); // Show main menu

		key_pressed = KEYPAD_getPressedKey(); // Wait for user input

		_delay_ms(50); // Simple debounce

		// Handle menu selection
		switch(key_pressed)
		{
		case '1':
			handle_start_operation();
			break;

		case '2':
			handle_display_values();
			break;

		case '3':
			handle_retrieve_faults();
			break;

		case '4':
			handle_stop_monitoring();
			break;

		default: // Handle invalid input
			LCD_clearScreen();
			LCD_displayStringRowColumn(1, 3, "Invalid Key!");
			_delay_ms(2000);
			break;
		}
	}
}
