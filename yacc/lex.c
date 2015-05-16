
# line 8 "ultimo19.y"
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
#define LONG_MAX 30 //largo maximo de los string y nombre de id
#define MAX_INT 65535 //largo maximo de los enteros de 16 bit
#define MAX_REAL FLT_MAX  //largo maximo de los reales de 32 bit
#define TAMMAX 100

/*---------------------------------------------*/
#define OP_ASIG 257
#define OP_IGUAL 258
#define OP_MENOR 259
#define OP_MAYOR 260
#define OP_MAYOR_IGUAL 261
#define OP_MENOR_IGUAL 262
#define OP_DISTINTO 263
#define OP_CONCATENAR 264
#define DOS_PUNTOS 265
#define PUNTO_Y_COMA 266
#define PARENT_ABRE 267
#define PARENT_CIERRA 268
#define COMA 269
#define LLAVE_CIERRA 270
#define LLAVE_ABRE 271
#define OP_SUMA 272
#define OP_MENOS 273
#define OP_MUL 274
#define OP_DIV 275
#define PUT 276
#define GET 277
#define WHILE 278
#define IF 279
#define CONST 280
#define DECLARE 281
#define ENDDECLARE 282
#define REAL 283
#define INT 284
#define STRING 285
#define MAIN 286
#define ELSE 287
#define AND 288
#define OR 289
#define NEGAR 290
#define ID 291
#define CTE_ENTERO 292
#define CTE_STRING 293
#define CTE_REAL 294
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 159 "ultimo19.y"

/* FUNCIONES AUXILIARES */
/* ------------------------------------------------------------------------- */
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

int yylex();
char proximo_caracter();
void get_elementos_esperados(char*);
int get_evento(char);
int esPalabraRes();
void a_minuscula (char*);
int insertarTS();
void guardarTS();
void guardarToken();


int nuevo_estado[CANT_ESTADOS][CANT_TERMINALES];
void (*proceso[CANT_ESTADOS][CANT_TERMINALES])();


/* TABLA DE SIMBOLOS */
struct tablaDeSimbolos
{
    char nombre[100];
    char tipo  [11];
    char valor [100];
    char ren   [31];
    int longitud;
};
struct tablaDeSimbolos TS[TAMMAX];

/*----------VARIABLES GLOBALES-----------------*/

int yylval;
FILE *salida,*entrada, *tos;
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
int main()
{
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


    //------------------------------------------------------//

    //Apertura del archivo con el lote de pruebas
    if((entrada = fopen("pruebagral.txt", "r")) == NULL){
        printf("No se puede abrir el archivo pruebagral.txt\n");
        exit(1);
    }

    if((salida = fopen("pruebagral_resultado.txt", "w"))==NULL){
    	printf("No se puede crear el archivo pruebagral_resultado.txt\n");
    	exit(1);
    }

    if((tos = fopen("tabla_de_simbolos.txt", "w"))==NULL){
        printf("No se puede crear el archivo tabla_de_simbolos.txt\n");
        exit(1);
    }

    while(!feof(entrada)){
    	yyparse();
    }

    guardarTS();

    fclose(entrada);
    fclose(salida);
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
    {
        fseek(entrada,-sizeof(char),SEEK_CUR);
    }
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
    tipo_token =  OP_SUMA;
}
void op_menos()
{
	tipo_token =  OP_MENOS;
}
void op_menos2()
{
    fseek(entrada,-sizeof(char),SEEK_CUR);
    tipo_token =  OP_MENOS;
}
void op_mul()
{
	tipo_token =  OP_MUL;
}
void op_div()
{
	tipo_token =  OP_DIV;
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
    tipo_token =   DOS_PUNTOS;
}
void op_negar()
{
	tipo_token =   NEGAR;
}
void puntoycoma()
{
    tipo_token =   PUNTO_Y_COMA;
}
void par_abre()
{
    tipo_token =   PARENT_ABRE;
}
void par_cierra()
{
    tipo_token =   PARENT_CIERRA;
}
void llave_abre()
{
    tipo_token =   LLAVE_ABRE;
}
void llave_cierra(tipo)
{
    tipo_token =   LLAVE_CIERRA;
}
void coma()
{
    tipo_token =   COMA;
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
    if (strlen(token) > LONG_MAX)
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
    if (strlen(token) > LONG_MAX)
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
    char aux[LONG_MAX];
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
    fprintf(tos,"Nro\t | Nombre\t\t\t | Tipo\t | Valor\t | Longitud \n");
    for (i=0; i<TStop; i++){
        fprintf(tos,"%d     \t | %s     \t\t\t | %s     \t | %s \t | %d \n",i,TS[i].nombre, TS[i].tipo, TS[i].valor, TS[i].longitud);
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
            strcpy(TS[TStop].nombre, "_");
            strcpy(TS[TStop].tipo,"CTE");
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
        case CTE_REAL:
            strcpy(TS[TStop].nombre,"_");
            strcpy(TS[TStop].tipo,"REAL");
            strcpy(TS[TStop].valor, token);
   			TStop++;
		break;
       	case CTE_STRING:
            strcpy(TS[TStop].nombre, "_");
            strcpy(TS[TStop].tipo,"STRING" );
            strcpy(TS[TStop].valor, token);
            TS[TStop].longitud = (strlen(token));
            TStop++;
        break;
    }

    return TStop-1;
}


