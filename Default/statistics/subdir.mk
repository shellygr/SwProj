################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../statistics/queue.c \
../statistics/statistics.c \
../statistics/tuple.c 

OBJS += \
./statistics/queue.o \
./statistics/statistics.o \
./statistics/tuple.o 

C_DEPS += \
./statistics/queue.d \
./statistics/statistics.d \
./statistics/tuple.d 


# Each subdirectory must supply rules for building sources it contributes
statistics/%.o: ../statistics/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


