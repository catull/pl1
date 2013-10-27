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
ASMCMP_INCLUDES := $(ASMCMP_INC_DIR) $(PL1_UTILS_DIR)
ASMCMP_INCLUDES := $(addprefix -I, $(ASMCMP_INCLUDES))

# ASSEMBLER COMPILER DEFINITIONS
# ASMCMP_DEFINITIONS :=
# ASMCMP_DEFINITIONS +=

# ASSEMBLER COMPILER LIBS DIR
ASMCMP_LIB_DIRS := $(PL1_UTILS_DIR)
ASMCMP_LIB_DIRS := $(addprefix -L, $(ASMCMP_LIB_DIRS))

# ASSEMBLER COMPILER STATIC LIBRARIES
ASMCMP_STAT_LIBS := utils
ASMCMP_STAT_LIBS := $(addprefix -l, $(ASMCMP_STAT_LIBS))

# ASSEMBLER COMPILER SOURCES WITHOUT DIRECTORIES
ASMCMP_SRCS_NOTDIR := $(notdir $(wildcard $(ASMCMP_SRC_DIR)/*.c))

# ASSEMBLER COMPILER OBJECTS
ASMCMP_OBJS := $(patsubst %.c, $(ASMCMP_OBJ_DIR)/%.o, $(ASMCMP_SRCS_NOTDIR))

# ASSEMBLER COMPILER DEPENDENCIES
ASMCMP_DEPS := $(patsubst %.c, $(ASMCMP_DEP_DIR)/%.d, $(ASMCMP_SRCS_NOTDIR))

build: $(ASMCMP_OBJS)
	$(CC) $^ $(ASMCMP_LIB_DIRS) $(ASMCMP_STAT_LIBS) -o $(ASMCMP_BIN)
clean:
	-$(RM) $(ASMCMP_OBJS) $(ASMCMP_DEPS) $(ASMCMP_BIN)

$(ASMCMP_OBJ_DIR)/%.o: $(ASMCMP_SRC_DIR)/%.c
	@$(CC) -c $< $(CC_FLAGS) $(ASMCMP_INCLUDES) -MM -MT $@ -MF \
$(addprefix $(ASMCMP_DEP_DIR)/, $(patsubst $(ASMCMP_OBJ_DIR)/%.o, %.d, $@))
	$(CC) -c $< $(CC_FLAGS) $(ASMCMP_INCLUDES) -o $@

-include $(ASMCMP_DEPS)

.PHONY: build clean