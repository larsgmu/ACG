# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

#We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -rf *.dSYM
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

UNITY_ROOT=../
C_COMPILER=gcc
#CFLAGS=-std=c89 #Cant have this on the elevator sources
CFLAGS=-std=gnu11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -g
#CFLAGS += -std=gnu11
#CFLAGS += -Wno-misleading-indentation

TARGET_BASE1=testRequests
TARGET_BASE2=testTimer
TARGET_BASE3=testFsm
TARGET1 = $(TARGET_BASE1)$(TARGET_EXTENSION)
TARGET2 = $(TARGET_BASE2)$(TARGET_EXTENSION)
TARGET3 = $(TARGET_BASE3)$(TARGET_EXTENSION)
SRC_FILES1=$(UNITY_ROOT)/src/unity.c test/testRequests.c $(patsubst src/%, %, $(shell find . -name '*.c'  ! -name '*test*' ! -name '*main*'))  test/test_runners/testRequests_Runner.c
SRC_FILES2=$(UNITY_ROOT)/src/unity.c test/testTimer.c $(patsubst src/%, %, $(shell find . -name '*.c'  ! -name '*test*' ! -name '*main*'))  test/test_runners/testTimer_Runner.c
SRC_FILES3=$(UNITY_ROOT)/src/unity.c test/testFsm.c $(patsubst src/%, %, $(shell find . -name '*.c'  ! -name '*test*' ! -name '*main*'))  test/test_runners/testFsm_Runner.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src
SYMBOLS=

all: clean default

default: $(SRC_FILES1) $(SRC_FILES2) $(SRC_FILES3)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES1) -o $(TARGET1)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES2) -o $(TARGET2)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES3) -o $(TARGET3)
	- ./$(TARGET1)
	- ./$(TARGET2)
	- ./$(TARGET3)

test/test_runners/testRequests_Runner.c: test/testRequests.c
	ruby $(UNITY_ROOT)/unity_scripts/generate_test_runner.rb test/testRequests.c  test/test_runners/testRequests_Runner.c
test/test_runners/testTimer_Runner.c: test/testTimer.c
	ruby $(UNITY_ROOT)/unity_scripts/generate_test_runner.rb test/testTimer.c  test/test_runners/testTimer_Runner.c
test/test_runners/testFsm_Runner.c: test/testFsm.c
	ruby $(UNITY_ROOT)/unity_scripts/generate_test_runner.rb test/testFsm.c  test/test_runners/testFsm_Runner.c


clean:
	$(CLEANUP) $(TARGET1) $(TARGET2) $(TARGET3)

ci: CFLAGS += -Werror
ci: default
