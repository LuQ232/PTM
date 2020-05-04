################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Laboratorium_1.1_Zadanie_2.lss \

FLASH_IMAGE += \
Laboratorium_1.1_Zadanie_2.hex \

EEPROM_IMAGE += \
Laboratorium_1.1_Zadanie_2.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Laboratorium_1.1_Zadanie_2.elf secondary-outputs

# Tool invocations
Laboratorium_1.1_Zadanie_2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Laboratorium_1.1_Zadanie_2.map -mmcu=atmega32 -o "Laboratorium_1.1_Zadanie_2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Laboratorium_1.1_Zadanie_2.lss: Laboratorium_1.1_Zadanie_2.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Laboratorium_1.1_Zadanie_2.elf  >"Laboratorium_1.1_Zadanie_2.lss"
	@echo 'Finished building: $@'
	@echo ' '

Laboratorium_1.1_Zadanie_2.hex: Laboratorium_1.1_Zadanie_2.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Laboratorium_1.1_Zadanie_2.elf  "Laboratorium_1.1_Zadanie_2.hex"
	@echo 'Finished building: $@'
	@echo ' '

Laboratorium_1.1_Zadanie_2.eep: Laboratorium_1.1_Zadanie_2.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Laboratorium_1.1_Zadanie_2.elf  "Laboratorium_1.1_Zadanie_2.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Laboratorium_1.1_Zadanie_2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Laboratorium_1.1_Zadanie_2.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Laboratorium_1.1_Zadanie_2.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
