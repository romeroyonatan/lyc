/* 
* Compilador Grupo 6
* Lenguajes y compiladores
* Universidad Nacional de la Matanza
* 2015
*/

/* DECLARACIONES*/
/* ------------------------------------------------------------------------- */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <float.h>
#define ERROR -1
//////////////////////////////

#define QFIN 33
/* Terminales */
#define T_mas               0
#define T_menos             1
#define T_asterisco         2
#define T_barra             3
#define T_letra             4
#define T_digito            5
#define T_igual             6
#define T_menor             7
#define T_mayor             8
#define T_pregunta          9
#define T_dospuntos         10
#define T_exclamacion       11
#define T_comillas          12
#define T_punto             13
#define T_pyc               14
#define T_CAR               15
#define T_parentesis_abre   16
#define T_parentesis_cierra 17
#define T_coma              18
#define T_tab               19
#define T_espacio           20
#define T_newline           21
#define T_EOF               22
#define T_corchete_abre     23
#define T_corchete_cierra   24
/****************************/

#define CMP_MENOR 0
#define CMP_MENOR_IGUAL 1
#define CMP_MAYOR 2
#define CMP_MAYOR_IGUAL 3
#define CMP_DISTENTEROO 4
#define CMP_IGUAL 5

#define VAR_ENTERO 1
#define VAR_REAL 2
#define VAR_STRING 3

#define CANT_ESTADOS 39 //filas de la matriz de estados
#define CANT_TERMINALES 25 //columnas de la matriz de estados
#define CANTPR 20 //cantidad de palabras reservadas
#define LARGOMAX 15//largo maximo de las palabras reservadas
#define MAX_LONG 30 //largo maximo de los string y nombre de id
#define MAX_ENTERO 65535 //largo maximo de los enteros de 16 bit
#define MAX_REAL FLT_MAX  //largo maximo de los reales de 32 bit
#define TAMMAX 100
#define MAX_TERCETOS 1024 // cantidad maxima de tercetos

//Funciones de la matriz
void limpiar_token();
void inic_com();
void cont_com();
void fin_com();
void op_suma();
void op_menos();
void op_menos2();
void op_mul();
void op_div();
void op_asig();
void op_asig();
void op_menor();
void op_menor_ig();
void op_mayor();
void op_mayor_ig();
void dos_puntos();
void op_negar();
void puntoycoma();
void par_abre();
void par_cierra();
void llave_abre();
void llave_cierra();
void coma();
void inic_id();
void cont_id();
void fin_id();
void inic_real();
void cont_real();
void fin_real();
void inic_cte();
void cont_cte();
void fin_cte();
void inic_string();
void cont_string();
void fin_string();

void salto_linea();
void op_concaten();
void op_igualdad();
void op_distinto();
void error();
void nada();

//////////////////////////
void init();
int yylex();
int yyerror(char*);
char proximo_caracter();
void get_elementos_esperados(char*);
int get_evento(char);
int esPalabraRes();
void a_minuscula (char*);
int insertarTS();
void guardarTS();

int nuevo_estado[CANT_ESTADOS][CANT_TERMINALES];
void (*proceso[CANT_ESTADOS][CANT_TERMINALES])();

const char salto[7][4] = {
    {"BGE"}, // menor
    {"BG"}, // menor igual
    {"BLE"}, //mayor
    {"BL"}, //mayor igual
    {"BLE"}, // mayor
    {"EQ"}, // distinto
    {"NEQ"}, //igual
};

int linea = 1; //linea por la que esta leyendo
/* flag error de sintaxis */
int sintaxis_error;

/* Tabla de simbolos */
struct tablaDeSimbolos
{
    char nombre[100];
    int tipo;
    char valor [100];
    int longitud;
};

struct tablaDeSimbolos TS[TAMMAX];
/* obtiene nombre o valor del elemento en posicion i en la tabla de simbolos */
void obtener_nombre_o_valor(int, char*);

/* Notacion intermedia */
/* estrutura de un terceto */
typedef struct s_terceto {
    char t1[MAX_LONG], // primer termino
         t2[MAX_LONG], // segundo termino
         t3[MAX_LONG]; // tercer termino
    char aux[MAX_LONG]; // nombre variable auxiliar correspondiente
} t_terceto;
/* coleccion de tercetos */
t_terceto* tercetos[MAX_TERCETOS];
/* cantidad de tercetos */
int cant_tercetos;
/** crea una estructura de datos de terceto */
t_terceto* _crear_terceto (const char*, const char*, const char*);
/* crea un terceto y lo agrega a la coleccion */
int crear_terceto(const char*, const char*, const char*);
/* escribe los tercetos en un archivo */
void escribir_tercetos(FILE *);
/* libera memoria pedida para tercetos */
void limpiar_tercetos();
/* Indica que operador de comparacion se uso */
int iCmp;

/* Pila */ 
typedef struct s_nodo {
    int valor;
    struct s_nodo *sig;
} t_nodo;
/* apunta al ultimo elemento ingresado */
t_nodo *pila;
/** inserta un entero en la pila */
void insertar_pila (int);
/** obtiene un entero de la pila */
int sacar_pila(t_nodo*);
/** crea una estructura de pila */
void crear_pila(t_nodo*);
/** destruye pila */
void destruir_pila(t_nodo*);
/** 
* Comprueba que el elemento de la tabla de simbolos pertenece a alguno de los
* tipos compatibles. Devuelve cero en caso que no cumpla con el tipo requerido
* 
* comprobar_tipos(POSICION_TABLA_SIMBOLOS, CANTIDAD_DE_TIPOS_ADMITIDOS, TIPO1, 
*                 TIPO2, TIPON);
* Ejemplo de uso:
*   comprobar_tipos(1, 2, ENTERO, REAL);
*/
int comprobar_tipos(int, int,...);

