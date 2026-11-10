# parabola_demo cross-platform Makefile
# Build:   make
# Install: make install
# Clean:   make clean

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TARGET_NAME := parabola_demo
LDLIBS := -lm

WINDOWS_BUILD :=
ifneq (,$(findstring mingw,$(MAKE_HOST)))
WINDOWS_BUILD := 1
endif
ifneq (,$(findstring windows,$(MAKE_HOST)))
WINDOWS_BUILD := 1
endif
ifeq ($(OS),Windows_NT)
WINDOWS_BUILD := 1
endif

ifeq ($(WINDOWS_BUILD),1)
SHELL := cmd.exe
.SHELLFLAGS := /C
CC = gcc
EXE_EXT := .exe
CHARSET_FLAGS := -finput-charset=UTF-8 -fexec-charset=GBK
MKDIR_BUILD = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
MKDIR_BIN = if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
COPY_BIN = copy /Y "$(subst /,\,$(TARGET))" "$(subst /,\,$(INSTALL_TARGET))" >NUL
CLEAN_CMD = if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)" & if exist "$(BIN_DIR)" rmdir /S /Q "$(BIN_DIR)"
else
CC = cc
EXE_EXT :=
CHARSET_FLAGS :=
MKDIR_BUILD = mkdir -p "$(BUILD_DIR)"
MKDIR_BIN = mkdir -p "$(BIN_DIR)"
COPY_BIN = cp -f "$(TARGET)" "$(INSTALL_TARGET)"
CLEAN_CMD = rm -rf "$(BUILD_DIR)" "$(BIN_DIR)"
endif

CFLAGS ?= -std=c99 -Wall -O2
override CFLAGS += $(CHARSET_FLAGS)
override CPPFLAGS += -I$(INC_DIR)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET := $(BUILD_DIR)/$(TARGET_NAME)$(EXE_EXT)
INSTALL_TARGET := $(BIN_DIR)/$(TARGET_NAME)$(EXE_EXT)

.PHONY: all install clean rebuild

all: $(TARGET)

$(BUILD_DIR):
	$(MKDIR_BUILD)

$(BIN_DIR):
	$(MKDIR_BIN)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRC_DIR)/$*.c -o $(BUILD_DIR)/$*.o

install: $(TARGET) | $(BIN_DIR)
	$(COPY_BIN)

clean:
	$(CLEAN_CMD)

rebuild: clean all
