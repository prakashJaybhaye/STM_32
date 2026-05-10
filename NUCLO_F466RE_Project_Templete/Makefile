######################################
# Project name
######################################
TARGET = main

######################################
# Toolchain
######################################
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

######################################
# MCU FLAGS
######################################
CFLAGS = -mcpu=cortex-m4 -mthumb -g -O0 -Wall -ffreestanding -nostdlib

######################################
# INCLUDE PATHS
######################################
CFLAGS += -ICore/Inc
CFLAGS += -IDrivers/Inc

######################################
# SOURCE FILES
######################################
C_SOURCES = $(wildcard Core/Src/*.c) \
            $(wildcard Drivers/Src/*.c)

ASM_SOURCES = Startup/startup_stm32f446retx.s

######################################
# LINKER SCRIPT
######################################
LDSCRIPT = Linker/STM32F446RETX_FLASH.ld

######################################
# LINKER FLAGS
######################################
LDFLAGS = -T$(LDSCRIPT) -nostdlib -Wl,--gc-sections

######################################
# BUILD
######################################
all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(C_SOURCES) $(ASM_SOURCES)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

######################################
# FLASH
######################################
flash: $(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program $(TARGET).elf verify reset exit"

######################################
# CLEAN
######################################
clean:
	rm -f *.elf *.bin