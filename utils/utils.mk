# encoding: UTF-8

RM := rm -rf

CC := gcc
CC_FLAGS := -O2 -Wall -std=c99
CC_FLAGS += $(GLOBAL_CC_FLAGS)

UTILS_SRC_DIR := src
UTILS_OBJ_DIR := obj

AR := ar
AR_FLAGS := rcs

UTILS_LIB := utils
UTILS_LIB := $(addsuffix .a, $(addprefix lib, $(UTILS_LIB)))

# UTILS SOURCES WITHOUT DIRECTORIES's PREFIXES
UTILS_SRCS_NOTDIR := $(notdir $(wildcard $(UTILS_SRC_DIR)/*.c))

# UTILS OBJECTS
UTILS_OBJS := $(patsubst %.c, $(UTILS_OBJ_DIR)/%.o, $(UTILS_SRCS_NOTDIR))

# UTILS DEPENDENCIES
UTILS_DEPS := $(patsubst %.c, $(UTILS_OBJ_DIR)/%.d, $(UTILS_SRCS_NOTDIR))

build: $(UTILS_LIB)

clean:
	-$(RM) $(UTILS_OBJS) $(UTILS_DEPS) $(UTILS_LIB)

$(UTILS_OBJ_DIR)/%.o: $(UTILS_SRC_DIR)/%.c
	$(CC) -c $< $(CC_FLAGS) -MMD -o $@

$(UTILS_LIB): $(UTILS_OBJS)
	$(AR) $(AR_FLAGS) $@ $^

-include $(UTILS_DEPS)

.PHONY: build clean