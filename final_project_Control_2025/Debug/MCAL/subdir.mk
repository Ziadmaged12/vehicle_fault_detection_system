################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/ADC_program.c \
../MCAL/GPIO_program.c \
../MCAL/ICU_interface.c \
../MCAL/PWM_interface.c \
../MCAL/twi.c \
../MCAL/uart.c 

OBJS += \
./MCAL/ADC_program.o \
./MCAL/GPIO_program.o \
./MCAL/ICU_interface.o \
./MCAL/PWM_interface.o \
./MCAL/twi.o \
./MCAL/uart.o 

C_DEPS += \
./MCAL/ADC_program.d \
./MCAL/GPIO_program.d \
./MCAL/ICU_interface.d \
./MCAL/PWM_interface.d \
./MCAL/twi.d \
./MCAL/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


