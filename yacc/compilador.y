/* 
* Compilador Grupo 6
* Lenguajes y compiladores
* Universidad Nacional de la Matanza
* 2015
*/

/* DECLARACIONES*/
/* ------------------------------------------------------------------------- */
%{
%}

/* TOKENS */
/* ------------------------------------------------------------------------- */
/* operadores */
%token OP_ASIG
%token OP_IGUAL OP_MENOR OP_MAYOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_DISTINTO
%token OP_CONCATENAR 
/* palabras reservadas */
%token WHILE IF CONST DECLARE ENDDECLARE REAL INT STRING MAIN ELSE PUT GET
%token AND OR NEGAR
/* operandos */
%token ID CTE_ENTERO CTE_STRING CTE_REAL

/* REGLAS SEMANTICAS */
/* ------------------------------------------------------------------------- */
%%
programa: declaraciones lista_sentencias
        ;
declaraciones: DECLARE lista_declaraciones ENDDECLARE
             | 
             ;
lista_declaraciones : declaracion
                    | lista_declaraciones ',' declaracion
                    ;
declaracion : ID ':' REAL
            | ID ':' INT
            | ID ':' STRING
            ;
lista_sentencias: sentencia
                | lista_sentencias sentencia
                ;
sentencia: IF condicion_logica '{' lista_sentencias '}'
         | IF condicion_logica '{' lista_sentencias '}' 
           ELSE '{' lista_sentencias '}'
         | WHILE condicion_logica '{' lista_sentencias '}'
         | PUT ID
         | PUT CTE_STRING
         | GET ID
         | asignacion
         | CONST tipo ID OP_ASIG cte
         ;

tipo: INT 
    | REAL 
    | STRING
    ;
cte : CTE_STRING 
    | CTE_ENTERO 
    | CTE_REAL
    ;

condicion_logica: condicion AND condicion
                | condicion OR condicion
                | NEGAR condicion
                | condicion
                ;

condicion: factor OP_MENOR factor
         | factor OP_MENOR_IGUAL factor
         | factor OP_IGUAL factor
         | factor OP_DISTINTO factor
         | factor OP_MAYOR factor
         | factor OP_MAYOR_IGUAL factor
         ;

asignacion: ID OP_ASIG expresion
          | ID OP_ASIG concatenacion
          ;

expresion: expresion '+' termino
         | termino
         ;

termino: termino '*' factor
       | termino '/' factor
       | factor
       ;

factor: ID
      | cte
      | '(' expresion ')'
      ;

concatenacion: ID OP_CONCATENAR ID
             | ID OP_CONCATENAR CTE_STRING
             | CTE_STRING OP_CONCATENAR ID
             | CTE_STRING OP_CONCATENAR CTE_STRING
             ;
%%

/* FUNCIONES AUXILIARES */    
/* ------------------------------------------------------------------------- */

