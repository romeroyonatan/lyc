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

#define CANT_ESTADOS 39 //filas de la matriz de estados
#define CANT_TERMINALES 25 //columnas de la matriz de estados
#define CANTPR 15 //cantidad de palabras reservadas
#define LARGOMAX 15//largo maximo de las palabras reservadas
#define MAX_LONG 30 //largo maximo de los string y nombre de id
#define MAX_INT 65535 //largo maximo de los enteros de 16 bit
#define MAX_REAL FLT_MAX  //largo maximo de los reales de 32 bit
#define TAMMAX 100
#define TERCETOS_MAX 100 // cantidad maxima de tercetos

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

/* Tabla de simbolos */
struct tablaDeSimbolos
{
    char nombre[100];
    char tipo  [11];
    char valor [100];
    char ren   [31];
    int longitud;
};
struct tablaDeSimbolos TS[TAMMAX];

/* Notacion intermedia */
char tercetos[TERCETOS_MAX][MAX_LONG];
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
programa: {puts("Inicio de programa");} declaraciones lista_sentencias{puts("Fin de programa");}
        | {puts("Inicio de programa");} lista_sentencias{puts("Fin de programa");};

declaraciones: DECLARE lista_declaraciones ENDDECLARE
             ;
lista_declaraciones : declaracion
                    | lista_declaraciones ',' declaracion
                    ;
declaracion : ID ':' tipo{printf("Declaracion de variable '%s'\n", TS[$1].nombre);}
            ;
lista_sentencias: sentencia
                | lista_sentencias sentencia
                ;
sentencia: seleccion{puts("Fin THEN");} 
         | seleccion{puts("Fin THEN");} 
           ELSE {puts("Inicio ELSE");}'{' lista_sentencias '}'{puts("Fin ELSE");};
         | WHILE condicion_logica {puts("Inicio WHILE");} '{' lista_sentencias '}'{puts("Fin WHILE");} 
         | PUT ID{printf("Mostrar por pantalla el valor de variable '%s'\n",
                         TS[$2].nombre);}
         | PUT CTE_STRING{printf("Mostrar por pantalla '%s'\n",TS[$2].valor);}
         | GET ID {printf("Leer del teclado y guardar en variable '%s'",
                          TS[$2].valor);}
         | asignacion
		 |CONST tipo ID OP_ASIG cte
            {printf("Declaracion de CTE %s con nombre '%s' valor '%s'\n",
                    TS[$3].tipo, TS[$3].nombre, TS[$5].valor);}
         ;

seleccion: IF condicion_logica '{'{puts("Inicio THEN");}  lista_sentencias '}';

tipo: INT 
    | REAL 
    | STRING
    ;
cte : CTE_STRING 
    | CTE_ENTERO 
    | CTE_REAL
    ;

condicion_logica: condicion AND{puts("AND");} condicion
                | condicion OR{puts("OR");} condicion
                | NEGAR{puts("NEGAR");} condicion
                | condicion
                ;

condicion: expresion OP_MENOR expresion{puts("Comparacion MENOR");}
         | expresion OP_MENOR_IGUAL expresion{puts("Comparacion MENOR_IGUAL");}
         | expresion OP_IGUAL expresion{puts("Comparacion IGUALDAD");}
         | expresion OP_DISTINTO expresion{puts("Comparacion DISTINTO");}
         | expresion OP_MAYOR expresion{puts("Comparacion MAYOR");}
         | expresion OP_MAYOR_IGUAL expresion{puts("Comparacion MAYOR_IGUAL");}
         ;

asignacion: ID OP_ASIG expresion{printf("Asignacion de %s\n", TS[$1].nombre);}
          | ID OP_ASIG concatenacion{printf("Asignacion de %s\n", TS[$1].nombre);}
          ;

expresion: expresion '+' termino{printf("Suma entre %s y %s\n",
                                  *(TS[$1].nombre) ? TS[$1].nombre : TS[$1].valor,
                                  *(TS[$3].nombre) ? TS[$3].nombre : TS[$3].valor
                                  );}
		 | expresion '-' termino{printf("Resta entre %s y %s\n",
                                  *(TS[$1].nombre) ? TS[$1].nombre : TS[$1].valor,
                                  *(TS[$3].nombre) ? TS[$3].nombre : TS[$3].valor
                                  );}
         | termino
         ;

