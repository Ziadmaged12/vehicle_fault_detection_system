/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.c
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#include "twi.h"
#include "../MACROS/macros.h"
#include"../MACROS/STD_types.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType *Config_Ptr)
{

    CLEAR_BIT(TWSR, TWPS0);
    CLEAR_BIT(TWSR, TWPS1);

    TWBR = (u8)(((F_CPU / (Config_Ptr->bit_rate * 1000UL)) - 16) / 2);

    TWAR = (Config_Ptr->address << 1);

    SET_BIT(TWCR, TWEN);
}

void TWI_start(void)
{
    /* Clear TWINT, send START, enable TWI */
    SET_BIT(TWCR, TWINT);
    SET_BIT(TWCR, TWSTA);
    SET_BIT(TWCR, TWEN);

    /* Wait for TWINT flag set */
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

void TWI_stop(void)
{
    /* Clear TWINT, send STOP, enable TWI */
    SET_BIT(TWCR, TWINT);
    SET_BIT(TWCR, TWSTO);
    SET_BIT(TWCR, TWEN);
}

void TWI_writeByte(u8 data)
{
    TWDR = data;
    SET_BIT(TWCR, TWINT);
    SET_BIT(TWCR, TWEN);
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

u8 TWI_readByteWithACK(void)
{
    SET_BIT(TWCR, TWINT);
    SET_BIT(TWCR, TWEN);
    SET_BIT(TWCR, TWEA);
    while (BIT_IS_CLEAR(TWCR, TWINT));
    return TWDR;
}

u8 TWI_readByteWithNACK(void)
{
    SET_BIT(TWCR, TWINT);
    SET_BIT(TWCR, TWEN);
    while (BIT_IS_CLEAR(TWCR, TWINT));
    return TWDR;
}

u8 TWI_getStatus(void)
{
    return (TWSR & 0xF8);
}