/** Verifica que una variable haya sido declarada antes de su uso */
int variable_declarada (int);

/* Devuelve nombre del tipo en base a su numero */
void string_tipo(char *destino, int tipo);
%}


/* TOKENS */
/* ------------------------------------------------------------------------- */
/* operadores */
%token OP_ASIG
%token OP_IGUAL OP_MENOR OP_MAYOR OP_MAYOR_IGUAL OP_MENOR_IGUAL OP_DISTENTEROO
%token OP_CONCATENAR 
/* palabras reservadas */
%token WHILE IF CONST DECLARE ENDDECLARE REAL ENTERO STRING MAIN ELSE PUT GET CASE ESAC OF LET DEFAULT
%token AND OR NEGAR
/* operandos */
%token ID CTE_ENTERO CTE_STRING CTE_REAL

/* REGLAS SEMANTICAS */
/* ------------------------------------------------------------------------- */
%%
programa: declaraciones lista_sentencias
        | lista_sentencias 
        ;

declaraciones: DECLARE lista_declaraciones ENDDECLARE { $$ = $2; }
             ;
lista_declaraciones : declaracion
                    | lista_declaraciones ',' declaracion { $$ = $3; }
                    ;
declaracion : ID ':' tipo {
                  char id[MAX_LONG];
                  // obtengo tipo
                  int tipo = $3 + 255;
                  // obtengo nombre
                  obtener_nombre_o_valor($1, id);
                  // modifico tipo en tabla de simbolos
                  TS[$1].tipo = tipo;
                  // creo terceto
                  switch(tipo) {
                      case STRING:
                        $$ = crear_terceto("STRING", id, NULL);
                        break;
                      case ENTERO:
                        $$ = crear_terceto("ENTERO", id, NULL);
                        break;
                      case REAL:
                        $$ = crear_terceto("REAL", id, NULL);
                        break;
                  }
                  // creo terceto
              }
            ;
lista_sentencias: sentencia 
                | lista_sentencias sentencia {
                    $$ = $2;
                  }
                ;
sentencia: seleccion
         | WHILE condicion_logica {
               // creo un terceto temporal donde colocare el salto
               insertar_pila (crear_terceto("Temporal",NULL,NULL));
           } '{' lista_sentencias '}'{
               // creo el salto incondicional a la comparacion 
               int comparacion = sacar_pila (pila);
               int fin_while;
               char cmp[7], destino[7];
               sprintf(cmp, "[%d]", $2);
               // fin del while, creo salto incondicional
               fin_while = crear_terceto("BI", cmp, NULL);
               sprintf(destino, "[%d]", fin_while + 1);
               /*
               * creo terceto para saltar a la siguiente sentencia despues del
               * while
               */
               tercetos[comparacion] = _crear_terceto(salto[iCmp], cmp, destino); 
               $$ = $4;
           } 
         | PUT ID {
            //XXX ¿solo puedo escribir strings?
            if (variable_declarada($2)) {
                char valor[MAX_LONG];
                obtener_nombre_o_valor($2, valor);
                $$ = crear_terceto ("PUT", valor, NULL);
            }
           }
         | PUT CTE_STRING{
               char valor[MAX_LONG];
               sprintf(valor, "\"%s\"", TS[$2].valor);
               $$ = crear_terceto ("PUT", valor, NULL);
           }
         | GET ID {
            //XXX ¿solo puedo leer strings?
            if (variable_declarada($2)) {
                char valor[MAX_LONG];
                obtener_nombre_o_valor($2, valor);
                $$ = crear_terceto ("GET", valor, NULL);
            }
           }
         | asignacion
		 | CONST tipo ID OP_ASIG cte {
            int tipo = $2 + 255;
            switch(tipo) {
                case STRING:
                  comprobar_tipos($5, 1, CTE_STRING);
                  break;
                case REAL:
                  comprobar_tipos($5, 2, CTE_REAL, CTE_ENTERO);
                  break;
                case ENTERO:
                  comprobar_tipos($5, 2, CTE_REAL, CTE_ENTERO);
                  break;
            }
            TS[$3].tipo = TS[$5].tipo; 
            strcpy (TS[$3].valor, TS[$5].valor); 
            if ($2 == STRING)
                TS[$3].longitud = strlen(TS[$3].valor);
           }
         | CASE expresion {insertar_pila($2);} OF lista_case ESAC {sacar_pila (pila);}
		 | LET lista_let DEFAULT expresion {
			 char e[7];
			 sprintf(e, "[%d]", $4);
			 int i, posicion_ts;
			 for (i=0; i<contador; i++) {
				 posicion_ts=sacar_pila (pila);
				 $$ = crear_terceto("=", TS[posicion_ts].nombre, e); 
			 }
		 }
		 ; 
         
lista_let: lista_let ',' asignacion_let;
lista_let: asignacion_let;
asignacion_let: ID ':' expresion {
					if (variable_declarada($1) &&
						comprobar_tipos ($1, 2, ENTERO, REAL)) {
						char e[7];
						sprintf(e, "[%d]", $3);
						$$ = crear_terceto("=", TS[$1].nombre, e); 
					}
				}
				;
