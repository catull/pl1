# encoding: UTF-8

PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload
BIN_DIR := bin

RM := rm -rf
MV := mv

CC := gcc
FLAGS := -O2 -Wall -std=c99

all: plcmp asmcmp absload

-include $(PLCMP_DIR)/plcmp.mk
-include $(ASMCMP_DIR)/asmcmp.mk
-include $(ABSLOAD_DIR)/absload.mk

plcmp: $(PLCMP_OBJS)
	$(CC) $(PLCMP_OBJS) -o $(PLCMP_BIN)

asmcmp: $(ASMCMP_OBJS)
	$(CC) $(ASMCMP_OBJS) -o $(ASMCMP_BIN)

absload: $(ABSLOAD_OBJS)
	$(CC) -lncurses $(ABSLOAD_OBJS) -o $(ABSLOAD_BIN)

.PHONY: clean clean_plcmp clean_asmcmp clean_absload clean_all

clean: clean_all
clean_all: clean_plcmp clean_asmcmp clean_absload

clean_plcmp:
	-$(RM) $(PLCMP_OBJ_DIR)/* $(PLCMP_DEP_DIR)/* $(PLCMP_BIN)
clean_asmcmp:
	-$(RM) $(ASMCMP_OBJ_DIR)/* $(ASMCMP_DEP_DIR)/* $(ASMCMP_BIN)
clean_absload:
	-$(RM) $(ABSLOAD_OBJ_DIR)/* $(ABSLOAD_DEP_DIR)/* $(ABSLOAD_BIN)