################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ADXL345.cpp \
../src/HMC5883L.cpp \
../src/L3G4200D.cpp \
../src/main.cpp \
../src/myi2c.cpp 

OBJS += \
./src/ADXL345.o \
./src/HMC5883L.o \
./src/L3G4200D.o \
./src/main.o \
./src/myi2c.o 

CPP_DEPS += \
./src/ADXL345.d \
./src/HMC5883L.d \
./src/L3G4200D.d \
./src/main.d \
./src/myi2c.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabi-g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


