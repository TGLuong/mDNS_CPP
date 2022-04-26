.PHONY: clean all

CC = g++
CFLAGS := -ldns_sd

OBJ_FILE := $(wildcard )
INC_DIR := inc
SRC_DIR := src
OBJ_DIR := obj

C_FILE := $(wildcard $(SRC_DIR)/*.cc)

pub: $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o pub pub.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)

sub: $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o sub sub.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -o $@ -c $(patsubst $(OBJ_DIR)/%.o, $(SRC_DIR)/%.cc, $@) -I$(INC_DIR) $(CFLAGS)

remake: clean all

clean:
	rm -rf obj/*
	rm -rf main