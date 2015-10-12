# Makefile for scheduler
PROJECT = scheduler
BOARD = ek-lm3s811

PREFIX = arm-none-eabi

SRC = src
OUT = bin
INCLUDE = include
LIB = lib

${shell mkdir -p $(OUT)}

CC = $(PREFIX)-gcc
CFLAGS =	-mcpu=cortex-m3 \
			-mthumb \
			-ffreestanding \
			-nostdlib \
			-O1 \
			-Wall \
			-Werror \
			-Wextra \
			-ggdb3 \
			--std=c99 \
			-fdata-sections \
			-ffunction-sections \
			-fomit-frame-pointer

LD = $(PREFIX)-ld
LINKFLAGS = -T standalone.ld \
			-Map $(OUT)/mem.map \
			--entry ResetISR

AS = $(PREFIX)-as
GDB = $(PREFIX)-gdb
OBJCOPY = $(PREFIX)-objcopy
SIZE = $(PREFIX)-size

OBJS = $(patsubst %.c, %.o, $(wildcard $(SRC)/*.c))
LIBS = $(foreach x, $(wildcard $(LIB)/*.a), $(x))

all : $(OBJS) $(PROJECT).elf $(PROJECT).bin

program : all
	openocd -f board/$(BOARD).cfg &
	$(GDB) $(OUT)/$(PROJECT).elf

clean :
	@rm -rf $(OUT)
	@rm -rf $(wildcard $(SRC)/*.o)
	@rm -rf $(PROJECT).s

%.o : %.c
	@echo "CC	$(<)"
	@$(CC) -c -I$(INCLUDE) $(CFLAGS) $< -o $@

$(PROJECT).elf : $(OBJS)
	@echo "LD	$(@)"
	@$(LD) $^ $(LINKFLAGS) $(LIBS) -o $(OUT)/$(PROJECT).elf

$(PROJECT).bin : $(PROJECT).elf
	@echo "OBJCOPY $(@)"
	@$(OBJCOPY) $(OUT)/$^ -O binary $(OUT)/$@
	@arm-none-eabi-objdump $(OUT)/$(PROJECT).elf -D > $(PROJECT).s

.phony: all clean
