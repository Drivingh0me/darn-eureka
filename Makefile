TARGET = darneureka

SRC_DIR := src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# This selects the paltform
ifeq ($(shell uname -s),Darwin)
	PLATFORM := Darwin
	CC := clang

	GLFW_INCLUDE_DIR := /opt/homebrew/opt/glfw/include
	GLFW_LIB_DIR := /opt/homebrew/opt/glfw/lib

	CFLAGS = -O2 -Wall -Wextra -I$(INCLUDE_DIR) -I$(GLFW_INCLUDE_DIR)

	LDFLAGS = -L$(GLFW_LIB_DIR) -lglfw -ldl -framework OpenGL -framework Cocoa -framework Foundation
endif

ifeq ($(shell uname -s),Linux)
	PLATFORM := Linux
	CC := gcc

	CFLAGS = -O2 -Wall -Wextra -I$(INCLUDE_DIR)
	LDFLAGS = -lglfw -ldl -lGL -lm
endif

PLATFORM_DIR = platform/$(PLATFORM)

SRCS := $(wildcard $(SRC_DIR)/*.c)
PLAT_SRC := $(wildcard $(PLATFORM_DIR)/*.c)

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
PLAT_OBJ := $(patsubst $(PLATFORM_DIR)/%.c, $(BUILD_DIR)/%.o, $(PLAT_SRC))

all: $(TARGET)

$(TARGET): $(OBJS) $(PLAT_OBJ)
	$(CC) $^ $(CT_FLAGS) -o $(BUILD_DIR)/$(TARGET)

# $(PLAT_OBJ): $(PLAT_SRC)
# 	$(CC) -c $< $(CFLAGS) -o $@
#
# $(OBJS): $(SRCS)
# 	mkdir -p $(BUILD_DIR)
# 	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/%.o: $(PLATFORM_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
