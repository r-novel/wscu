SHELL := /bin/bash

PATH-PROJECT := $(shell pwd)
PATH-BIN := $(PATH-PROJECT)/bin
PATH-SRC := $(PATH-PROJECT)/src
PATH-INCLUDE := $(PATH-PROJECT)/include
PATH-OBJECTS := $(PATH-BIN)

CC := gcc

CFLAGS := -O1 -std=c99 -pipe -I$(PATH-INCLUDE)
WFLAGS := -Wall -Werror
LDFLAGS := -lncurses -lcurl -lyaml

.DEFAULT: default
.PHONY: all default build
.PHONY: all clean

default: build
build: project-name ensure-dirs wscu

project-name: 
	@echo \
		-e \
		"\x1B[1;33m \\n" \
		"WWWWWWWW                           WWWWWWWW   SSSSSSSSSSSSSSS         CCCCCCCCCCCCCUUUUUUUU     UUUUUUUU \\n" \
		"W::::::W                           W::::::W SS:::::::::::::::S     CCC::::::::::::CU::::::U     U::::::U \\n" \
		"W::::::W                           W::::::WS:::::SSSSSS::::::S   CC:::::::::::::::CU::::::U     U::::::U \\n" \
		"W::::::W                           W::::::WS:::::S     SSSSSSS  C:::::CCCCCCCC::::CUU:::::U     U:::::UU \\n" \
		" W:::::W           WWWWW           W:::::W S:::::S             C:::::C       CCCCCC U:::::U     U:::::U  \\n" \
		"  W:::::W         W:::::W         W:::::W  S:::::S            C:::::C               U:::::D     D:::::U  \\n" \
		"   W:::::W       W:::::::W       W:::::W    S::::SSSS         C:::::C               U:::::D     D:::::U  \\n" \
		"    W:::::W     W:::::::::W     W:::::W      SS::::::SSSSS    C:::::C               U:::::D     D:::::U  \\n" \
		"     W:::::W   W:::::W:::::W   W:::::W         SSS::::::::SS  C:::::C               U:::::D     D:::::U  \\n" \
		"      W:::::W W:::::W W:::::W W:::::W             SSSSSS::::S C:::::C               U:::::D     D:::::U  \\n" \
		"       W:::::W:::::W   W:::::W:::::W                   S:::::SC:::::C               U:::::D     D:::::U  \\n" \
		"        W:::::::::W     W:::::::::W                    S:::::S C:::::C       CCCCCC U::::::U   U::::::U  \\n" \
		"         W:::::::W       W:::::::W         SSSSSSS     S:::::S  C:::::CCCCCCCC::::C U:::::::UUU:::::::U  \\n" \
		"          W:::::W         W:::::W          S::::::SSSSSS:::::S   CC:::::::::::::::C  UU:::::::::::::UU   \\n" \
		"           W:::W           W:::W           S:::::::::::::::SS      CCC::::::::::::C    UU:::::::::UU     \\n" \
		"            WWW             WWW             SSSSSSSSSSSSSSS           CCCCCCCCCCCCC      UUUUUUUUU       \\n" \
		"\033[0m \\n"

clean:
	rm -rvf \
		$(PATH-OBJECTS)/*

ensure-dirs:
	mkdir -p $(PATH-BIN)

$(PATH-OBJECTS)/main.o: $(PATH-SRC)/main.c
	$(CC) "-D__WSCUFILE__=\"`basename $<`\"" $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

$(PATH-OBJECTS)/dl.o: $(PATH-SRC)/dl.c
	$(CC) "-D__WSCUFILE__=\"`basename $<`\"" $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

$(PATH-OBJECTS)/config.o: $(PATH-SRC)/config.c
	$(CC) "-D__WSCUFILE__=\"`basename $<`\"" $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

$(PATH-OBJECTS)/dir.o: $(PATH-SRC)/dir.c
	$(CC) "-D__WSCUFILE__=\"`basename $<`\"" $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

$(PATH-OBJECTS)/cli.o: $(PATH-SRC)/cli.c
	$(CC) "-D__WSCUFILE__=\"`basename $<`\"" $(CFLAGS) $(WFLAGS) -c -fPIC $< -o $@

wscu: \
	$(PATH-OBJECTS)/main.o \
	$(PATH-OBJECTS)/dl.o \
	$(PATH-OBJECTS)/config.o \
	$(PATH-OBJECTS)/dir.o \
	$(PATH-OBJECTS)/cli.o
	$(CC) $(CFLAGS) $(WFLAGS) -o $(PATH-BIN)/$@ $+ $(LDFLAGS)
