NAME := whoami
TARGET := riscv64

OBJS := main.o art.o uart.o start.o

FEL := ../xfel/xfel
FEL_ADDR = 0x40000000

CC := clang
CFLAGS := -Wall -Wextra -Werror -mno-relax -nostdlib --target=$(TARGET)
CPPFLAGS := -I .

LD := ld.lld
LDFLAGS := -T link.ld

OBJCOPY := llvm-objcopy
OBJCOPYFLAGS := --binary-architecture=riscv64 --strip-all

RM := rm -f
MAKEFLAGS += --no-print-directory

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

bin: $(NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(NAME) -O binary $(NAME).bin

# Run with sudo
fel-exec: bin
	$(FEL) version
	$(FEL) ddr d1
	$(FEL) write $(FEL_ADDR) $(NAME).bin
	$(FEL) exec $(FEL_ADDR)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) $(NAME).bin

re:
	$(MAKE) fclean
	$(MAKE) bin

.PHONY: bin fel-exec clean fclean re
