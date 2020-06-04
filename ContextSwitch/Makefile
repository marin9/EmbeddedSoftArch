CC = arm-none-eabi
MCPU = -mcpu=arm1176jzf-s
CFLAGS = -ffreestanding -nostdlib -O2 -Wall -Wextra
LDFLAGS = -nostdlib -lgcc

SRC = $(wildcard *.c)
INC = $(wildcard *.h)
OBJ = $(SRC:%.c=%.o)

LD = linker.ld
ELF = kernel.elf
TARGET = kernel.img

$(TARGET): $(ELF)
	@echo " Compile\t$(TARGET)"
	@$(CC)-objcopy $(ELF) -O binary $(TARGET)

$(ELF): $(LD) $(OBJ)
	@echo " Compile\t$(ELF)"
	@$(CC)-gcc $(OBJ) -o $@ -T $(LD) $(LDFLAGS)

$(OBJ): %.o : %.c $(INC)
	@echo " Compile\t$<"
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $< -o $@

clean:
	@echo " Clean"
	@rm -f $(TARGET) $(ELF) *.o
