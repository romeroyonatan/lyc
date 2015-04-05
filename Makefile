test/lex: bin/lex
	cp bin/lex test/lex
bin/lex: lex/lex.c
	gcc -o bin/lex lex/lex.c

