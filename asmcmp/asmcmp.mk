# encoding: UTF-8

RM := rm -rf

CC := gcc

CC_FLAGS := -O2 -Wall -std=c99
CC_FLAGS += $(GLOBAL_CC_FLAGS)

ASMCMP_BIN := bin/asmcmp

ASMCMP_INC_DIR := inc
ASMCMP_SRC_DIR := src
ASMCMP_OBJ_DIR := obj
ASMCMP_DEP_DIR := dep

# ASSEMBLER COMPILER INCLUDES
ASMCMP_INCLUDES := -I $(ASMCMP_INC_DIR)
ASMCMP_INCLUDES += 

# ASSEMBLER COMPILER DEFINITIONS
ASMCMP_DEFINITIONS := 
ASMCMP_DEFINITIONS += 

# ASSEMBLER COMPILER SOURCES WITHOUT DIRECTORIES
ASMCMP_SRCS_NOTDIR := $(notdir $(wildcard $(ASMCMP_SRC_DIR)/*.c))

# ASSEMBLER COMPILER OBJECTS
ASMCMP_OBJS := $(patsubst %.c, $(ASMCMP_OBJ_DIR)/%.o, $(ASMCMP_SRCS_NOTDIR))

# ASSEMBLER COMPILER DEPENDENCIES
ASMCMP_DEPS := $(patsubst %.c, $(ASMCMP_DEP_DIR)/%.d, $(ASMCMP_SRCS_NOTDIR))

-include $(ASMCMP_DEPS)

build: $(ASMCMP_OBJS)
	$(CC) $(ASMCMP_OBJS) -o $(ASMCMP_BIN)
clean:
	-$(RM) $(ASMCMP_OBJS) $(ASMCMP_DEPS) $(ASMCMP_BIN)

$(ASMCMP_OBJ_DIR)/%.o: $(ASMCMP_SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS) $(ASMCMP_INCLUDES) -MM -MF \
$(addprefix $(ASMCMP_DEP_DIR)/, $(patsubst $(ASMCMP_OBJ_DIR)/%.o, %.d, $@))
	$(CC) -c $< $(FLAGS) $(ASMCMP_INCLUDES) -o $@

