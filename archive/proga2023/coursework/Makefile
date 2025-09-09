SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
APP_NAME = bmatcher
GDB = gdbtest

COMP = gcc
CFLAGS = -Wall -o

create:
	$(COMP) $(SRC_FILES) -g $(CFLAGS) $(APP_NAME)

clean:
	rm $(APP_NAME)