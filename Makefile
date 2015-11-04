# Makefile for scheduler
PROJECT = scheduler
BOARD ?= ek-lm3s811

PREFIX = arm-none-eabi

SRC = src
OUT = bin
INCLUDE = include
LIB = lib

${shell mkdir -p $(OUT)}

CC = $(PREFIX)-gcc
LD = $(PREFIX)-ld
AS = $(PREFIX)-as
GDB = $(PREFIX)-gdb
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump
SIZE = $(PREFIX)-size

CFLAGS +=	-mcpu=cortex-m3 -mthumb -ffreestanding 	-nostdlib -O1 -Wall
CFLAGS +=	-Werror -Wextra -ggdb3 --std=c99 -fdata-sections
CFLAGS +=	-ffunction-sections -fomit-frame-pointer

LINKFLAGS += -Tstandalone.ld -Map $(OUT)/mem.map --entry ResetISR

OBJS = $(patsubst %.c, %.o, $(wildcard $(SRC)/*.c))
LIBS = $(foreach x, $(wildcard $(LIB)/*.a), $(x))

# Rules
all : $(OBJS) $(PROJECT).elf $(PROJECT).bin size

program : all
	openocd -f board/$(BOARD).cfg &
	$(GDB) $(OUT)/$(PROJECT).elf

clean :
	@rm -rf $(OUT)
	@rm -rf $(wildcard $(SRC)/*.o)
	@rm -rf $(PROJECT).s

size : $(PROJECT).elf
	@$(SIZE) --format=berkeley $(OUT)/$^

%.o : %.c
	@echo "CC	$(<)"
	@$(CC) -c -I$(INCLUDE) $(CFLAGS) $< -o $@

$(PROJECT).elf : $(OBJS)
	@echo "LD	$(@)"
	@$(LD) $^ $(LINKFLAGS) $(LIBS) -o $(OUT)/$(PROJECT).elf

$(PROJECT).bin : $(PROJECT).elf
	@echo "OBJCOPY $(@)"
	@$(OBJCOPY) $(OUT)/$^ -O binary $(OUT)/$@
	@$(OBJDUMP) $(OUT)/$(PROJECT).elf -D > $(PROJECT).s

.phony: all clean