asignacion_let: ID { 
					if (variable_declarada($1) &&
						comprobar_tipos ($1, 2, ENTERO, REAL)) {
						insertar_pila ($1);
						contador++;
					}
				}
				;
		 
lista_case: lista_case case;
lista_case: case;
case : ID 
	{
		auxCase = sacar_pila (pila);
		char id[MAX_LONG];
		char terc_ant[MAX_LONG];
		char aux[MAX_LONG];
        	obtener_nombre_o_valor ($1, id);
		sprintf(aux, "[%d]", auxCase);
		sprintf(terc_ant, "[%d]", crear_terceto(id,NULL,NULL));
		crear_terceto("CMP",terc_ant,aux);
		insertar_pila (crear_terceto("Temporal",NULL,NULL));
		insertar_pila(auxCase);
	}
	OP_ASIG OP_MAYOR lista_sentencias ';'
	{
		auxCase = sacar_pila (pila);
		char terc_act[MAX_LONG];
		char salto[MAX_LONG];
		sprintf(terc_act, "[%d]", cant_tercetos);
		int proximo=sacar_pila (pila);
		sprintf(salto, "[%d]", proximo-1);
		tercetos[proximo] = _crear_terceto("BNE",salto,terc_act);
		insertar_pila(auxCase);
	}
	;
	
seleccion: IF condicion_logica {
               // creo un terceto temporal donde colocare el salto
               insertar_pila (crear_terceto("Temporal",NULL,NULL));
           }
           '{' lista_sentencias '}' {
               // creo el salto al ultimo terceto del then
               int inicio_then = sacar_pila (pila);
               char condicion[7], destino[7];
               sprintf(condicion, "[%d]", $2);
               sprintf(destino, "[%d]", $5 + 1);
               tercetos[inicio_then] = _crear_terceto(salto[iCmp],
                                                      condicion,
                                                      destino);
               $$ = $4;
           }
         | seleccion ELSE {
               // creo un terceto temporal donde colocare el salto del then
               insertar_pila (crear_terceto("Temporal",NULL,NULL));
           }
           '{' lista_sentencias '}'{
               // creo el salto al ultimo terceto del else
               int fin_then = sacar_pila (pila);
               char  destino[7];
               sprintf(destino, "[%d]", $4 + 1);
               tercetos[fin_then] = _crear_terceto("BI", destino, NULL);
               $$ = $4;
           }
         ;

tipo: ENTERO {$$ = ENTERO - 255;}
    | REAL {$$ = REAL - 255;}
    | STRING {$$ = STRING - 255;}
    ;
cte : CTE_STRING 
    | CTE_ENTERO 
    | CTE_REAL
    ;

condicion_logica: condicion AND condicion {
                    char c1[7], c2[7];
                    sprintf(c1, "[%d]", $1);
                    sprintf(c2, "[%d]", $3);
                    $$ = crear_terceto("AND", c1, c2); 
                  }
                | condicion OR condicion {
                    char c1[7], c2[7];
                    sprintf(c1, "[%d]", $1);
                    sprintf(c2, "[%d]", $3);
                    $$ = crear_terceto("OR", c1, c2); 
                  }
                | NEGAR condicion  {
                    char c[7];
                    sprintf(c, "[%d]", $2);
                    $$ = crear_terceto("NOT", c, NULL); 
                  }
                | condicion
                ;

condicion: expresion OP_MENOR expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_MENOR;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         | expresion OP_MENOR_IGUAL expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_MENOR_IGUAL;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         | expresion OP_IGUAL expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_IGUAL;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         | expresion OP_DISTENTEROO expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_DISTENTEROO;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         | expresion OP_MAYOR expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_MAYOR;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         | expresion OP_MAYOR_IGUAL expresion {
            char e1[7], e2[7];
            sprintf(e1, "[%d]", $1);
            sprintf(e2, "[%d]", $3);
            /* aviso que operacion hay que hacer */
            iCmp = CMP_MAYOR_IGUAL;
            $$ = crear_terceto("CMP", e1, e2); 
           }
         ;

asignacion: ID OP_ASIG expresion {
                if (variable_declarada($1) &&
                    comprobar_tipos ($1, 2, ENTERO, REAL)) {
                    char e[7];
                    sprintf(e, "[%d]", $3);
                    $$ = crear_terceto("=", TS[$1].nombre, e); 
                }
            }
          | ID OP_ASIG concatenacion{
                if (variable_declarada($1) && comprobar_tipos ($1, 1, STRING)) {
                    char e[7];
                    sprintf(e, "[%d]", $3);
                    $$ = crear_terceto("=", TS[$1].nombre, e); 
                }
            }
          ;
		  
expresion: expresion '+' termino {
                char e[7], t[7];
                sprintf(e, "[%d]", $1);
                sprintf(t, "[%d]", $3);
                $$ = crear_terceto("+", e, t); 
           }
		 | expresion '-' termino {
                char e[7], t[7];
                sprintf(e, "[%d]", $1);
                sprintf(t, "[%d]", $3);
                $$ = crear_terceto("-", e, t); 
           }
         | termino
         ;

termino: termino '*' factor {
           char t[7], f[7];
           sprintf(t, "[%d]", $1);
           sprintf(f, "[%d]", $3);
           $$ = crear_terceto("*", t, f); 
         }
       | termino '/' factor { 
           char t[7], f[7];
           sprintf(t, "[%d]", $1);
           sprintf(f, "[%d]", $3);
           $$ = crear_terceto("/", t, f); 
         }
       | factor
       ;

