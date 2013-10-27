# encoding: UTF-8

RM := rm -rf

CC := gcc

CC_FLAGS := -O2 -Wall -std=c99
CC_FLAGS += $(GLOBAL_CC_FLAGS)

PLCMP_BIN := bin/plcmp

PLCMP_INC_DIR := inc
PLCMP_SRC_DIR := src
PLCMP_OBJ_DIR := obj
PLCMP_DEP_DIR := dep

# PL COMPILER INCLUDES
PLCMP_INCLUDES := -I $(PLCMP_INC_DIR)
PLCMP_INCLUDES +=

# PL COMPILER DEFINITIONS
PLCMP_DEFINITIONS :=
PLCMP_DEFINITIONS +=

# PL COMPILER SOURCES
PLCMP_SRCS_NOTDIR := $(notdir $(wildcard $(PLCMP_SRC_DIR)/*.c))

# PL COMPILER OBJECTS
PLCMP_OBJS := $(patsubst %.c, $(PLCMP_OBJ_DIR)/%.o, $(PLCMP_SRCS_NOTDIR))

# PL COMPILER DEPENDENCIES
PLCMP_DEPS := $(patsubst %.c, $(PLCMP_DEP_DIR)/%.d, $(PLCMP_SRCS_NOTDIR))

build: $(PLCMP_OBJS)
	$(CC) $(PLCMP_OBJS) -o $(PLCMP_BIN)
clean:
	-$(RM) $(PLCMP_OBJS) $(PLCMP_DEPS) $(PLCMP_BIN)

$(PLCMP_OBJ_DIR)/%.o: $(PLCMP_SRC_DIR)/%.c
	$(CC) -c $< $(CC_FLAGS) $(PLCMP_INCLUDES) -MM -MT $@ -MF \
$(addprefix $(PLCMP_DEP_DIR)/, $(patsubst $(PLCMP_OBJ_DIR)/%.o, %.d, $@))
	$(CC) -c $< $(CC_FLAGS) $(PLCMP_INCLUDES) -o $@

-include $(PLCMP_DEPS)

.PHONY: build clean