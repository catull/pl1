# encoding: UTF-8

RM := rm -rf

CC := gcc

CC_FLAGS := -O2 -Wall -std=c99
CC_FLAGS += $(GLOBAL_CC_FLAGS)

ABSLOAD_BIN := bin/absload

ABSLOAD_INC_DIR := inc
ABSLOAD_SRC_DIR := src
ABSLOAD_OBJ_DIR := obj
ABSLOAD_DEP_DIR := dep

# ABSLOAD INCLUDES
ABSLOAD_INCLUDES := -I $(ABSLOAD_INC_DIR)
ABSLOAD_INCLUDES += 

# ABSLOAD DEFINITIONS
ABSLOAD_DEFINITIONS := 
ABSLOAD_DEFINITIONS += 

# ABSLOAD SOURCES
ABSLOAD_SRCS_NOTDIR := $(notdir $(wildcard $(ABSLOAD_SRC_DIR)/*.c))

# ABSLOAD OBJECTS
ABSLOAD_OBJS := $(patsubst %.c, $(ABSLOAD_OBJ_DIR)/%.o, $(ABSLOAD_SRCS_NOTDIR))

# ABSLOAD DEPENDENCIES
ABSLOAD_DEPS := $(patsubst %.c, $(ABSLOAD_DEP_DIR)/%.d, $(ABSLOAD_OBJS_NOTDIR))

-include $(ABSLOAD_DEPS)

build: $(ABSLOAD_OBJS)
	$(CC) -lncurses $(ABSLOAD_OBJS) -o $(ABSLOAD_BIN)
clean:
	-$(RM) $(ABSLOAD_OBJS) $(ABSLOAD_DEPS) $(ABSLOAD_BIN)

$(ABSLOAD_OBJ_DIR)/%.o: $(ABSLOAD_SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS) $(ABSLOAD_INCLUDES) -MM -MF \
$(addprefix $(ABSLOAD_DEP_DIR)/, $(patsubst $(ABSLOAD_OBJ_DIR)/%.o, %.d, $@))
	$(CC) -c $< $(FLAGS) $(ABSLOAD_INCLUDES) -o $@