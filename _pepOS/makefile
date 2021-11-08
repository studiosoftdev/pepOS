BUILD_DIR = build
SRC_DIR = src
BUILD_NAME = pep.uf2
COPS = -march=armv6-m -mthumb -Wall -nostdlib -nostartfiles -ffreestanding \
		-Iinclude
ASMOPS = -Iinclude

all : bld

clean :
	rm -rf $(BUILD_DIR)/$(BUILD_NAME)
	rm $(BUILD_DIR)/pep.os

bld :
	arm-none-eabi-gcc -o $(BUILD_DIR)/pep.os $(SRC_DIR)/pep.c $(COPS)
	./$(BUILD_DIR)/o2u

run :
	$(BUILD_DIR)/$(BUILD_NAME)

bro2u:
	gcc $(SRC_DIR)/o2u.c -o $(BUILD_DIR)/$(BUILD_NAME)
	$(BUILD_DIR)/$(BUILD_NAME)