factor: ID { 
          if (variable_declarada($1) &&
              comprobar_tipos ($1, 4, ENTERO, REAL, CTE_ENTERO, CTE_REAL)) {
            char id[MAX_LONG];
            obtener_nombre_o_valor ($1, id);
            $$ = crear_terceto(id, NULL, NULL); 
          }
        }
      | cte { 
            char cte[MAX_LONG];
            obtener_nombre_o_valor ($1, cte);
            $$ = crear_terceto(cte, NULL, NULL);
        }
      | '(' expresion ')'
      ;

concatenacion: ID OP_CONCATENAR ID {
                   if (variable_declarada($1) && variable_declarada($3) &&
                       comprobar_tipos ($1, 1, STRING) &&
                       comprobar_tipos ($3, 1, STRING))
                       $$ = crear_terceto("++", TS[$1].nombre, TS[$3].nombre);
               }
             | ID OP_CONCATENAR CTE_STRING {
                   char cte[MAX_LONG];
                   if (variable_declarada($1) &&
                       comprobar_tipos ($1, 1, STRING)) {
                    sprintf(cte, "\"%s\"", TS[$3].valor);
                    $$ = crear_terceto("++", TS[$1].nombre, cte);
                   }
               }
             | CTE_STRING OP_CONCATENAR ID {
                   char cte[MAX_LONG];
                   if (variable_declarada($3) && 
                       comprobar_tipos ($3, 1, STRING)) {
                    sprintf(cte, "\"%s\"", TS[$1].valor);
                    $$ = crear_terceto("++", cte, TS[$1].valor);
                   }
               }
             | CTE_STRING OP_CONCATENAR CTE_STRING {
                   char cte1[MAX_LONG];
                   char cte2[MAX_LONG];
                   sprintf(cte1, "\"%s\"", TS[$1].valor);
                   sprintf(cte2, "\"%s\"", TS[$3].valor);
                   $$ = crear_terceto("++", cte1, cte2);
               }
             ;
%%
/* FUNCIONES AUXILIARES */    
/* ------------------------------------------------------------------------- */

/*----------VARIABLES GLOBALES-----------------*/
int yylval;
FILE *entrada, *tos;
int TStop = 0;  // Índice de la TS
int tipo_token; //numero identificador del token
int estado = 0; // estado actual
int longitud; //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo
int auxCase;
int contador = 0;
const char palabrasRes[CANTPR][LARGOMAX]={
    {"while"},
    {"if"},
    {"const"},
    {"declare"},
    {"enddeclare"},
    {"real"},
    {"int"},
    {"string"},
    {"main"},
    {"else"},
    {"and"},
    {"or"},
    {"put"},
    {"get"},
    {"case"},
    {"esac"},
    {"of"},
	{"let"},
    {"default"}
};


const char *terminal[CANT_TERMINALES];

int NroPalabrasRes[CANTPR]={
    WHILE,
    IF,
    CONST,
    DECLARE,
    ENDDECLARE,
    REAL,
    ENTERO,
    STRING,
    MAIN,
    ELSE,
    AND,
    OR,
    PUT,
    GET,
    CASE,
    ESAC,
    OF,
	LET,
	DEFAULT
};

int main(int argc, char **argv)
{
    FILE *intermedia;
    // inicializo matriz de estados de automata finito
    init();

    //Apertura del archivo con el lote de pruebas
    entrada = argc == 2 ? fopen(argv[1], "r") : fopen("prueba.txt", "r"); 
    if(entrada == NULL){
        printf("No se puede abrir el archivo %s\n", argc == 2 ? argv[1] : 
                                                    "prueba.txt");
        exit(ERROR);
    }

    if((intermedia = fopen("Intermedia.txt", "w"))==NULL){
        printf("No se puede crear el archivo Intermedia.txt\n");
        exit(ERROR);
    }

    //XXX: Para el analizador sintactico ya no la necesitamos mostrar
    if((tos = fopen("tabla_de_simbolos.txt", "w"))==NULL){
        printf("No se puede crear el archivo tabla_de_simbolos.txt\n");
        exit(ERROR);
    }

   	yyparse();

    guardarTS();
    // guardo coleccion de tercetos en archivo
    if (!sintaxis_error) {
        escribir_tercetos(intermedia);
        escribir_tercetos(stdout);
    }
    // libero memoria de tercetos
    limpiar_tercetos();

    // cerramos archivos abiertos
    fclose(entrada);
    fclose(intermedia);
    fclose(tos);
    return 0;
}

int yylex()
{
    estado=0;
    while(estado != QFIN)
    {
        if ((caracter = proximo_caracter()) != EOF)
        {
            int columna = get_evento(caracter);
            (proceso [estado] [columna]) ();

            estado = nuevo_estado [estado] [columna];
        }
        else
        {
			if(estado==0)return EOF;
            (proceso [estado] [22]) ();
            estado=QFIN;
        }
    }

    if(!feof(entrada))
        fseek(entrada,-sizeof(char),SEEK_CUR);

    return tipo_token;
}

char proximo_caracter()
{
    char _caracter;
    // obtengo caracter desde el archivo de entrada
    _caracter = fgetc(entrada);
    // devuelvo caracter leido
    return _caracter;
}

