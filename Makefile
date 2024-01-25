CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/inventory_bst.c $(SRC_DIR)/order_queue.c $(SRC_DIR)/undo_stack.c $(SRC_DIR)/inventory_sort.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXEC = $(BIN_DIR)/OmniStock.exe

all: $(BIN_DIR) $(EXEC)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*
