bin/compilador: yacc/compilador.y
	bison -o compilador.tab.c  yacc/compilador.y
	gcc -g -Wall -o bin/compilador compilador.tab.c
	rm compilador.tab.c
bin/lex: lex/lex.c
	gcc -g -Wall -o bin/lex lex/lex.c

