################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../operation/commands.c \
../operation/errors.c \
../operation/ui.c 

OBJS += \
./operation/commands.o \
./operation/errors.o \
./operation/ui.o 

C_DEPS += \
./operation/commands.d \
./operation/errors.d \
./operation/ui.d 


# Each subdirectory must supply rules for building sources it contributes
operation/%.o: ../operation/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


