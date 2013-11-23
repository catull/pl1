# encoding: UTF-8

RM := rm -rf

CC := gcc
CC_FLAGS := -O2 -Wall -std=c99
CC_FLAGS += $(GLOBAL_CC_FLAGS)

AR := ar
AR_FLAGS := rcs

UTILS_LIB := utils
UTILS_LIB := $(addsuffix .a, $(addprefix lib, $(UTILS_LIB)))

UTILS_SRCS := $(wildcard *.c)
UTILS_OBJS := $(patsubst %.c, %.o, $(UTILS_SRCS))
UTILS_DEPS := $(patsubst %.c, %.d, $(UTILS_SRCS))

build: $(UTILS_LIB)

clean:
	-$(RM) $(UTILS_OBJS) $(UTILS_DEPS) $(UTILS_LIB)

%.o: %.c
	@$(CC) -E $< $(CC_FLAGS) -MMD 1> /dev/null
	$(CC) -c $< $(CC_FLAGS) -o $@

$(UTILS_LIB): $(UTILS_OBJS)
	$(AR) $(AR_FLAGS) $@ $^

-include $(UTILS_DEPS)

.PHONY: build clean