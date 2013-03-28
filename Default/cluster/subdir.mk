################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cluster/cluster.c \
../cluster/cluster_editing.c 

OBJS += \
./cluster/cluster.o \
./cluster/cluster_editing.o 

C_DEPS += \
./cluster/cluster.d \
./cluster/cluster_editing.d 


# Each subdirectory must supply rules for building sources it contributes
cluster/%.o: ../cluster/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