/* inicializa matriz de automata finito del analizador lexico */
void init () {

    int i,j;
    /* lleno la matriz de proximo estado */
    for (i=0; i<CANT_ESTADOS; i++)
        for (j=0; j<CANT_TERMINALES; j++)
            nuevo_estado[i][j] = QFIN;

    /* lleno la matriz de proceso */
    for (i=0; i<CANT_ESTADOS; i++)
        for (j=0; j<CANT_TERMINALES; j++)
            proceso[i][j] = nada;


    nuevo_estado[0][T_mas] = 3;
    nuevo_estado[0][T_menos] = 15;
    nuevo_estado[0][T_asterisco] = 16;
    nuevo_estado[0][T_barra] = 17;
    nuevo_estado[0][T_letra] = 1;
    nuevo_estado[0][T_digito] = 24;
    nuevo_estado[0][T_igual] = 5;
    nuevo_estado[0][T_menor] = 9;
    nuevo_estado[0][T_mayor] = 11;
    nuevo_estado[0][T_pregunta] = 27;
    nuevo_estado[0][T_dospuntos] = 26;
    nuevo_estado[0][T_exclamacion] = 21;
    nuevo_estado[0][T_comillas] = 23;
    nuevo_estado[0][T_punto] = 25;
    nuevo_estado[0][T_pyc] = 2;
    nuevo_estado[0][T_parentesis_abre] = 18;
    nuevo_estado[0][T_parentesis_cierra] = 19;
    nuevo_estado[0][T_coma] = 20;
    nuevo_estado[0][T_corchete_abre] = 7;
    nuevo_estado[0][T_corchete_cierra] = 8;
    nuevo_estado[0][T_CAR] = 0;
    nuevo_estado[0][T_tab] = 0;
    nuevo_estado[0][T_espacio] = 0;
    nuevo_estado[0][T_newline] = 0;

    nuevo_estado[1][T_letra] = 1;
    nuevo_estado[1][T_digito] = 1;
    nuevo_estado[3][T_mas] = 4;
    nuevo_estado[5][T_igual] = 6;
    nuevo_estado[9][T_igual] = 10;
    nuevo_estado[11][T_igual] = 12;
    nuevo_estado[15][T_menos] = 29;
    nuevo_estado[21][T_igual] = 22;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[23][i] = 23;
    nuevo_estado[23][T_comillas] = 28;
    nuevo_estado[23][T_EOF] = QFIN;

    nuevo_estado[24][T_digito] = 24;
    nuevo_estado[24][T_punto] = 25;
    nuevo_estado[25][T_digito] = 25;
    nuevo_estado[29][T_barra] = 30;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[30][i] = 30;
    nuevo_estado[30][T_menos] = 34;
    nuevo_estado[30][T_barra] = 31;
    nuevo_estado[30][T_EOF] = QFIN;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[31][i] = 30;
    nuevo_estado[31][T_menos] = 32;
    nuevo_estado[31][T_barra] = 31;
    nuevo_estado[31][T_EOF] = QFIN;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[32][i] = 30;
    nuevo_estado[32][T_menos] = 0;
    nuevo_estado[32][T_barra] = 31;
    nuevo_estado[31][T_EOF] = QFIN;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[34][i] = 30;
    nuevo_estado[34][T_menos] = 35;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[35][i] = 30;
    nuevo_estado[35][T_menos] = 35;
    nuevo_estado[35][T_barra] = 36;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[36][i] = 36;
    nuevo_estado[36][T_barra] = 37;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[37][i] = 36;
    nuevo_estado[37][T_menos] = 38;
    nuevo_estado[37][T_barra] = 37;

    for(i = 0; i < CANT_TERMINALES; i++)
        nuevo_estado[38][i] = 36;
    nuevo_estado[38][T_barra] = 37;
    nuevo_estado[38][T_menos] = 30;

    proceso[0][T_mas] = op_suma;
    proceso[0][T_menos] = op_menos;
    proceso[0][T_asterisco] = op_mul;
    proceso[0][T_barra] = op_div;
    proceso[0][T_letra] = inic_id;
    proceso[0][T_digito] = inic_cte;
    proceso[0][T_igual] = op_asig;
    proceso[0][T_menor] = op_menor;
    proceso[0][T_mayor] = op_mayor;
    proceso[0][T_dospuntos] = dos_puntos;
    proceso[0][T_exclamacion] = op_negar;
    proceso[0][T_comillas] = inic_string;
    proceso[0][T_punto] = inic_real;
    proceso[0][T_pyc] = puntoycoma;
    proceso[0][T_parentesis_abre] = par_abre;
    proceso[0][T_parentesis_cierra] = par_cierra;
    proceso[0][T_coma] = coma;
    proceso[0][T_newline] = salto_linea;
    proceso[0][T_CAR] = error;
    proceso[0][T_corchete_abre] = llave_abre;
    proceso[0][T_corchete_cierra] = llave_cierra;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[1][i] = fin_id;
    proceso[1][T_letra] = cont_id;
    proceso[1][T_digito] = cont_id;

    proceso[3][T_mas] = op_concaten;
    proceso[5][T_igual] = op_igualdad;
    proceso[9][T_igual] = op_menor_ig;
    proceso[11][T_igual] = op_mayor_ig;
    proceso[21][T_igual] = op_distinto;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[23][i] = cont_string;
    proceso[23][T_comillas] = fin_string;
    proceso[23][T_EOF] = error;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[24][i] = fin_cte;
    proceso[24][T_digito] = cont_cte;
    proceso[24][T_punto] = cont_real;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[25][i] = fin_real;
    proceso[25][T_digito] = cont_real;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[29][i] = op_menos2;
    proceso[29][T_barra] = inic_com;
    proceso[29][T_EOF] = nada;
    proceso[29][T_newline] = nada;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[30][i] = cont_com;
    proceso[30][T_EOF] = error;
    proceso[30][T_newline] = salto_linea;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[31][i] = cont_com;
    proceso[31][T_EOF] = error;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[32][i] = cont_com;
    proceso[32][T_menos] = fin_com;
    proceso[32][T_EOF] = error;

    proceso[36][T_newline] = salto_linea;

    terminal[T_mas] =               "+";
    terminal[T_menos] =             "-";
    terminal[T_asterisco] =         "*";
    terminal[T_barra] =             "/";
    terminal[T_letra] =             "a-z A-Z _";
    terminal[T_digito] =            "0-9";
    terminal[T_igual] =             "=";
    terminal[T_menor] =             "<";
    terminal[T_mayor] =             ">";
    terminal[T_pregunta] =          "?";
    terminal[T_dospuntos] =         ":";
    terminal[T_exclamacion] =       "!";
    terminal[T_comillas] =          "\"";
    terminal[T_punto] =             ".";
    terminal[T_pyc] =               ";";
    terminal[T_CAR] =               "caracter";
    terminal[T_parentesis_abre] =   "(";
    terminal[T_parentesis_cierra] = ")";
    terminal[T_coma] =              ",";
    terminal[T_tab] =               "tab";
    terminal[T_espacio] =           "espacio";
    terminal[T_newline] =           "nueva linea";
    terminal[T_EOF] =               "EOF";
    terminal[T_corchete_abre] =     "[";
    terminal[T_corchete_cierra] =   "]";
    
    cant_tercetos = 0;
    crear_pila(pila);
}

