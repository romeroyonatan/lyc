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
%token OP_SUMA OP_MENOS
%token OP_MUL OP_DIV
%token OP_IGUAL OP_MENOR OP_MAYOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_DISTINTO
%token OP_CONCATENAR NEGAR
/* puntuacion */
%token DOS_PUNTOS PUNTO_Y_COMA PARENT_ABRE PARENT_CIERRA COMA LLAVE_ABRE
%token LLAVE_CIERRA 
/* palabras reservadas */
%token OUTPUT INPUT WHILE IF CONST DECLARE ENDDECLARE REAL INT STRING
%token MAIN ELSE  PUT GET 
%nonassoc AND OR
/* operadores */
%token ID CTE_ENTERO CTE_STRING CTE_REAL

/* REGLAS SEMANTICAS */
/* ------------------------------------------------------------------------- */
%%
programa: declaraciones lista_sentencias
        ;
declaraciones: DECLARE lista_declaraciones ENDDECLARE
             ;
lista_declaraciones : declaracion
                    | lista_declaraciones COMA declaracion
                    ;
declaracion : ID DOS_PUNTOS REAL
            | ID DOS_PUNTOS INT
            | ID DOS_PUNTOS STRING
            ;
lista_sentencias: sentencia
                | lista_sentencias sentencia
                ;
sentencia: IF condicion LLAVE_ABRE lista_sentencias LLAVE_CIERRA
         | IF condicion LLAVE_ABRE lista_sentencias LLAVE_CIERRA 
           ELSE LLAVE_ABRE lista_sentencias LLAVE_CIERRA
         | WHILE condicion LLAVE_ABRE lista_sentencias LLAVE_CIERRA
         | PUT ID
         | PUT CTE_STRING
         | GET ID
         | asignacion
         ;

condicion: factor OP_MENOR factor
         | factor OP_MENOR_IGUAL factor
         | factor OP_IGUAL factor
         | factor OP_DISTINTO factor
         | factor OP_MAYOR factor
         | factor OP_MAYOR_IGUAL factor
         | condicion AND condicion
         | condicion OR condicion
         ;

asignacion: ID OP_ASIG expresion
          | ID OP_ASIG concatenacion
          ;

expresion: expresion OP_SUMA termino
         | expresion OP_MENOS termino
         | termino
         ;

termino: termino OP_MUL factor
       | termino OP_DIV factor
       | factor
       ;

factor: ID
      | CTE_STRING
      | CTE_ENTERO
      | CTE_REAL
      | PARENT_ABRE expresion PARENT_CIERRA
      ;

concatenacion: ID OP_CONCATENAR ID
             | ID OP_CONCATENAR CTE_STRING
             | CTE_STRING OP_CONCATENAR ID
             | CTE_STRING OP_CONCATENAR CTE_STRING
             ;
%%

/* FUNCIONES AUXILIARES */    
/* ------------------------------------------------------------------------- */

