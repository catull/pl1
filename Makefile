# encoding: UTF-8

PLCMP_DIR := plcmp
ASMCMP_DIR := asmcmp
ABSLOAD_DIR := absload
UTILS_DIR := utils

MAKE_FLAGS := -j 5
GLOBAL_CC_FLAGS :=

.PHONY: all clean clean_all libutils clean_libutils plcmp clean_plcmp asmcmp \
	clean_asmcmp absload clean_absload

all: libutils plcmp asmcmp absload
clean: clean_all
clean_all: clean_libutils clean_plcmp clean_asmcmp clean_absload

check_utils_dir:
ifndef PL1_UTILS_DIR
	$(error PL1_UTILS_DIR isn't defined. To fix it use 'source env.sh' \
		from the root of the project)
endif

libutils:
	$(MAKE) -C $(UTILS_DIR) $(MAKE_FLAGS) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f utils.mk build
clean_libutils:
	$(MAKE) -C $(UTILS_DIR) $(MAKE_FLAGS) -f utils.mk clean

plcmp: check_utils_dir libutils
	$(MAKE) -C $(PLCMP_DIR) $(MAKE_FLAGS) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f plcmp.mk build
clean_plcmp:
	$(MAKE) -C $(PLCMP_DIR) $(MAKE_FLAGS) -f plcmp.mk clean

asmcmp: check_utils_dir libutils
	$(MAKE) -C $(ASMCMP_DIR) $(MAKE_FLAGS) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) -f asmcmp.mk build
clean_asmcmp:
	$(MAKE) -C $(ASMCMP_DIR) $(MAKE_FLAGS) -f asmcmp.mk clean

absload: check_utils_dir libutils
	$(MAKE) -C $(ABSLOAD_DIR) \
	GLOBAL_CC_FLAGS=$(GLOBAL_CC_FLAGS) $(MAKE_FLAGS) -f absload.mk build
clean_absload:
	$(MAKE) -C $(ABSLOAD_DIR) $(MAKE_FLAGS) -f absload.mk clean