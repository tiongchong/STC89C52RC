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

SDCC ?= sdcc
PACKIHX ?= packihx
HOST_CC ?= cc
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

FIRMWARE_SOURCES := $(sort $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c))
FIRMWARE_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/obj/%.rel,$(FIRMWARE_SOURCES))

INCLUDES := -I$(INC_DIR)
DEFINES := -DF_CPU=$(F_CPU) -DUART_BAUD=$(UART_BAUD)

SDCCFLAGS ?= -mmcs51 --model-small --std-sdcc99 --opt-code-size
SDCCFLAGS += --code-size $(CODE_SIZE) --iram-size $(IRAM_SIZE) $(INCLUDES) $(DEFINES)

IHX := $(BUILD_DIR)/$(TARGET).ihx
HEX := $(BUILD_DIR)/$(TARGET).hex
MEM := $(BUILD_DIR)/$(TARGET).mem

HOST_TEST_SOURCES := $(sort $(wildcard $(TEST_DIR)/host/*.c)) \
	src/common/crc8.c \
	src/common/ring_buffer.c
HOST_TEST_BIN := $(BUILD_DIR)/tests/test_runner$(HOST_EXE)
HOST_CFLAGS ?= -std=c99 -Wall -Wextra -I$(INC_DIR) -I$(TEST_DIR)/host

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

$(HOST_TEST_BIN): $(HOST_TEST_SOURCES)
	$(call MKDIR_P,$(@D))
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_TEST_SOURCES) -o $@

test: $(HOST_TEST_BIN)
	$(HOST_TEST_BIN)

size: $(HEX)
	$(call PRINT_FILE,$(MEM))

print-config:
	@echo OS=$(DETECTED_OS)
	@echo TARGET=$(TARGET)
	@echo F_CPU=$(F_CPU)
	@echo UART_BAUD=$(UART_BAUD)
	@echo PORT=$(PORT)
	@echo STCGAL_PROTOCOL=$(STCGAL_PROTOCOL)
	@echo STCGAL_BAUD=$(STCGAL_BAUD)

clean:
	$(call RM_RF,$(BUILD_DIR))

help:
	@echo "Targets:"
	@echo "  make              Build $(HEX) with SDCC"
	@echo "  make flash        Flash with stcgal, override PORT=..."
	@echo "  make test         Run host unit tests"
	@echo "  make size         Print SDCC memory report"
	@echo "  make clean        Remove build artifacts"
	@echo ""
	@echo "Useful overrides:"
	@echo "  PORT=/dev/ttyUSB0, PORT=/dev/cu.usbserial-0001, or PORT=COM3"
	@echo "  F_CPU=11059200UL UART_BAUD=9600UL TARGET=my_firmware"
