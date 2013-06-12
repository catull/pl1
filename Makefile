PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload
BIN_DIR := bin

RM := rm -rf

CC := gcc
FLAGS := -Wall -O2

-include $(PLCMP_DIR)/plcmp.mk
-include $(ASMCMP_DIR)/asmcmp.mk
-include $(ABSLOAD_DIR)/absload.mk

all: pl asm load

pl: $(PLCMP_OBJS)
	$(CC) $(PLCMP_OBJS) -o $(PLCMP_BIN)

asm: $(ASMCMP_OBJS)
	$(CC) $(ASMCMP_OBJS) -o $(ASMCMP_BIN)

load: $(ABSLOAD_OBJS)
	$(CC) $(ABSLOAD_OBJS) -o $(ABSLOAD_BIN)

.PHONY: clean clean_plcmp clean_asmcmp clean_absload clean_all
	
clean: clean_all
clean_all: clean_plcmp clean_asmcmp clean_absload

clean_plcmp:
	-$(RM) $(PLCMP_OBJS) $(PLCMP_DEPS)
clean_asmcmp:
	-$(RM) $(ASMCMP_OBJ) $(ASMCMP_DEPS)
clean_absload:
	-$(RM) $(ABSLOAD_OBJ) $(ABSLOAD_DEPS)