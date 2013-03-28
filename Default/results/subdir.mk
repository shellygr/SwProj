################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../results/results_files_writer.c \
../results/xml_writer.c 

OBJS += \
./results/results_files_writer.o \
./results/xml_writer.o 

C_DEPS += \
./results/results_files_writer.d \
./results/xml_writer.d 


# Each subdirectory must supply rules for building sources it contributes
results/%.o: ../results/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


