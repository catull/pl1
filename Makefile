PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload
BIN_DIR := bin

RM := rm -rf

CC := gcc
FLAGS := -Wall -O2

-include $(PLCMP_DIR)/pl.mk
-include $(ASMCMP_DIR)/asm.mk
-include $(ABSLOAD_DIR)/absload.mk

all: pl asm load

pl: $(PLCMP_OBJS)
	$(CC) $(PLCMP_OBJS) -o $(PL_BIN)

asm:

load:

.PHONY: clean
clean:
	-$(RM) $(PLCMP_OBJS) $(PLCMP_DEPS) $(ASMCMP_OBJ) $(ASMCMP_DEPS) $(ABSLOAD_OBJ) $(ABSLOAD_DEPS)