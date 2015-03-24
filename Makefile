CC=gcc
CFLAGS=-Wall
SOURCES=main tcp_ipv4_socket checksum tcp_flags tcp_messaging
OBJ_DIR=obj
SOURCE_FILES=$(SOURCES:%=%.c)
OBJECTS=$(SOURCES:%=$(OBJ_DIR)/%.o)
EXEC_NAME=tcp

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXEC_NAME)

$(OBJECTS): $(OBJ_DIR)/%.o : %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm $(EXEC_NAME)
