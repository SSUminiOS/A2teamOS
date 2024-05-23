# Compiler and Compiler Flags
CC=gcc
CFLAGS=-Wall -g -Iinclude
# Linker flags
LDFLAGS=-lreadline -lm

# The build target executable:
TARGET=minios

# Source, Object files
DIR_modules = kernel/modules

SRCS=kernel/kernel.c kernel/system.c $(wildcard $(DIR_modules)/*.c)
OBJS=$(SRCS:.c=.o) 

# Include directory
INCLUDE_DIR=include

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# To obtain object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up:
clean:
	rm -f $(OBJS) $(TARGET)