void guardarToken()
{

   switch(tipo_token)
   {
   	   	   case ID:
   	   		    fprintf(salida,"< ID: %s >\n",token);
                break;
           case OP_SUMA:
                fprintf(salida,"< OP_SUMA >\n");
                break;
           case OP_MENOS:
                fprintf(salida,"< OP_MENOS >\n");
                break;
           case OP_MUL:
			   fprintf(salida,"< OP_MUL >\n");
			   break;
		  case OP_DIV:
			   fprintf(salida,"< OP_DIV >\n");
			   break;
		  case OP_ASIG:
			   fprintf(salida,"< OP_ASIG >\n");
			   break;
		  case OP_IGUAL:
			   fprintf(salida,"< OP_IGUAL >\n");
			   break;
		  case OP_MENOR:
			   fprintf(salida,"< OP_MENOR >\n");
			   break;
		  case OP_MAYOR:
			   fprintf(salida,"< OP_MAYOR >\n");
			   break;
		  case OP_MAYOR_IGUAL:
			   fprintf(salida,"< OP_MAYOR_IGUAL  >\n");
			   break;
		  case OP_MENOR_IGUAL:
			   fprintf(salida,"< OP_MENOR_IGUAL >\n");
			   break;
		  case DOS_PUNTOS:
			   fprintf(salida,"< DOS_PUNTOS >\n");
			   break;
		  case OP_DISTINTO:
			   fprintf(salida,"< OP_DISTINTO >\n");
			   break;
		  case PUNTO_Y_COMA:
			   fprintf(salida,"< PUNTO_Y_COMA >\n");
			   break;
		  case PARENT_ABRE:
			   fprintf(salida,"< PARENT_ABRE >\n");
			   break;
		  case PARENT_CIERRA:
			   fprintf(salida,"< PARENT_CIERRA >\n");
			   break;
		  case COMA:
			   fprintf(salida,"< COMA >\n");
			   break;
		  case CTE_ENTERO:
			   fprintf(salida,"< CTE_ENTERO: %s >\n",token);
			   break;
		  case CTE_REAL:
			   fprintf(salida,"< CTE_REAL: %s >\n",token);
			   break;
		  case CTE_STRING:
		  	   fprintf(salida,"< CTE_STRING: %s >\n",token);
			   break;
		  case OP_CONCATENAR:
			   fprintf(salida,"< OP_CONCATENAR >\n");
			   break;
		  case PUT:
			   fprintf(salida,"< PUT >\n");
			   break;
		  case GET:
			   fprintf(salida,"< GET >\n");
			   break;
		  case NEGAR:
			   fprintf(salida,"< NEGAR >\n");
			   break;
		  case LLAVE_ABRE:
			   fprintf(salida,"< LLAVE_ABRE >\n");
			   break;
		  case LLAVE_CIERRA:
			   fprintf(salida,"< LLAVE_CIERRA >\n");
			   break;
          default:
        	   fprintf(salida,"< PR: %s >\n",token);
        	   break;
     }


}
int yyerror(char *s)
{
    fprintf(stderr,"%s en linea %d\n",s,linea);
	exit(1);
}
FILE *yytfilep;
char *yytfilen;
int yytflag = 0;
int svdprd[2];
char svdnams[2][2];