void limpiar_token()
{
  *token = '\0';
}
void inic_com()
{
    limpiar_token();
}
void cont_com()
{
}
void fin_com()
{
    tipo_token =  0;
}

void op_suma()
{
    tipo_token =  '+';
}
void op_menos()
{
	tipo_token =  '-';
}
void op_menos2()
{
    fseek(entrada,-sizeof(char),SEEK_CUR);
    tipo_token =  '-';
}
void op_mul()
{
	tipo_token =  '*';
}
void op_div()
{
	tipo_token =  '/';
}
void op_asig()
{
	tipo_token =  OP_ASIG;
}

void op_menor()
{
    tipo_token =   OP_MENOR;
}
void op_menor_ig()
{
    tipo_token =   OP_MENOR_IGUAL;
}
void op_mayor()
{
    tipo_token =   OP_MAYOR;
}
void op_mayor_ig()
{
    tipo_token =   OP_MAYOR_IGUAL;
}
void dos_puntos()
{
    tipo_token =   ':';
}
void op_negar()
{
	tipo_token =   NEGAR;
}
void puntoycoma()
{
    tipo_token =   ';';
}
void par_abre()
{
    tipo_token =   '(';
}
void par_cierra()
{
    tipo_token =   ')';
}
void llave_abre()
{
    tipo_token =   '{';
}
void llave_cierra()
{
    tipo_token =   '}';
}
void coma()
{
    tipo_token =   ',';
}
void inic_id()
{
    limpiar_token();
    strcat(token,&caracter);
    tipo_token =   ID;
}
void cont_id()
{
    strcat(token,&caracter);
    tipo_token =   ID;
}
void fin_id()
{
    int i;
    if (strlen(token) > MAX_LONG)
    {
        fprintf(stderr,"identificador demasiado largo en linea: %d\n",linea);
        *token='\0';
        tipo_token =  0;
    } else {
        if( (i = esPalabraRes()) != -1)
        {
            tipo_token =  NroPalabrasRes[i];
        }
        else
        {
            tipo_token = ID;
            yylval = insertarTS();
        }
    }
}
void inic_cte()
{
    limpiar_token();
    strcat(token,&caracter);
    tipo_token =   CTE_ENTERO;
}
void cont_cte()
{
    strcat(token,&caracter);
    tipo_token =   CTE_ENTERO;
}
void fin_cte()
{
    int cte = atoi(token);
    if(cte > MAX_ENTERO) {
        fprintf(stderr, "Entero sobrepasa limite maximo en linea: %d\n",linea);
		*token='\0';
        tipo_token =  0;
    } else {
        tipo_token = CTE_ENTERO;
        yylval = insertarTS();
    }

}
void inic_real()
{
    limpiar_token();
    strcat(token,&caracter);
    tipo_token =   CTE_REAL;
}
void cont_real()
{
    strcat(token,&caracter);
    tipo_token =   CTE_REAL;
}
void fin_real()
{
    float cte = atof(token);
    if(cte > MAX_REAL)
    {
        fprintf(stderr, "Real sobrepasa limite maximo en linea: %d\n",linea);
		*token='\0';
        tipo_token =  0;
    } else {
        tipo_token = CTE_REAL;
        yylval = insertarTS();
    }

}
void inic_string()
{
    limpiar_token();
    tipo_token =   CTE_STRING;
}
void cont_string()
{
    strcat(token,&caracter);
    tipo_token =   CTE_STRING;
}