termino: termino '*' factor{printf("Multiplicacion entre %s y %s\n", 
                                  *(TS[$1].nombre) ? TS[$1].nombre : TS[$1].valor,
                                  *(TS[$3].nombre) ? TS[$3].nombre : TS[$3].valor
                                  );}
       | termino '/' factor{printf("Division entre %s y %s\n",
                                *(TS[$1].nombre) ? TS[$1].nombre : TS[$1].valor,
                                *(TS[$3].nombre) ? TS[$3].nombre : TS[$3].valor
                            );}
       | factor
       ;

factor: ID 
      | cte 
      | '(' expresion ')'
      ;

concatenacion: ID OP_CONCATENAR ID{puts("Concatena ID con ID");}
             | ID OP_CONCATENAR CTE_STRING{puts("Concatena ID con CTE_STRING");}
             | CTE_STRING OP_CONCATENAR ID{puts("Concatena CTE_STRING con ID");}
             | CTE_STRING OP_CONCATENAR CTE_STRING{puts("Concatena CTE_STRING con CTE_STRING");}
             ;
%%
/* FUNCIONES AUXILIARES */    
/* ------------------------------------------------------------------------- */

/*----------VARIABLES GLOBALES-----------------*/
int yylval;
FILE *entrada, *tos;
int TStop = 0;  // Índice de la TS
int tipo_token; //numero identificador del token
int linea = 1; //linea por la que esta leyendo
int estado = 0; // estado actual
int longitud; //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo
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
    {"get"}
};

const char *terminal[CANT_TERMINALES];

int NroPalabrasRes[CANTPR]={
    WHILE,
    IF,
    CONST,
    DECLARE,
    ENDDECLARE,
    REAL,
    INT,
    STRING,
    MAIN,
    ELSE,
    AND,
    OR,
    PUT,
    GET
};

int main(int argc, char **argv)
{
    // inicializo matriz de estados de automata finito
    init();

    //Apertura del archivo con el lote de pruebas
    entrada = argc == 2 ? fopen(argv[1], "r") : fopen("prueba.txt", "r"); 
    if(entrada == NULL){
        printf("No se puede abrir el archivo %s\n", argc == 2 ? argv[1] : 
                                                    "prueba.txt");
        exit(ERROR);
    }

    //XXX: Para el analizador sintactico ya no la necesitamos mostrar
    if((tos = fopen("tabla_de_simbolos.txt", "w"))==NULL){
        printf("No se puede crear el archivo tabla_de_simbolos.txt\n");
        exit(ERROR);
    }

   	yyparse();

    guardarTS();
    fclose(entrada);
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
    // salto de linea
    //if (_caracter == '\n') linea++;
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
    if(cte > MAX_INT) {
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
    tipo_token =  OP_DISTINTO;
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
    fprintf(tos,"%-9s|%-20s|%-10s|%-20s|%-10s\n","Nro","Nombre","Tipo","Valor",
                                                "Longitud");
    for (i=0; i<TStop; i++){
        fprintf(tos,"%-9d|%-20s|%-10s|%-20s|%-10d\n",
                i,TS[i].nombre, TS[i].tipo, TS[i].valor, TS[i].longitud);
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
            strcpy(TS[TStop].tipo,"ID" );
            TStop++;
        break;
        case CTE_ENTERO:
            strcpy(TS[TStop].nombre, "");
            strcpy(TS[TStop].tipo,"CTE");
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
        case CTE_REAL:
            strcpy(TS[TStop].nombre,"");
            strcpy(TS[TStop].tipo,"REAL");
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
       	case CTE_STRING:
            strcpy(TS[TStop].nombre, "");
            strcpy(TS[TStop].tipo,"STRING" );
            strcpy(TS[TStop].valor, token);
            TS[TStop].longitud = (strlen(token));
            TStop++;
        break;
    }

    return TStop-1;
}

int yyerror(char *s)
{
    fprintf(stderr,"%s en linea %d\n",s,linea);
	exit(1);
}