int yyexca[] = {
  -1, 1,
  0, -1,
  -2, 0,
  -1, 84,
  287, 10,
  -2, 9,
  0,
};

#define YYNPROD 53
#define YYLAST 196

int yyact[] = {
      26,      28,      27,      29,      26,      91,      17,      92,
      26,      89,      31,      90,      32,      58,      33,      43,
      44,      95,      36,      35,      37,      54,      55,      20,
      24,      28,      27,      29,      24,      28,      27,      29,
      61,      28,      62,      29,      98,      52,      53,      40,
      96,      94,       8,       9,       7,       6,      11,       8,
       9,       7,       6,      11,      39,      80,      42,      79,
      41,      12,      84,      52,      53,      83,      12,      82,
       8,       9,       7,       6,      11,       8,       9,       7,
       6,      11,       4,      81,      38,      25,      19,      12,
      21,      60,      45,      34,      12,       8,       9,       7,
       6,      11,      48,      46,      50,      51,      47,      49,
       3,       5,      67,      13,      12,      14,      23,      66,
      52,      53,      10,      56,      16,      57,      42,      14,
       5,      18,      22,      93,      15,       2,       1,      59,
       0,      30,       0,       0,      68,      64,       0,      69,
      70,      71,      72,      73,      74,       0,       0,       0,
       0,      38,       0,      65,      46,       0,       0,       0,
      18,      85,      86,      66,      34,      63,      47,      48,
      49,      50,      51,      21,       0,      77,      78,      88,
       0,       0,      80,      14,      67,      19,      25,      75,
      76,       0,       0,       0,      16,       0,       0,       0,
       0,      87,       0,       0,      55,      23,       0,       0,
      96,      14,       0,       0,       0,       0,      53,      22,
       0,      97,       3,      14,
};

int yypact[] = {
    -207,   -1000,    -191,    -191,    -285,   -1000,    -267,    -267,
    -281,    -277,   -1000,    -265,    -181,    -191,   -1000,    -230,
   -1000,    -209,    -217,    -273,   -1000,    -168,    -253,   -1000,
   -1000,   -1000,    -263,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -278,   -1000,   -1000,   -1000,    -259,   -1000,
    -285,    -265,    -191,   -1000,   -1000,    -263,    -263,    -263,
    -263,    -263,    -263,    -263,    -263,    -263,    -263,    -263,
    -213,    -218,    -182,    -235,   -1000,    -201,    -203,   -1000,
   -1000,    -212,    -263,    -263,   -1000,    -235,    -235,    -235,
    -235,    -235,    -235,    -253,    -253,   -1000,   -1000,   -1000,
    -191,    -291,    -282,    -286,   -1000,   -1000,   -1000,    -229,
   -1000,   -1000,   -1000,   -1000,   -1000,    -270,   -1000,    -231,
    -191,    -234,   -1000,
};

int yypgo[] = {
       0,     118,     117,      96,     116,     108,      83,      97,
     113,     115,     109,     106,      77,      78,     103,      98,
      82,      80,      81,     114,     102,
};

