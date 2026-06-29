# STC89C52RC SDCC firmware build and stcgal flashing entry point.

PROJECT := stc89c52rc
TARGET ?= $(PROJECT)_demo

BUILD_DIR ?= build
SRC_DIR ?= src
INC_DIR ?= include
TEST_DIR ?= tests

F_CPU ?= 11059200UL
UART_BAUD ?= 9600UL
CODE_SIZE ?= 8192
IRAM_SIZE ?= 256
MCU_TESTS ?=

SDCC ?= sdcc
PACKIHX ?= packihx
HOST_CC ?= cc
HOST_CXX ?= c++
PKG_CONFIG ?= pkg-config
STCGAL ?= stcgal

ifeq ($(OS),Windows_NT)
  DETECTED_OS := Windows
  PYTHON ?= python
  HOST_EXE := .exe
  PORT ?= COM3
else
  UNAME_S := $(shell uname -s 2>/dev/null)
  PYTHON ?= python3
  HOST_EXE :=
  ifeq ($(UNAME_S),Darwin)
    DETECTED_OS := macOS
    PORT ?= /dev/cu.usbserial-0001
  else
    DETECTED_OS := Linux
    PORT ?= /dev/ttyUSB0
  endif
endif

MKDIR_P = $(PYTHON) -c "import os; os.makedirs(r'$(1)', exist_ok=True)"
RM_RF = $(PYTHON) -c "import shutil; shutil.rmtree(r'$(1)', ignore_errors=True)"
PRINT_FILE = $(PYTHON) -c "from pathlib import Path; p=Path(r'$(1)'); print(p.read_text() if p.exists() else 'missing: '+str(p), end='')"

MCU_TEST_NAMES := button crc8 gpio lcd led shift_register soft_i2c timer uart
ifeq ($(strip $(MCU_TESTS)),all)
  ENABLED_MCU_TESTS := $(MCU_TEST_NAMES)
else
  ENABLED_MCU_TESTS := $(strip $(MCU_TESTS))
endif

