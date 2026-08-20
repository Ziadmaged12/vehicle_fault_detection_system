/******************************************************************************
 *
 * Module: CONTROL_ECU_MAIN
 *
 * File Name: main.c
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#include <avr/interrupt.h> // For sei() global interrupt enable
#include <util/delay.h>    // For _delay_ms()
#include "../HAL/DcMotor_driver.h"
#include "../HAL/LM35_driver.h"
#include "../HAL/Ultra_sonic_sensor_driver.h"
#include "../HAL/external_eeprom.h"
#include "../MCAL/GPIO_inteface.h"
#include "../MCAL/ADC_interface.h"
#include "../MCAL/ICU_interface.h"
#include "../MCAL/PWM_inteface.h" // Using the fixed interface
#include "../MCAL/twi.h"
#include "../MCAL/uart.h"
#include "../MACROS/STD_types.h" // For u8, u16, etc.

// --- Definitions ---

// Commands received from HMI ECU
#define HMI_CMD_START_MONITORING   0x01
#define HMI_CMD_SEND_LIVE_VALUES   0x02
#define HMI_CMD_SEND_LOGGED_FAULTS 0x03
#define HMI_CMD_STOP_MONITORING    0x04

// Fault DTCs (Diagnostic Trouble Codes)
#define DTC_P001_DISTANCE_CLOSE    0x01
#define DTC_P002_ENGINE_OVERHEAT   0x02
#define NO_FAULT                   0x00
#define END_OF_FAULTS_MARKER       0xFF // Marker sent to HMI after all faults

// EEPROM Configuration
#define FAULTS_LOG_START_ADDRESS   0x0001
#define EEPROM_MAX_ADDRESS         0x07FF // Max for 24C16 (2K bytes)
#define EEPROM_WRITE_DELAY_MS      10     // Delay for EEPROM internal write cycle

// Fault Thresholds
#define DISTANCE_FAULT_THRESHOLD   10     // cm
#define TEMPERATURE_FAULT_THRESHOLD 90    // °C

// Window States
#define WINDOW_STATE_CLOSED        0
#define WINDOW_STATE_OPEN          1

// GPIO Configuration for window control buttons (Active Low)
#define WIN1_OPEN_BUTTON_PORT      PORT_u8B
#define WIN1_OPEN_BUTTON_PIN       PIN4
#define WIN1_CLOSE_BUTTON_PORT     PORT_u8B
#define WIN1_CLOSE_BUTTON_PIN      PIN5

#define WIN2_OPEN_BUTTON_PORT      PORT_u8B
#define WIN2_OPEN_BUTTON_PIN       PIN6
#define WIN2_CLOSE_BUTTON_PORT     PORT_u8B
#define WIN2_CLOSE_BUTTON_PIN      PIN7

// Debounce Configuration
#define BUTTON_DEBOUNCE_DELAY_MS   50

// --- Static Global Variables ---
static u8 g_p001_logged = 0; // Flag: Distance fault logged this session
static u8 g_p002_logged = 0; // Flag: Temperature fault logged this session
static u8 g_window1_state = WINDOW_STATE_CLOSED;
static u8 g_window2_state = WINDOW_STATE_CLOSED;


// --- Function Prototypes ---
u8 log_fault(u8 fault_dtc_code);
void check_and_log_faults(u8 temp, u16 dist);
void handle_windows(u8 *w1_state, u8 *w2_state);
void UART_sendWord(u16 data);
u8 read_button_debounced(u8 port, u8 pin);


// --- Functions Definitions ---

// Sends a 16-bit word (u16) over UART
void UART_sendWord(u16 data)
{
	UART_sendByte( (u8)(data >> 8) );    // Send High Byte first
	UART_sendByte( (u8)(data & 0xFF) );  // Send Low Byte second
}

// Writes a fault code to the next available slot in EEPROM
u8 log_fault(u8 fault_dtc_code)
{
	u16 address = FAULTS_LOG_START_ADDRESS;
	u8 data_in_slot = 0;
	u8 status = 0;

	while (address < EEPROM_MAX_ADDRESS)
	{
		status = EEPROM_readByte(address, &data_in_slot); // Read current slot

		if (status != SUCCESS)
		{
			return ERROR; // I2C read error
		}

		if (data_in_slot == 0xFF) // Find first empty slot (default EEPROM value)
		{
			status = EEPROM_writeByte(address, fault_dtc_code); // Write the fault code

			if (status == SUCCESS)
			{
				_delay_ms(EEPROM_WRITE_DELAY_MS); // Wait for write cycle
				return SUCCESS;
			}
			else
			{
				return ERROR; // I2C write failed
			}
		}
		address++; // Check next address
	}
	return ERROR; // EEPROM is full
}

// Checks sensor values against thresholds and logs faults if needed
void check_and_log_faults(u8 temp, u16 dist)
{
	// Check for Distance Fault (P001)
	if (dist < DISTANCE_FAULT_THRESHOLD)
	{
		if (g_p001_logged == 0) // Log only once per session
		{
			if (log_fault(DTC_P001_DISTANCE_CLOSE) == SUCCESS)
			{
				g_p001_logged = 1; // Mark as logged
			}
		}
	}
	// Note: Fault flags are only reset when monitoring starts

	// Check for Temperature Fault (P002)
	if (temp > TEMPERATURE_FAULT_THRESHOLD)
	{
		if (g_p002_logged == 0) // Log only once per session
		{
			if (log_fault(DTC_P002_ENGINE_OVERHEAT) == SUCCESS)
			{
				g_p002_logged = 1; // Mark as logged
			}
		}
	}
}

// Reads a button press with a simple software debounce
u8 read_button_debounced(u8 port, u8 pin)
{
	if (GPIO_u8GetPinValue(port, pin) == PIN_LOW) // Check for initial press (active low)
	{
		_delay_ms(BUTTON_DEBOUNCE_DELAY_MS);

		if (GPIO_u8GetPinValue(port, pin) == PIN_LOW) // Verify button is still pressed
		{
			return 1; // Button confirmed pressed
		}
	}
	return 0; // Button not pressed
}

// Reads window buttons and controls motors accordingly
void handle_windows(u8 *w1_state, u8 *w2_state)
{
	// --- Window 1 Control ---
	if (read_button_debounced(WIN1_OPEN_BUTTON_PORT, WIN1_OPEN_BUTTON_PIN))
	{
		DcMotor_Rotate(MOTOR_1ID, MOTOR_CW, 100); // Open window 1
		g_window1_state = WINDOW_STATE_OPEN;
	}
	else if (read_button_debounced(WIN1_CLOSE_BUTTON_PORT, WIN1_CLOSE_BUTTON_PIN))
	{
		DcMotor_Rotate(MOTOR_1ID, MOTOR_ACW, 100); // Close window 1
		g_window1_state = WINDOW_STATE_CLOSED;
	}
	else
	{
		DcMotor_Rotate(MOTOR_1ID, MOTOR_STOP, 0); // Stop window 1
	}

	// --- Window 2 Control ---
	if (read_button_debounced(WIN2_OPEN_BUTTON_PORT, WIN2_OPEN_BUTTON_PIN))
	{
		DcMotor_Rotate(MOTOR_2ID, MOTOR_CW, 100); // Open window 2
		g_window2_state = WINDOW_STATE_OPEN;
	}
	else if (read_button_debounced(WIN2_CLOSE_BUTTON_PORT, WIN2_CLOSE_BUTTON_PIN))
	{
		DcMotor_Rotate(MOTOR_2ID, MOTOR_ACW, 100); // Close window 2
		g_window2_state = WINDOW_STATE_CLOSED;
	}
	else
	{
		DcMotor_Rotate(MOTOR_2ID, MOTOR_STOP, 0); // Stop window 2
	}

	*w1_state = g_window1_state; // Update state variable via pointer
	*w2_state = g_window2_state; // Update state variable via pointer
}



// --- Main Function ---
int main(void)
{
	// Local variables
	u8 command_from_hmi = 0;
	u8 monitoring_active = 0;
	u8 temperature = 0;
	u16 distance = 0;
	u8 window1_state = WINDOW_STATE_CLOSED;
	u8 window2_state = WINDOW_STATE_CLOSED;
	u8 fault_code = 0;
	u16 eeprom_address = 0;
	u8 data_read_status = 0;

	sei(); // Enable Global Interrupts (required for ICU)

	// --- Initialization ---

	// Initialize UART for HMI communication
	UART_ConfigType uart_config = {
			.bit_data = UART_8_BIT_DATA,
			.parity = UART_NO_PARITY,
			.stop_bit = UART_1_STOP_BIT,
			.baud_rate = 9600
	};
	UART_init(&uart_config);

	// Initialize TWI (I2C) for EEPROM
	TWI_ConfigType twi_config = {
			.address = 0x01, // ECU TWI address (if acting as slave, not relevant here)
			.bit_rate = 100000
	};
	TWI_init(&twi_config);

	// Initialize ADC for LM35 Temperature Sensor
	ADC_ConfigType adc_config = {
			.ref_volt = INTERNAL_REF,       // Use 2.56V internal reference
			.prescaler = ADC_PRESCALER_128
	};
	ADC_init(&adc_config);

	// Initialize PWM Timer *before* DcMotor_Init() as it depends on it
	PWM_Timer0_Init();

	// Initialize HAL Drivers
	DcMotor_Init();
	Ultrasonic_init();

	// Initialize GPIO pins for window buttons (Input with Pull-up)
	GPIO_voidSetPinDirection(WIN1_OPEN_BUTTON_PORT, WIN1_OPEN_BUTTON_PIN, PIN_IN);
	GPIO_voidSetPinDirection(WIN1_CLOSE_BUTTON_PORT, WIN1_CLOSE_BUTTON_PIN, PIN_IN);
	GPIO_voidSetPinDirection(WIN2_OPEN_BUTTON_PORT, WIN2_OPEN_BUTTON_PIN, PIN_IN);
	GPIO_voidSetPinDirection(WIN2_CLOSE_BUTTON_PORT, WIN2_CLOSE_BUTTON_PIN, PIN_IN);

	// Enable internal pull-ups (buttons are active low)
	GPIO_voidSetPinValue(WIN1_OPEN_BUTTON_PORT, WIN1_OPEN_BUTTON_PIN, PIN_HIGH);
	GPIO_voidSetPinValue(WIN1_CLOSE_BUTTON_PORT, WIN1_CLOSE_BUTTON_PIN, PIN_HIGH);
	GPIO_voidSetPinValue(WIN2_OPEN_BUTTON_PORT, WIN2_OPEN_BUTTON_PIN, PIN_HIGH);
	GPIO_voidSetPinValue(WIN2_CLOSE_BUTTON_PORT, WIN2_CLOSE_BUTTON_PIN, PIN_HIGH);

	// --- Main Loop ---
	while(1)
	{
		command_from_hmi = UART_recieveByte(); // Wait (block) for a command

		// Handle window buttons regardless of the command
		handle_windows(&window1_state, &window2_state);

		switch(command_from_hmi)
		{
		case HMI_CMD_START_MONITORING:
			monitoring_active = 1;
			g_p001_logged = 0; // Reset fault flags for new session
			g_p002_logged = 0;
			break;

		case HMI_CMD_SEND_LIVE_VALUES:
			temperature = LM35_getTemperature(); // Read sensors
			distance = Ultrasonic_readDistance();

			handle_windows(&window1_state, &window2_state); // Read window buttons

			// Send all data back to HMI
			UART_sendByte(temperature);
			UART_sendWord(distance); // Send 16-bit distance

			// Pack window states into one byte (bit0=Win1, bit1=Win2)
			u8 window_states = (window2_state << 1) | window1_state;
			UART_sendByte(window_states);

			if (monitoring_active) // Log faults only if monitoring is active
			{
				check_and_log_faults(temperature, distance);
			}
			break;

		case HMI_CMD_SEND_LOGGED_FAULTS:
			eeprom_address = FAULTS_LOG_START_ADDRESS;
			do {
				data_read_status = EEPROM_readByte(eeprom_address, &fault_code);

				if (data_read_status == SUCCESS)
				{
					if (fault_code == 0xFF) // 0xFF marks end of faults
							{
						break;
							}
					UART_sendByte(fault_code); // Send the fault code
					eeprom_address++;
				}
				else
				{
					break; // I2C Error, stop reading
				}
			} while (eeprom_address < EEPROM_MAX_ADDRESS);

			UART_sendByte(END_OF_FAULTS_MARKER); // Tell HMI we are done
			break;

		case HMI_CMD_STOP_MONITORING:
			monitoring_active = 0;
			DcMotor_Rotate(MOTOR_1ID, MOTOR_STOP, 0); // Stop motors
			DcMotor_Rotate(MOTOR_2ID, MOTOR_STOP, 0);
			window1_state = WINDOW_STATE_CLOSED; // Reset states
			window2_state = WINDOW_STATE_CLOSED;
			break;

		default:
			// Unknown command, do nothing
			break;
		}
	}
}