int yyr1[] = {
       0,       1,       1,       2,       4,       4,       5,       3,
       3,       7,       9,       7,      10,       7,       7,       7,
       7,       7,       7,       6,       6,       6,      12,      12,
      12,      14,       8,      15,       8,      16,       8,       8,
      13,      13,      13,      13,      13,      13,      11,      11,
      17,      17,      17,      19,      19,      19,      20,      20,
      20,      18,      18,      18,      18,
};

int yyr2[] = {
       2,       2,       1,       3,       1,       3,       3,       1,
       2,       5,       0,      10,       0,       6,       2,       2,
       2,       1,       5,       1,       1,       1,       1,       1,
       1,       0,       4,       0,       4,       0,       3,       1,
       3,       3,       3,       3,       3,       3,       3,       3,
       3,       3,       1,       3,       3,       1,       1,       1,
       3,       3,       3,       3,       3,
};

int yychk[] = {
   -1000,      -1,      -2,      -3,     281,      -7,     279,     278,
     276,     277,     -11,     280,     291,      -3,      -7,      -4,
      -5,     291,      -8,     -13,     290,     -17,     -19,     -20,
     291,     -12,     267,     293,     292,     294,      -8,     291,
     293,     291,      -6,     284,     283,     285,     257,     282,
     269,     265,     271,     288,     289,     -16,     259,     262,
     258,     263,     260,     261,     272,     273,     274,     275,
     -17,     -10,     291,     -17,     -18,     291,     293,      -5,
      -6,      -3,     -14,     -15,     -13,     -17,     -17,     -17,
     -17,     -17,     -17,     -19,     -19,     -20,     -20,     268,
     271,     257,     264,     264,     270,     -13,     -13,      -3,
     -12,     291,     293,     291,     293,      -9,     270,     287,
     271,      -3,     270,
};

int yydef[] = {
       0,      -2,       0,       2,       0,       7,       0,       0,
       0,       0,      17,       0,       0,       1,       8,       0,
       4,       0,       0,      31,      29,       0,      42,      45,
      46,      47,       0,      22,      23,      24,      12,      14,
      15,      16,       0,      19,      20,      21,       0,       3,
       0,       0,       0,      25,      27,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      38,      39,      46,      22,       5,
       6,       0,       0,       0,      30,      32,      33,      34,
      35,      36,      37,      40,      41,      43,      44,      48,
       0,       0,       0,       0,      -2,      26,      28,       0,
      18,      49,      50,      51,      52,       0,      13,       0,
       0,       0,      11,
};

int *yyxi;


/*****************************************************************/
/* PCYACC LALR parser driver routine -- a table driven procedure */
/* for recognizing sentences of a language defined by the        */
/* grammar that PCYACC analyzes. An LALR parsing table is then   */
/* constructed for the grammar and the skeletal parser uses the  */
/* table when performing syntactical analysis on input source    */
/* programs. The actions associated with grammar rules are       */
/* inserted into a switch statement for execution.               */
/*****************************************************************/


#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200
#endif
#ifndef YYREDMAX
#define YYREDMAX 1000
#endif
#define PCYYFLAG -1000
#define WAS0ERR 0
#define WAS1ERR 1
#define WAS2ERR 2
#define WAS3ERR 3
#define yyclearin pcyytoken = -1
#define yyerrok   pcyyerrfl = 0
YYSTYPE yyv[YYMAXDEPTH];     /* value stack */
int pcyyerrct = 0;           /* error count */
int pcyyerrfl = 0;           /* error flag */
int redseq[YYREDMAX];
int redcnt = 0;
int pcyytoken = -1;          /* input token */


