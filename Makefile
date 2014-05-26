ARMGNU ?= arm-none-eabi
CFLAGS = -std=gnu99 -nostdlib -fpic -fno-builtin

BUILD = build/
SOURCE = src/

# Generated files
TARGET = kernel.img
LIST = kernel.list
MAP = kernel.map


LINKER = kernel.ld

OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
OBJECTS += $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

all: $(TARGET) $(LIST)

rebuild: all

# Rule to make the listing file.
$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -d $(BUILD)output.elf > $(LIST)

# Rule to make the image file.
$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

# Rule to make the elf file.
$(BUILD)output.elf : $(OBJECTS) $(LINKER)
	$(ARMGNU)-ld --no-undefined $(OBJECTS) -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER)

# ASM
$(BUILD)%.o: $(SOURCE)%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $< -o $@

# C
$(BUILD)%.o: $(SOURCE)%.c $(BUILD)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir $@

clean :
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)