#	SRC_DIR: folder locaion that holds all source code files
#	BUILD_DIR: folder location where the compiled program will end up
#	CC: compiler to use
#	SRC_FILES: *.c compiles all, if you want to compile one file at a time use 'TITLE.c'
#	OBJ_NAME: the name you want your compiled file to have
#	INCLUDE_PATHS: location of all the include files
#	LIBRARY_PATHS: location of all the library files
#	COMPILER_FLAGS: flags to use through terminal
#	LINKER_FLAGS: used to link a library or other source to the program
# 	all: full terminal command that will be executed to compile the program 


SRC_DIR = src
BUILD_DIR = build/debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/physics.c)
OBJ_NAME = physics
INCLUDE_PATHS = -I/opt/homebrew/Cellar/sdl2/2.30.11/include
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.30.11/lib/ -L/opt/homebrew/Cellar/sdl2_image/2.8.4/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib
COMPILER_FLAGS = -std=c17 -Wall -O0 -g
LINKER_FLAGS = -lsdl2

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)
