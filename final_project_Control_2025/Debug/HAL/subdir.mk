################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/DcMotor_driver.c \
../HAL/LM35_driver.c \
../HAL/Ultra_sonic_sensor_driver.c \
../HAL/external_eeprom.c 

OBJS += \
./HAL/DcMotor_driver.o \
./HAL/LM35_driver.o \
./HAL/Ultra_sonic_sensor_driver.o \
./HAL/external_eeprom.o 

C_DEPS += \
./HAL/DcMotor_driver.d \
./HAL/LM35_driver.d \
./HAL/Ultra_sonic_sensor_driver.d \
./HAL/external_eeprom.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/%.o: ../HAL/%.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