void fin_string()
{
    if (strlen(token) > MAX_LONG)
    {
        fprintf(stderr, "String demasiado largo en linea: %d\n", linea);
		*token='\0';
        tipo_token = 0;
    } else {
        tipo_token = CTE_STRING;
        yylval = insertarTS();
    }

}
void salto_linea()
{
    tipo_token =  0;
    linea++;
}
void op_concaten()
{
    limpiar_token();
    tipo_token =   OP_CONCATENAR;
}
void op_igualdad()
{
    limpiar_token();
    tipo_token =   OP_IGUAL;
}
void op_distinto()
{
    limpiar_token();
    tipo_token =  OP_DISTENTEROO;
}
void nada()
{
}


void error()
{
    char _elementos_esperados[300] = {'\0'};
    /* como es un error, descarto el contenido de token */
    *token = '\0';

    // en caso de un fin de archivo inesperado muestro mensaje y salgo
    if (caracter == EOF) {
        fprintf (stderr, "Error linea %d: Fin de archivo inesperado \n", linea);
		tipo_token =  ERROR;
		return;
    }
    // obtengo elementos esperados
    get_elementos_esperados (_elementos_esperados);
    // muestro mensaje de error
    fprintf (stderr, "Error linea %d, cerca del elemento inesperado: '%c'\n", linea, caracter);
    // muestro elementos esperados
    fprintf (stderr, ">>>> Elementos esperados: %s\n", _elementos_esperados);
    tipo_token =  ERROR;
}

void get_elementos_esperados(char *esperados)
{
    int i;
    for (i = 0; i < CANT_TERMINALES; i++)
        /* busco los terminales que no me deriven en un error, obviando EOF
         * y espacios en blanco */
        if (i != T_EOF && i!= T_tab && i!= T_espacio && i != T_newline &&
            proceso [estado][i] != error) {
            /* agrego elemento a la lista de terminales esperados */
            strcat (esperados, terminal[i]);
            strcat (esperados, " ");
        }
}

int get_evento(char c)
{
    if(c>='a' && c<='z')
        return 4;
    if(c>='A' && c<='Z')
        return 4;
    if(c>='0' && c<='9')
        return 5;

    switch(c)
    {

        case '+':
            return 0;
        case '-':
            return 1;

        case '*':
            return 2;

        case '/':
            return 3;

        case '=':
            return 6;

        case '<':
            return 7;

        case '>':
            return 8;

        case '?':
            return 9;

        case ':':
            return 10;

        case '!':
            return 11;

        case '"':
            return 12;

        case '.':
            return 13;

        case ';':
            return 14;

        case '(':
            return 16;

        case ')':
            return 17;

        case ',':
            return 18;

        //case '\t':
        //    return 19;

        case '\t':
        case '\r':
        case ' ':
            return 20;

        case '\n':
            return 21;

        case EOF:
            return 22;
        case '{':
            return 23;

        case '}':
            return 24;

        case '_':
            return 4;

        default:
            return 15;
    }
}

int esPalabraRes()
{
    char aux[MAX_LONG];
    int i;
    strcpy(aux, token);
    // pasamos todo el token a minuscula
    a_minuscula(aux);
    for(i=0;i<CANTPR;i++)
    {
        if(strcmp(palabrasRes[i],aux)==0)
        {
            return i;
        }
    }

    return -1;
}

void a_minuscula (char *palabra)
{
    char *tmp = palabra;
    while (*tmp)
    {
        *tmp = tolower(*tmp);
        tmp++;
    }
}

void guardarTS()
{
    int i;
    char tipo[MAX_LONG];
    fprintf(tos,"%-4s|%-20s|%-20s|%-25s|%-4s\n","Nro","Nombre","Tipo","Valor",
                                                "Longitud");
    for (i=0; i<TStop; i++){
        // obtengo string del tipo
        string_tipo(tipo, TS[i].tipo);
        fprintf(tos,"%-4d|%-20s|%-20s|%-25s|%-4d\n",
                i,TS[i].nombre, tipo, TS[i].valor, TS[i].longitud);
    }
}

/* Inserta en la TS o si ya existe devuelve la posicion */
int insertarTS()
{
	int i=0;

    // Reviso si ya existe en la TS
    for (i=0; i<TStop;  i++) {

        if (tipo_token==ID) {
            if (strcmp(TS[i].nombre,token)==0)
                return i;
        }
        else {
            if (strcmp(TS[i].valor,token)==0)
                return i;
        }
    }

    // Inserto en la TS
  	switch (tipo_token) {

        case ID:
            strcpy(TS[TStop].nombre,token);
            TS[TStop].tipo = ID;
            TStop++;
        break;
        case CTE_ENTERO:
            strcpy(TS[TStop].nombre, "");
            TS[TStop].tipo = CTE_ENTERO;
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
        case CTE_REAL:
            strcpy(TS[TStop].nombre,"");
            TS[TStop].tipo = CTE_REAL;
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
       	case CTE_STRING:
            strcpy(TS[TStop].nombre, "");
            TS[TStop].tipo = CTE_STRING;
            strcpy(TS[TStop].valor, token);
            TS[TStop].longitud = (strlen(token));
            TStop++;
        break;
    }

    return TStop-1;
}

int yyerror(char *s)
{
    fprintf(stderr,"Linea %d: %s.\n",linea, s);
    sintaxis_error = 1;
    return 1;
}

