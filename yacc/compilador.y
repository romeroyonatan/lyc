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
%token MAIN ELSE AND OR PUT GET 
/* operadores */
%token ID CTE_ENTERO CTE_STRING CTE_REAL

/* REGLAS SEMANTICAS */
/* ------------------------------------------------------------------------- */
%%
programa: declaraciones lista_sentencias
declaraciones: BEGIN lista_declaraciones END
lista_declaraciones: declaracion
                   | lista_declaraciones COMA declaracion
%%

/* FUNCIONES AUXILIARES */    
/* ------------------------------------------------------------------------- */

