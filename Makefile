# Makefile for scheduler
PROJECT = scheduler

PREFIX = arm-none-eabi

SRC = src
OUT = bin
INCLUDE = include
LIB = lib

${shell mkdir -p $(OUT)}

CC = $(PREFIX)-gcc
CFLAGS =	-mcpu=cortex-m3 \
			-mthumb \
			-O1 \
			-Wall \
			-Werror \
			-Wextra \
			-g \
			--std=c99 \
			-fdata-sections \
			-ffunction-sections

LD = $(PREFIX)-ld
LINKFLAGS = -T standalone.ld \
			-Map $(OUT)/mem.map \
			--entry ResetISR

AS = $(PREFIX)-as
OBJCOPY = $(PREFIX)-objcopy
SIZE = $(PREFIX)-size

OBJS = $(patsubst %.c, %.o, $(wildcard $(SRC)/*.c))
LIBS = $(foreach x, $(wildcard $(LIB)/*.a), $(x))

all : $(OBJS) $(PROJECT).elf $(PROJECT).bin

clean :
	@rm -rf $(OUT)
	@rm -rf $(wildcard $(SRC)/*.o)

%.o : %.c
	@echo "CC	$(<)"
	@$(CC) -c -I$(INCLUDE) $(CFLAGS) $< -o $@

$(PROJECT).elf : $(OBJS)
	@echo "LD	$(@)"
	@$(LD) $^ $(LINKFLAGS) $(LIBS) -o $(OUT)/$(PROJECT).elf

$(PROJECT).bin : $(PROJECT).elf
	@echo "OBJCOPY $(@)"
	@$(OBJCOPY) $(OUT)/$^ -O binary $(OUT)/$@

.phony: all clean
