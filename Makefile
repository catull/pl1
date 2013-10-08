# encoding: UTF-8

PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload

GLOBAL_CC_FLAGS := 

.PHONY: all plcmp asmcmp absload clean clean_plcmp clean_asmcmp clean_absload

all: plcmp asmcmp absload
clean: clean_all
clean_all: clean_plcmp clean_asmcmp clean_absload

plcmp:
	cd $(PLCMP_DIR) && pwd -P && \
$(MAKE) GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f plcmp.mk build
clean_plcmp:
	cd $(PLCMP_DIR) && pwd -P && $(MAKE) -f plcmp.mk clean

asmcmp:
	cd $(ASMCMP_DIR) && pwd -P && \
$(MAKE) GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f asmcmp.mk build
clean_asmcmp:
	cd $(ASMCMP_DIR) && pwd -P && $(MAKE) -f asmcmp.mk clean

absload:
	cd $(ABSLOAD_DIR) && pwd -P && \
$(MAKE) GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f absload.mk build
clean_absload:
	cd $(ABSLOAD_DIR) && pwd -P && $(MAKE) -f absload.mk clean