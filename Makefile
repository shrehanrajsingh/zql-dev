all: compile run

compile:
	@gcc -o bin/main src/main.c -Iinclude -g -Lbin -lzql

run:
	@.\bin\main

debug:
	@gdb bin/main

build:
	@cd include && gcc -c -DZQL_BUILD_DLL -o ../bin/zql.o zql_utils.c
	@gcc -o ./bin/zql.dll -s -shared ./bin/zql.o -Wl,--subsystem,windows
	@rm -rf bin/zql.o
	@cp include/zql_utils.h bin/zql.h