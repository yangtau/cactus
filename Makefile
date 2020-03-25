CC = gcc
CFLAGS += -std=gnu99 -g -Wall 
INCLUDE = -Icommon

MKDIR = mkdir -p

bin = bin
src = $(wildcard ./*/*.c)
objs = $(patsubst %.c, %.o, $(src))

.PHONY: all
all: $(objs)

scanner-test: $(objs)
	$(CC) $(CFLAGS) -o $(bin)/scanner-test $(bin)/scanner.o $(bin)/scanner_test.o 
	$(bin)/scanner-test

$(objs):%.o: %.c $(bin)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(bin)/$(notdir $@)

$(bin):
	$(MKDIR) $(bin)

.PHONY: clean
clean:
	rm -rf $(bin)/*
