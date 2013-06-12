ABSLOAD_BIN := $(BIN_DIR)/plcmp

ABSLOAD_INC_DIR := $(ABSLOAD_DIR)/inc
ABSLOAD_SRC_DIR := $(ABSLOAD_DIR)/src
ABSLOAD_OBJ_DIR := $(ABSLOAD_DIR)/obj
ABSLOAD_DEP_DIR := $(ABSLOAD_DIR)/obj

# PL COMPILER INCLUDES
ABSLOAD_INCLUDES := $(ABSLOAD_INC_DIR)
ABSLOAD_INCLUDES += 

# PL COMPILER DEFINITIONS
ABSLOAD_DEFINITIONS := 
ABSLOAD_DEFINITIONS += 

# PL COMPILER SOURCES
ABSLOAD_SRCS := $(wildcard $(ABSLOAD_SRC_DIR)/*.c)
ABSLOAD_SRCS_NOTDIR := $(notdir $(ABSLOAD_SRCS))

# PL COMPILER OBJECTS
ABSLOAD_OBJS := $(patsubst %.c, $(ABSLOAD_OBJ_DIR)/%.o, $(ABSLOAD_SRCS_NOTDIR))
ABSLOAD_OBJS_NOTDIR := $(notdir $(ABSLOAD_OBJS))

# PL COMPILER DEPENDENCIES
ABSLOAD_DEPS := $(patsubst %.o, $(ABSLOAD_DEP_DIR)/%.d, $(ABSLOAD_OBJS_NOTDIR))
ABSLOAD_DEPS_NOTDIR := $(notdir $(ABSLOAD_DEPS))

$(ABSLOAD_OBJ_DIR)/%.o: $(ABSLOAD_SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS) -I $(ABSLOAD_INCLUDES) -MMD -o $@

-include $(ABSLOAD_DEPS)