yyparse()
{
  int statestack[YYMAXDEPTH]; /* state stack */
  int      j, m;              /* working index */
  YYSTYPE *yypvt;
  int      tmpstate, tmptoken, *yyps, n;
  YYSTYPE *yypv;


  tmpstate = 0;
  pcyytoken = -1;
#ifdef YYDEBUG
  tmptoken = -1;
#endif
  pcyyerrct = 0;
  pcyyerrfl = 0;
  yyps = &statestack[-1];
  yypv = &yyv[-1];


  enstack:    /* push stack */
#ifdef YYDEBUG
    printf("at state %d, next token %d\n", tmpstate, tmptoken);
#endif
    if (++yyps - &statestack[YYMAXDEPTH] > 0) {
      yyerror("pcyacc internal stack overflow");
      return(1);
    }
    *yyps = tmpstate;
    ++yypv;
    *yypv = yyval;


  newstate:
    n = yypact[tmpstate];
    if (n <= PCYYFLAG) goto defaultact; /*  a simple state */


    if (pcyytoken < 0) if ((pcyytoken=yylex()) < 0) pcyytoken = 0;
    if ((n += pcyytoken) < 0 || n >= YYLAST) goto defaultact;


    if (yychk[n=yyact[n]] == pcyytoken) { /* a shift */
#ifdef YYDEBUG
      tmptoken  = pcyytoken;
#endif
      pcyytoken = -1;
      yyval = yylval;
      tmpstate = n;
      if (pcyyerrfl > 0) --pcyyerrfl;
      goto enstack;
    }


  defaultact:


    if ((n=yydef[tmpstate]) == -2) {
      if (pcyytoken < 0) if ((pcyytoken=yylex())<0) pcyytoken = 0;
      for (yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=tmpstate); yyxi += 2);
      while (*(yyxi+=2) >= 0) if (*yyxi == pcyytoken) break;
      if ((n=yyxi[1]) < 0) { /* an accept action */
        if (yytflag) {
          int ti; int tj;
          yytfilep = fopen(yytfilen, "w");
          if (yytfilep == NULL) {
            fprintf(stderr, "Can't open t file: %s\n", yytfilen);
            return(0);          }
          for (ti=redcnt-1; ti>=0; ti--) {
            tj = svdprd[redseq[ti]];
            while (strcmp(svdnams[tj], "$EOP"))
              fprintf(yytfilep, "%s ", svdnams[tj++]);
            fprintf(yytfilep, "\n");
          }
          fclose(yytfilep);
        }
        return (0);
      }
    }


    if (n == 0) {        /* error situation */
      switch (pcyyerrfl) {
        case WAS0ERR:          /* an error just occurred */
          yyerror("syntax error");
          yyerrlab:
            ++pcyyerrct;
        case WAS1ERR:
        case WAS2ERR:           /* try again */
          pcyyerrfl = 3;
	   /* find a state for a legal shift action */
          while (yyps >= statestack) {
	     n = yypact[*yyps] + YYERRCODE;
	     if (n >= 0 && n < YYLAST && yychk[yyact[n]] == YYERRCODE) {
	       tmpstate = yyact[n];  /* simulate a shift of "error" */
	       goto enstack;
            }
	     n = yypact[*yyps];


	     /* the current yyps has no shift on "error", pop stack */
#ifdef YYDEBUG
            printf("error: pop state %d, recover state %d\n", *yyps, yyps[-1]);
#endif
	     --yyps;
	     --yypv;
	   }


	   yyabort:
            if (yytflag) {
              int ti; int tj;
              yytfilep = fopen(yytfilen, "w");
              if (yytfilep == NULL) {
                fprintf(stderr, "Can't open t file: %s\n", yytfilen);
                return(1);              }
              for (ti=1; ti<redcnt; ti++) {
                tj = svdprd[redseq[ti]];
                while (strcmp(svdnams[tj], "$EOP"))
                  fprintf(yytfilep, "%s ", svdnams[tj++]);
                fprintf(yytfilep, "\n");
              }
              fclose(yytfilep);
            }
	     return(1);


	 case WAS3ERR:  /* clobber input char */
#ifdef YYDEBUG
          printf("error: discard token %d\n", pcyytoken);
#endif
          if (pcyytoken == 0) goto yyabort; /* quit */
	   pcyytoken = -1;
	   goto newstate;      } /* switch */
    } /* if */


    /* reduction, given a production n */
