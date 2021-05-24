PRJ_ROOT_DIR := $(shell pwd)

.PHONY = all clean
#Target Name
TARGET_NAME := Myfirstbuild
PROJECT_TYPE := app

#Version Number
MAJOR := 0
MINOR := 08

#Include directories
INCLUDES_SRC = -I./Inc \
			   -I./Drivers/STM32F4xx_HAL_Driver/Inc \
			   -I./Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
			   -I./Drivers/CMSIS/Device/ST/STM32F4xx/Include \
			   -I./Drivers/CMSIS/Include \
			   -I./Drivers/CMSIS/Device/ST/STM32F4xx/Include

# Source files

SRC := $(PRJ_ROOT_DIR)/Src/main.c \
#Bin files
BINS := $(PRJ_ROOT_DIR)/Src/main

#Compiler
CC := D:/gcc-arm-none-eabi-9-2019-q4-major-win32/bin/arm-none-eabi-gcc.exe

#Compile Flag
COMPILE_FLAG := -mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 \
				-mfloat-abi=hard -mthumb-interwork -std=c99 \
				
#Global Build Macros
DEFINES +=

main.o: ./Src/main.c 
	$(CC) $(INCLUDES_SRC) -c ./Src/main.c $(COMPILE_FLAG) 
	
#Make build
all: main.o
	$(CC) main.o -o main


#Clean
clean:
	rm -rvf *.o $(BINS)

help:
	echo "$(PRJ_ROOT_DIR)"