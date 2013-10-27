# encoding: UTF-8

PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload

MAKE_FLAGS := -j 5
GLOBAL_CC_FLAGS :=

.PHONY: all plcmp asmcmp absload clean clean_plcmp clean_asmcmp clean_absload

all: plcmp asmcmp absload
clean: clean_all
clean_all: clean_plcmp clean_asmcmp clean_absload

plcmp:
	$(MAKE) -C $(PLCMP_DIR) $(MAKE_FLAGS) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f plcmp.mk build
clean_plcmp:
	$(MAKE) -C $(PLCMP_DIR) $(MAKE_FLAGS) -f plcmp.mk clean

asmcmp:
	$(MAKE) -C $(ASMCMP_DIR) $(MAKE_FLAGS) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f asmcmp.mk build
clean_asmcmp:
	$(MAKE) -C $(ASMCMP_DIR) $(MAKE_FLAGS) -f asmcmp.mk clean

absload:
	$(MAKE) -C $(ABSLOAD_DIR) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) $(MAKE_FLAGS) -f absload.mk build
clean_absload:
	$(MAKE) -C $(ABSLOAD_DIR) $(MAKE_FLAGS) -f absload.mk clean