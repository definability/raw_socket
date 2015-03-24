CC=gcc
CFLAGS=-Wall
S_DIR=socket
S_SRC=tcp_ipv4_socket 
T_DIR=tcp
T_SRC=tcp_flags tcp_messaging tcp_commands
U_DIR=utils
U_SRC=checksum addresses
SOURCES=main $(S_SRC:%=$(S_DIR)/%) $(T_SRC:%=$(T_DIR)/%) $(U_SRC:%=$(U_DIR)/%)
OBJ_DIR=obj
OBJ_DIRS=$(OBJ_DIR)/$(S_DIR) $(OBJ_DIR)/$(T_DIR) $(OBJ_DIR)/$(U_DIR)
SOURCE_FILES=$(SOURCES:%=%.c)
OBJECTS=$(SOURCES:%=$(OBJ_DIR)/%.o)
EXEC_NAME=raw_tcp

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXEC_NAME)

$(OBJECTS): $(OBJ_DIR)/%.o : %.c
	@mkdir -p $(OBJ_DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm $(EXEC_NAME)
