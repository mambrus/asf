.PHONY: all clean flash info start stop debug

BASE ?= sam/drivers/xdmac/example
BOARD_DIR := same70q21_same70_xplained/gcc
BUILD := ${BASE}/${BOARD_DIR}
PROJECT := $(shell echo ${BASE} | awk -F'/' '{print $$(NF-1)}')
FLASH_FILE := ${BUILD}/${PROJECT}_example_flash.bin
ELF_FILE := ${BUILD}/${PROJECT}_example_flash.elf

VERBOSE  ?= 0
ifneq ("$(VERBOSE)","1")
export VERBOSE

.SILENT :
endif

all:
	bear make -C ${BUILD}

clean:
	make -C ${BUILD} clean

start:
	ocdctrl.sh gdbserver start

stop:
	ocdctrl.sh gdbserver stop

flash: all
	ocdctrl.sh flash "${FLASH_FILE}"

#debug: flash

debug:
	kdbg -r :3333 ${ELF_FILE}

info:
	@echo "PROJECT:             ${PROJECT}"
	@echo "FLASH_FILE:          ${FLASH_FILE}"
	@echo "BUILD:               ${BUILD}"

