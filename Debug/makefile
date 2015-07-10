################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/BlackLib/BlackUART/subdir.mk
-include src/BlackLib/BlackTime/subdir.mk
-include src/BlackLib/BlackSPI/subdir.mk
-include src/BlackLib/BlackPWM/subdir.mk
-include src/BlackLib/BlackI2C/subdir.mk
-include src/BlackLib/BlackGPIO/subdir.mk
-include src/BlackLib/BlackDirectory/subdir.mk
-include src/BlackLib/BlackADC/subdir.mk
-include src/BlackLib/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: bbb-ahrs

# Tool invocations
bbb-ahrs: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	arm-linux-gnueabi-g++  -o "bbb-ahrs" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) bbb-ahrs
	-@echo ' '

post-build:
	-scp /home/kbdavid15/workspace/bbb-ahrs/Debug/bbb-ahrs root@192.168.7.2:/home/root/bbb_ahrs/
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
