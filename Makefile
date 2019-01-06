SHELL := /bin/bash

CC := gcc

CFLAGS := -O1 -std=c99 -pipe
WFLAGS := -Wall -Werror
LDFLAGS := -lm -lncurses -lcurl

PATH-PROJECT := $(shell pwd)
PATH-BIN := $(PATH-PROJECT)/bin
PATH-DOWNLOADS := $(PATH-PROJECT)/downloads
PATH-SRC := $(PATH-PROJECT)/src
PATH-OBJECTS := $(PATH-BIN)

.DEFAULT: default
.PHONY: all default build
.PHONY: all clean

default: build
build: ensure-dirs wscu

clean:
	rm -rvf \
		$(PATH-OBJECTS)/* \
		$(PATH-DOWNLOADS)/*

ensure-dirs:
	mkdir -p $(PATH-BIN)
	mkdir -p $(PATH-DOWNLOADS)

$(PATH-OBJECTS)/main.o: $(PATH-SRC)/main.c
	$(CC) $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

$(PATH-OBJECTS)/get-tmux.o: $(PATH-SRC)/get-tmux.c
	$(CC) $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@ 

wscu: \
	$(PATH-OBJECTS)/main.o \
	$(PATH-OBJECTS)/get-tmux.o
	$(CC) $(CFLAGS) $(WFLAGS) -o $(PATH-BIN)/$@ $+ $(LDFLAGS)