#ifdef YYDEBUG
    printf("reduce with rule %d\n", n);
#endif
    if (yytflag && redcnt<YYREDMAX) redseq[redcnt++] = n;
    yyps -= yyr2[n];
    yypvt = yypv;
    yypv -= yyr2[n];
    yyval = yypv[1];
    m = n;
    /* find next state from goto table */
    n = yyr1[n];
    j = yypgo[n] + *yyps + 1;
    if (j>=YYLAST || yychk[ tmpstate = yyact[j] ] != -n) tmpstate = yyact[yypgo[n]];
    switch (m) { /* actions associated with grammar rules */

      case 1:
# line 88 "ultimo19.y"
      {puts("Fin de programa");} break;
      case 6:
# line 96 "ultimo19.y"
      {puts("Declaracion de variable");} break;
      case 9:
# line 101 "ultimo19.y"
      {puts("Fin IF");} break;
      case 10:
# line 102 "ultimo19.y"
      {puts("Fin IF");} break;
      case 11:
# line 103 "ultimo19.y"
      {puts("Fin ELSE");} break;
      case 12:
# line 104 "ultimo19.y"
      {puts("Inicio WHILE");} break;
      case 13:
# line 104 "ultimo19.y"
      {puts("Fin WHILE");} break;
      case 14:
# line 105 "ultimo19.y"
      {puts("Mostrar por pantalla ID");} break;
      case 15:
# line 106 "ultimo19.y"
      {puts("Mostrar por pantalla STRING");} break;
      case 16:
# line 107 "ultimo19.y"
      {puts("Leer por pantalla ID");} break;
      case 18:
# line 109 "ultimo19.y"
      {puts("Declaracion de CTE con nombre");} break;
      case 25:
# line 121 "ultimo19.y"
      {puts("AND");} break;
      case 27:
# line 122 "ultimo19.y"
      {puts("OR");} break;
      case 29:
# line 123 "ultimo19.y"
      {puts("NEGAR");} break;
      case 32:
# line 127 "ultimo19.y"
      {puts("Comparacion MENOR");} break;
      case 33:
# line 128 "ultimo19.y"
      {puts("Comparacion MENOR_IGUAL");} break;
      case 34:
# line 129 "ultimo19.y"
      {puts("Comparacion IGUALDAD");} break;
      case 35:
# line 130 "ultimo19.y"
      {puts("Comparacion DISTINTO");} break;
      case 36:
# line 131 "ultimo19.y"
      {puts("Comparacion MAYOR");} break;
      case 37:
# line 132 "ultimo19.y"
      {puts("Comparacion MAYOR_IGUAL");} break;
      case 38:
# line 135 "ultimo19.y"
      {puts("Asignacion");} break;
      case 39:
# line 136 "ultimo19.y"
      {puts("Asignacion");} break;
      case 40:
# line 139 "ultimo19.y"
      {puts("SUMA");} break;
      case 41:
# line 140 "ultimo19.y"
      {puts("RESTA");} break;
      case 43:
# line 144 "ultimo19.y"
      {puts("MULITPLICACION");} break;
      case 44:
# line 145 "ultimo19.y"
      {puts("DIVISION");} break;
      case 49:
# line 154 "ultimo19.y"
      {puts("Concatena ID con ID");} break;
      case 50:
# line 155 "ultimo19.y"
      {puts("Concatena ID con CTE_STRING");} break;
      case 51:
# line 156 "ultimo19.y"
      {puts("Concatena CTE_STRING con ID");} break;
      case 52:
# line 157 "ultimo19.y"
      {puts("Concatena CTE_STRING con CTE_STRING");} break;    }
    goto enstack;
}
