all: build/Grupo06 build/Grupo06.exe
	
build/Grupo06: build/Grupo06.tab.c
	gcc -g -Wall -o build/Grupo06 build/Grupo06.tab.c

build/Grupo06.exe: build/Grupo06.tab.c
	i686-w64-mingw32-gcc -g -o build/Grupo06.exe build/Grupo06.tab.c

build/Grupo06.tab.c: yacc/Grupo06.y
	bison -o build/Grupo06.tab.c  yacc/Grupo06.y

build/lex: lex/lex.c
	gcc -g -Wall -o build/lex lex/lex.c
clean:
	rm build/Grupo06 build/Grupo06.exe build/Grupo06.tab.c
