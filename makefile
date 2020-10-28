.PHONY: all clean flash info start stop debug cscope

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

all: cscope.out
	bear make -C ${BUILD}

# Use ack and pattens defined in .ackrc as filters as double-inverted
# matches are much easier to define as follows:

#--ignore-directory=is:sams70
#--ignore-directory=is:samv70
#--ignore-directory=is:samv71
#--ignore-directory=is:avr32
#--ignore-directory=is:mega
#--ignore-directory=is:xmega
#--ignore-directory=match:sam[^e].*
#--ignore-directory=match:at(x)?mega.*
#--ignore-directory=match:at32.*
#--ignore-directory=match:(x)?mega.*

cscope.files:
	ack --cc -f > cscope.files

cscope.out: cscope.files
	cscope -b -q -k

cscope: cscope.out

clean:
	make -C ${BUILD} clean
	-rm cscope.out
	-rm cscope.files

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

