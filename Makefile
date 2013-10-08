# encoding: UTF-8

PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload
BIN_DIR := bin

RM := rm -rf

CC := gcc
FLAGS := -O2 -Wall -std=c99

.PHONY: all plcmp asmcmp absload clean clean_plcmp clean_asmcmp clean_absload

all: plcmp asmcmp absload
clean: clean_all
clean_all: clean_plcmp clean_asmcmp clean_absload

plcmp:
	cd $(PLCMP_DIR) && pwd -P && $(MAKE) -f plcmp.mk build
clean_plcmp:
	cd $(PLCMP_DIR) && pwd -P && $(MAKE) -f plcmp.mk clean

asmcmp:
	cd $(ASMCMP_DIR) && pwd -P && $(MAKE) -f asmcmp.mk build
clean_asmcmp:
	cd $(ASMCMP_DIR) && pwd -P && $(MAKE) -f asmcmp.mk clean

absload:
	cd $(ABSLOAD_DIR) && pwd -P && $(MAKE) -f absload.mk build
clean_absload:
	cd $(ABSLOAD_DIR) && pwd -P && $(MAKE) -f absload.mk clean