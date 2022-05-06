.PHONY: clean all

CC = g++
CFLAGS := -ldns_sd -pthread

OBJ_FILE := $(wildcard )
INC_DIR := inc
SRC_DIR := src
OBJ_DIR := obj

C_FILE := $(wildcard $(SRC_DIR)/*.cc)

pub: pub.cc $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o pub pub.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)

sub: sub.cc $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o sub sub.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)

client: client1 client2

client1: client1.cc $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o client1 client1.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)

client2: client2.cc $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(C_FILE))
	$(CC) -o client2 client2.cc $(wildcard $(OBJ_DIR)/*.o) -I$(INC_DIR) $(CFLAGS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -o $@ -c $(patsubst $(OBJ_DIR)/%.o, $(SRC_DIR)/%.cc, $@) -I$(INC_DIR) $(CFLAGS)

remake: clean all

clean: clean-test clean-client
	rm -rf obj/*
	rm -rf sub pub

clean-test:
	rm -rf test

clean-client:
	rm -rf client1 client2
