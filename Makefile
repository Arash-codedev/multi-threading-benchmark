IS_LINUX:=0

ifeq ($(OS),Windows_NT) #if windows
	CCFLAGS += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		CCFLAGS += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux) #if Linux
		CCFLAGS += -D LINUX
		IS_LINUX:=1
	endif
	ifeq ($(UNAME_S),Darwin) #if OSX
		CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CCFLAGS += -D ARM
	endif
endif

LIBS:= -lboost_regex -lboost_system -lboost_thread -lboost_filesystem -lboost_wave -lboost_date_time -lboost_chrono -lboost_atomic -larmadillo
LIBS+= -lGL -lGLU -lglut -lGLEW -lSDL -lSDL2main -lSDL2

INCLUDE_DIR:= 

export GCC_COLORS='error=01;31:warning=01;33:note=01;36:caret=01;32:locus=01;32:quote=0'

ifeq ($(IS_LINUX),1)
	#
else
	LIBS+= -L'C:\cygwin\usr\local\lib\'
	INCLUDE_DIR+= -I'C:\cygwin\usr\local\include\' -I'C:\cygwin\usr\include'
endif

CXX:=g++
BIN:=bin

DEBUG_FLAG:= -g -O3
RELESE_FLAG:= -O3 -s -DNDEBUG -DARMA_NO_DEBUG
CURRENT_FLAGS:= $(DEBUG_FLAG)
CURRENT_FLAGS +=-fdiagnostics-color=always
CURRENT_FLAGS +=-pthread

.PHONY: test

all:
	$(CXX) main.cpp -std=c++11 $(CURRENT_FLAGS) -o $(BIN)/main $(INCLUDE_DIR) -Wall -Wconversion $(LIBS) -Wfatal-errors -Wextra

run:
	./bin/main

build:
	$(CXX) main.cpp -std=c++11 $(CURRENT_FLAGS) -o $(BIN)/main $(INCLUDE_DIR) -Wall -Wconversion $(LIBS) -Wfatal-errors -Wextra

test:
	g++ test.cpp -o bin/test -O0  -std=c++11 -g -Wall -Wconversion -larmadillo  -Wfatal-errors -Wconversion