FIRMWARE_SOURCES := \
	$(SRC_DIR)/app/main.c \
	$(SRC_DIR)/cli/cli_commands.c \
	$(SRC_DIR)/cli/cli_print.c \
	$(SRC_DIR)/cli/test_registry.c \
	$(SRC_DIR)/drivers/button.c \
	$(SRC_DIR)/drivers/lcd1602.c \
	$(SRC_DIR)/drivers/led.c \
	$(sort $(wildcard $(SRC_DIR)/hal/*.c))

ifneq ($(strip $(ENABLED_MCU_TESTS)),)
  FIRMWARE_SOURCES += \
	$(SRC_DIR)/cli/test_utils.c \
	$(foreach test,$(ENABLED_MCU_TESTS),$(TEST_DIR)/mcu/test_$(test).c)
endif
ifneq ($(filter crc8,$(ENABLED_MCU_TESTS)),)
  FIRMWARE_SOURCES += $(SRC_DIR)/common/crc8.c
endif
ifneq ($(filter shift_register,$(ENABLED_MCU_TESTS)),)
  FIRMWARE_SOURCES += $(SRC_DIR)/drivers/shift_register_74hc595.c
endif
ifneq ($(filter soft_i2c,$(ENABLED_MCU_TESTS)),)
  FIRMWARE_SOURCES += $(SRC_DIR)/drivers/soft_i2c.c
endif
FIRMWARE_SOURCES := $(sort $(FIRMWARE_SOURCES))
FIRMWARE_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/obj/%.rel,$(FIRMWARE_SOURCES))

INCLUDES := -I$(INC_DIR)
DEFINES := -DF_CPU=$(F_CPU) -DUART_BAUD=$(UART_BAUD)
ifneq ($(filter button,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_BUTTON
endif
ifneq ($(filter crc8,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_CRC8
endif
ifneq ($(filter gpio,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_GPIO
endif
ifneq ($(filter lcd,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_LCD
endif
ifneq ($(filter led,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_LED
endif
ifneq ($(filter shift_register,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_SHIFT_REGISTER
endif
ifneq ($(filter soft_i2c,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_SOFT_I2C
endif
ifneq ($(filter timer,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_TIMER
endif
ifneq ($(filter uart,$(ENABLED_MCU_TESTS)),)
  DEFINES += -DSTC89C52RC_ENABLE_TEST_UART
endif

SDCCFLAGS ?= -mmcs51 --model-small --std-sdcc99 --opt-code-size
SDCCFLAGS += --code-size $(CODE_SIZE) --iram-size $(IRAM_SIZE) $(INCLUDES) $(DEFINES)

IHX := $(BUILD_DIR)/$(TARGET).ihx
HEX := $(BUILD_DIR)/$(TARGET).hex
MEM := $(BUILD_DIR)/$(TARGET).mem

HOST_COMMON_SOURCES := src/common/crc8.c src/common/ring_buffer.c
HOST_TEST_SOURCES := $(sort $(wildcard $(TEST_DIR)/host/*_test.cpp))
HOST_COMMON_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/tests/obj/%.o,$(HOST_COMMON_SOURCES))
HOST_TEST_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/tests/obj/%.o,$(HOST_TEST_SOURCES))
HOST_TEST_BIN := $(BUILD_DIR)/tests/gtest_runner$(HOST_EXE)
HOST_CFLAGS ?= -std=c99 -Wall -Wextra -I$(INC_DIR) -I$(TEST_DIR)/host
HOST_CXXFLAGS ?= -std=c++17 -Wall -Wextra -I$(INC_DIR) -I$(TEST_DIR)/host
GTEST_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags gtest 2>/dev/null)
GTEST_LIBS ?= $(shell $(PKG_CONFIG) --libs gtest_main gtest 2>/dev/null)
ifeq ($(strip $(GTEST_LIBS)),)
  GTEST_LIBS := -lgtest_main -lgtest -pthread
endif

STCGAL_PROTOCOL ?= stc89
STCGAL_BAUD ?= 115200
STCGAL_FLAGS ?= -t 5000
FLASH_IMAGE ?= $(HEX)

.PHONY: all firmware flash test clean size help print-config

all: firmware

firmware: $(HEX)

$(BUILD_DIR)/obj/%.rel: %.c
	$(call MKDIR_P,$(@D))
	$(SDCC) $(SDCCFLAGS) -c $< -o $@

$(IHX): $(FIRMWARE_OBJECTS)
	$(call MKDIR_P,$(@D))
	$(SDCC) $(SDCCFLAGS) $(FIRMWARE_OBJECTS) -o $@

$(HEX): $(IHX)
	$(PACKIHX) $< > $@

flash: $(HEX)
	$(STCGAL) -P $(STCGAL_PROTOCOL) -p $(PORT) -b $(STCGAL_BAUD) $(STCGAL_FLAGS) $(FLASH_IMAGE)

$(BUILD_DIR)/tests/obj/%.o: %.c
	$(call MKDIR_P,$(@D))
	$(HOST_CC) $(HOST_CFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/obj/%.o: %.cpp
	$(call MKDIR_P,$(@D))
	$(HOST_CXX) $(HOST_CXXFLAGS) $(GTEST_CFLAGS) -c $< -o $@

$(HOST_TEST_BIN): $(HOST_COMMON_OBJECTS) $(HOST_TEST_OBJECTS)
	$(call MKDIR_P,$(@D))
	$(HOST_CXX) $(HOST_COMMON_OBJECTS) $(HOST_TEST_OBJECTS) $(GTEST_LIBS) -o $@

test: $(HOST_TEST_BIN)
	$(HOST_TEST_BIN)

size: $(HEX)
	$(call PRINT_FILE,$(MEM))

print-config:
	@echo OS=$(DETECTED_OS)
	@echo TARGET=$(TARGET)
	@echo F_CPU=$(F_CPU)
	@echo UART_BAUD=$(UART_BAUD)
	@echo MCU_TESTS=$(if $(strip $(ENABLED_MCU_TESTS)),$(ENABLED_MCU_TESTS),none)
	@echo PORT=$(PORT)
	@echo STCGAL_PROTOCOL=$(STCGAL_PROTOCOL)
	@echo STCGAL_BAUD=$(STCGAL_BAUD)

clean:
	$(call RM_RF,$(BUILD_DIR))

help:
	@echo "Targets:"
	@echo "  make              Build $(HEX) with SDCC"
	@echo "  make MCU_TESTS=\"gpio led\""
	@echo "                    Build selected MCU test commands"
	@echo "  make MCU_TESTS=all"
	@echo "                    Build all MCU tests (may exceed 8 KB flash)"
	@echo "  make flash        Flash with stcgal, override PORT=..."
	@echo "  make test         Run GoogleTest host unit tests"
	@echo "  make size         Print SDCC memory report"
	@echo "  make clean        Remove build artifacts"
	@echo ""
	@echo "Useful overrides:"
	@echo "  PORT=/dev/ttyUSB0, PORT=/dev/cu.usbserial-0001, or PORT=COM3"
	@echo "  F_CPU=11059200UL UART_BAUD=9600UL TARGET=my_firmware"
