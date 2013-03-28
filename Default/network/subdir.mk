################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../network/dynamic_array.c \
../network/edge.c \
../network/network.c \
../network/vertex.c 

OBJS += \
./network/dynamic_array.o \
./network/edge.o \
./network/network.o \
./network/vertex.o 

C_DEPS += \
./network/dynamic_array.d \
./network/edge.d \
./network/network.d \
./network/vertex.d 


# Each subdirectory must supply rules for building sources it contributes
network/%.o: ../network/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


