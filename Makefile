TARGET = darneureka

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include

# Select platform
ifeq ($(shell uname -s),Darwin)
	PLATFORM := Darwin
	CC := clang

	GLFW_INCLUDE_DIR := /opt/homebrew/opt/glfw/include
	GLFW_LIB_DIR := /opt/homebrew/opt/glfw/lib

	CFLAGS := -O2 -Wall -Wextra -MMD -MP -I$(INCLUDE_DIR) -I$(GLFW_INCLUDE_DIR)
	LDFLAGS := -L$(GLFW_LIB_DIR) -lglfw -ldl -framework OpenGL -framework Cocoa -framework Foundation
endif

ifeq ($(shell uname -s),Linux)
	PLATFORM := Linux
	CC := gcc

	CFLAGS := -O2 -Wall -Wextra -MMD -MP -I$(INCLUDE_DIR)
	LDFLAGS := -lglfw -ldl -lGL -lm
endif

PLATFORM_DIR := platform/$(PLATFORM)

SRCS := $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)
PLAT_SRC := $(shell find $(PLATFORM_DIR) -name '*.c' 2>/dev/null)

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(SRC_DIR)/%.o, $(SRCS))
PLAT_OBJ := $(patsubst $(PLATFORM_DIR)/%.c, $(BUILD_DIR)/$(PLATFORM_DIR)/%.o, $(PLAT_SRC))
DEPS := $(OBJS:.o=.d) $(PLAT_OBJ:.o=.d)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS) $(PLAT_OBJ)
	mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< $(CFLAGS) -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

# TARGET = darneureka
#
# SRC_DIR := src
# BUILD_DIR = build
# BIN_DIR = bin
# INCLUDE_DIR = include
#
# # This selects the paltform
# ifeq ($(shell uname -s),Darwin)
# 	PLATFORM := Darwin
# 	CC := clang
#
# 	GLFW_INCLUDE_DIR := /opt/homebrew/opt/glfw/include
# 	GLFW_LIB_DIR := /opt/homebrew/opt/glfw/lib
#
# 	CFLAGS = -O2 -Wall -Wextra -I$(INCLUDE_DIR) -I$(GLFW_INCLUDE_DIR)
#
# 	LDFLAGS = -L$(GLFW_LIB_DIR) -lglfw -ldl -framework OpenGL -framework Cocoa -framework Foundation
# endif
#
# ifeq ($(shell uname -s),Linux)
# 	PLATFORM := Linux
# 	CC := gcc
#
# 	CFLAGS = -O2 -Wall -Wextra -I$(INCLUDE_DIR)
# 	LDFLAGS = -lglfw -ldl -lGL -lm
# endif
#
# PLATFORM_DIR = platform/$(PLATFORM)
#
# SRCS := $(wildcard $(SRC_DIR)/*.c)
# PLAT_SRC := $(wildcard $(PLATFORM_DIR)/*.c)
#
# OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
# PLAT_OBJ := $(patsubst $(PLATFORM_DIR)/%.c, $(BUILD_DIR)/%.o, $(PLAT_SRC))
#
# all: $(TARGET)
#
# $(TARGET): $(OBJS) $(PLAT_OBJ)
# 	$(CC) $^ $(CT_FLAGS) -o $(BUILD_DIR)/$(TARGET)
#
# # $(PLAT_OBJ): $(PLAT_SRC)
# # 	$(CC) -c $< $(CFLAGS) -o $@
# #
# # $(OBJS): $(SRCS)
# # 	mkdir -p $(BUILD_DIR)
# # 	$(CC) -c $< $(CFLAGS) -o $@
#
# $(BUILD_DIR)/%.o: $(PLATFORM_DIR)/%.c
# 	mkdir -p $(BUILD_DIR)
# 	$(CC) -c $< $(CFLAGS) -o $@
#
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
# 	mkdir -p $(BUILD_DIR)
# 	$(CC) -c $< $(CFLAGS) -o $@
#
# clean:
# 	rm -rf $(BUILD_DIR)
#
# .PHONY: all clean
