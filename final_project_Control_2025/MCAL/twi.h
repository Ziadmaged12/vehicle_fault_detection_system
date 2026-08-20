/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "../MACROS/STD_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08
#define TWI_REP_START     0x10
#define TWI_MT_SLA_W_ACK  0x18
#define TWI_MT_SLA_R_ACK  0x40
#define TWI_MT_DATA_ACK   0x28
#define TWI_MR_DATA_ACK   0x50
#define TWI_MR_DATA_NACK  0x58

/*******************************************************************************
 *                      Types Declaration                                      *
 *******************************************************************************/

typedef u8  TWI_AddressType;
typedef u32 TWI_BaudRateType;

typedef struct
{
    TWI_AddressType address;
    TWI_BaudRateType bit_rate;
} TWI_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(u8 data);
u8 TWI_readByteWithACK(void);
u8 TWI_readByteWithNACK(void);
u8 TWI_getStatus(void);

#endif /* TWI_H_ */