/** crea una estructura de datos de terceto */
t_terceto* _crear_terceto (const char* t1, const char* t2, const char* t3){
    t_terceto* terceto = (t_terceto*) malloc(sizeof(t_terceto));
    // completo sus atributos
    strcpy(terceto->t1, t1);

    if (t2)
        strcpy(terceto->t2, t2);
    else
        *(terceto->t2) = '\0';

    if (t3)
        strcpy(terceto->t3, t3);
    else
        *(terceto->t3) = '\0';
    return terceto; 
}

/** crea un terceto y lo agrega a la coleccion de tercetos */
int crear_terceto(const char* t1, const char* t2, const char* t3){
    // creo un nuevo terceto y lo agrego a la coleccion de tercetos
    int numero = cant_tercetos;
    tercetos[numero] = _crear_terceto (t1, t2, t3);
    cant_tercetos++;
    // devuelvo numero de terceto
    return numero;
}

/** Escribe tercetos en un archivo de texto */
void escribir_tercetos (FILE* archivo) {
    int i;
    for (i = 0; i < cant_tercetos; i++)
        fprintf(archivo, "%d (%s, %s, %s)\n", i,
                                              tercetos[i]->t1,
                                              tercetos[i]->t2,
                                              tercetos[i]->t3);
}
/** Libera memoria pedida para tercetos */ 
void limpiar_tercetos () {
    int i;
    for (i = 0; i < cant_tercetos; i++)
        free(tercetos[i]);
}

/** Obtiene nombre o valor del elemento en posicion i en la tabla de simbolos */
void obtener_nombre_o_valor(int posicion, char* destino) {
    if (*(TS[posicion].valor))
        strcpy(destino, TS[posicion].valor);
    else
        strcpy(destino, TS[posicion].nombre);
}

/** inserta un entero en la pila */
void insertar_pila (int valor) {
    // creo nodo
    t_nodo *nodo = (t_nodo*) malloc (sizeof(t_nodo));
    // asigno valor
    nodo->valor = valor;
    // apunto al elemento siguiente
    nodo->sig = pila;
    // apunto al tope de la pila
    pila = nodo;
}

/** obtiene un entero de la pila */
int sacar_pila(t_nodo* p) {
    int valor;
    t_nodo *aux;
    if (pila != NULL) {
       aux = pila;
       valor = aux->valor;
       pila = aux->sig;
       free(aux);
    } else
       valor = ERROR;
    return valor;
}

/** crea una estructura de pila */
void crear_pila(t_nodo* pila) {
    pila = NULL;
}

/** destruye pila */
void destruir_pila(t_nodo* pila) {
    while ( ERROR != sacar_pila(pila));
}

/** 
* Comprueba que el elemento de la tabla de simbolos pertenece a alguno de los
* tipos compatibles. Devuelve cero en caso que no cumpla con el tipo requerido
* 
* comprobar_tipos(POSICION_TABLA_SIMBOLOS, CANTIDAD_DE_TIPOS_ADMITIDOS, TIPO1, 
*                 TIPO2, TIPON);
*
* Ejemplo de uso:
*   comprobar_tipos($1, 2 , ENTERO, REAL);
*/
int comprobar_tipos(int posicion, int cantidad_tipos, ...) {
    int i;
    /* defino tipos esperados */
    int esperados[cantidad_tipos];
    // valores de los parametros
    int val; 
    // lista de parametros
    va_list vl;
    // empiezo a leer parametros variables
    va_start(vl, cantidad_tipos);
    // ************** aca leo parametros y comparo
    for (i = 0; i < cantidad_tipos; i++) {
        // obtengo proximo parametro de tipo entero
        val = va_arg (vl, int);
        esperados[i] = val;
        if (TS[posicion].tipo == esperados[i]){
            /* termino de leer parametros variables */
            va_end(vl);
            /* devuelvo 1 porque encontre el tipo en la lista*/
            return 1;
        }
   }
   /* si llegue aca no encontre el tipo en la lista */
   /* --------------------------------------------- */
   char espera [255];
   char aux [255];
   char error [255];
   *espera = '\0';
   for (i = 0; i < cantidad_tipos; i++) {
       string_tipo(aux, esperados[i]);
       if (i > 0) 
           /* agrego coma */
           strcat (espera, " o ");
       strcat (espera, aux);
   }
   /* Muestro mensaje de error */
   sprintf(error, "Error. Tipo incompatible en variable %s - Tipo esperado %s", 
                  TS[posicion].nombre,
                  espera);
   yyerror(error);
   /* termino de leer parametros variables */
   va_end(vl);
   return 0;
}

/** Verifica que una variable haya sido declarada antes de su uso */
int variable_declarada (int posicion) {
    char error[255];
    if (TS[posicion].tipo != ID)
        return 1;
    else {
        sprintf(error, "Intentando usar variable %s no declarada", 
                       TS[posicion].nombre);
        yyerror(error);
        return 0;
    }
}

/* Devuelve nombre del tipo en base a su numero */
void string_tipo(char *destino, int tipo) {

    // armo el string del tipo
   switch (tipo) {
       case STRING:
           strcpy (destino, "variable string");
           break;
       case CTE_STRING:
           strcpy (destino, "constante string");
           break;
       case ENTERO:
           strcpy (destino, "variable entera");
           break;
       case CTE_ENTERO:
           strcpy (destino, "constante entera");
           break;
       case REAL:
           strcpy (destino, "variable real");
           break;
       case CTE_REAL:
           strcpy (destino, "constante real");
           break;
       default:
           strcpy (destino, "");
   }
}
