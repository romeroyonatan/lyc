#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

//TOKENS
#define QFIN 33
#define OP_SUMA 257
#define OP_MENOS 258
#define OP_MUL 259
#define OP_DIV 260
#define OP_ASIG 261
#define OP_IGUAL 262
#define OP_MENOR 263
#define OP_MAYOR 264
#define OP_MAYOR_IGUAL 265
#define OP_MENOR_IGUAL 266
#define DOS_PUNTOS 267
#define OP_DISTINTO 268
#define PUNTO_Y_COMA 269
#define PARENT_ABRE 270
#define PARENT_CIERRA 271
#define COMA 272
#define CTE 273
#define STRING 274
#define OP_CONCATENAR 275
#define OUTPUT 276
#define INPUT 277
#define MAIN 278
#define AND 280
#define OR 281
#define IF 282
#define ELSE 283
#define WHILE 284
#define ID 287
#define DECLARE 288
#define ENDDECLARE 289
#define NEGAR 290
#define REAL 294
#define INT 295
#define CONST 296
#define LLAVE_ABRE 299
#define LLAVE_CIERRA 300
#define COM 301
#define ERROR -1
//////////////////////////////

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


//Funciones de la matriz
void limpiar_token();
int inic_com();
int cont_com();
int fin_com();
int op_suma();
int op_menos();
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int op_menos2();//lo puse para el caso de ---/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int op_mul();
int op_div();
int op_asig();
int op_asig();
int op_menor();
int op_menor_ig();
int op_mayor();
int op_mayor_ig();
int dos_puntos();
int op_negar();
int puntoycoma();
int par_abre();
int par_cierra();
int llave_abre();
int llave_cierra();
int coma();
int inic_id();
int cont_id();
int fin_id();
int inic_real();
int cont_real();
int fin_real();
int inic_cte();
int cont_cte();
int fin_cte();
int inic_string();
int cont_string();
int fin_string();
int punto();
int salto_linea();
int op_concaten();
int op_igualdad();
int op_distinto();
int error();
int nada();
//////////////////////////

int yylex();
char proximo_caracter();
void get_elementos_esperados(char*);
int get_evento(char);
int esPalabraRes();
void a_minuscula (char*);

int nuevo_estado[CANT_ESTADOS][CANT_TERMINALES];
int (*proceso[CANT_ESTADOS][CANT_TERMINALES])();

/*----------VARIABLES GLOBALES-----------------*/

FILE * entrada,*salida,*pruebagral;
int tipo_token; //numero identificador del token
int linea = 0; //linea por la que esta leyendo
int estado = 0; // estado actual
int longitud; //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo
char archivo[50]; //linea que se lee del archivo principal
char archivo_entrada[50]; //nombre del archivo de entrada
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
  OR
};

/*---------------------------------------------*/
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

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[31][i] = cont_com;
    proceso[31][T_EOF] = error;

    for(i = 0; i < CANT_TERMINALES; i++)
        proceso[32][i] = cont_com;
    proceso[32][T_menos] = fin_com;
    proceso[32][T_EOF] = error;

    terminal[T_mas] =               "+"; 
    terminal[T_menos] =             "-";
    terminal[T_asterisco] =         "*";  
    terminal[T_barra] =             "/"; 
    terminal[T_letra] =             "letra";   
    terminal[T_digito] =            "digito";   
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
    

    //Apertura del archivo que contiene los casos a ejecutar
    if((pruebagral = fopen("pruebagral.txt", "r"))==NULL){
        printf("No se puede abrir el archivo pruebagral.txt\n");
        exit(1);
    }

    while(!feof(pruebagral)){
        // limpio variables con las que voy a trabajar
        linea = 0;
        *archivo='\0';
        //Leo la linea con el nombre del archivo a analizar.
        fgets(archivo,50,pruebagral);
        if(strstr(archivo, "\n")){
            archivo[strlen(archivo) - 1] = '\0';
        }
        if (*archivo) {
            strcpy(archivo_entrada,archivo);
            strcat(archivo_entrada,".txt");

            //Apertura del archivo con el programa
            if((entrada = fopen(archivo_entrada, "r"))!=NULL){

                //Apertura del archivo de salida
                strcat(archivo,"_resultado.txt");
                if((salida = fopen(archivo, "w"))!=NULL){

                    while(!feof(entrada)){
                        tipo_token = yylex();
                        if(*token)
                            fprintf(salida,"%s\n",token);
                        limpiar_token();
                    }
                    fclose(salida);

                } else {
                    printf("No se puede crear el archivo %s\n", archivo);
                }

                fclose(entrada);

            } else {
                printf("No se puede abrir el archivo %s\n", archivo_entrada);
            }
        }
    }

    fclose(pruebagral);
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
            tipo_token = (proceso [estado] [columna]) ();
            estado = nuevo_estado [estado] [columna];
        }
        else
        {
            /* +++++++++++++++++++++++++++++++++++++++++++++++++ */
            tipo_token = (proceso [estado] [22]) ();
            /* +++++++++++++++++++++++++++++++++++++++++++++++++ */
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
    if (_caracter == '\n') linea++;
    // devuelvo caracter leido
    return _caracter;  
}


void limpiar_token()
{
  *token = '\0';
}
int inic_com()
{
    limpiar_token();
    return COM;
}
int cont_com()
{
    return COM;
}
int fin_com()
{
    return COM;
}
int op_suma()
{
    strcpy(token,"<OP_SUMA>");
    return OP_SUMA;
}
int op_menos()
{
    strcpy(token,"<OP_RESTA>");
    return OP_MENOS;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int op_menos2()
{
    fseek(entrada,-sizeof(char),SEEK_CUR);
    strcpy(token,"<OP_RESTA>");
    return OP_MENOS;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int op_mul()
{
    strcpy(token,"<OP_MULTIPLICACION>");
    return OP_MUL;
}
int op_div()
{
    strcpy(token,"<OP_DIVISION>");
    return OP_DIV;
}
int op_asig()
{
    strcpy(token,"<OP_ASIGNACION>");
    return OP_ASIG;
}

int op_menor()
{
    strcpy(token,"<OP_MENOR>");
    return OP_MENOR;
}
int op_menor_ig()
{
    limpiar_token();
    strcpy(token,"<OP_MENOR_IGUAL>");
    return OP_MENOR_IGUAL;
}
int op_mayor()
{
    strcpy(token,"<OP_MAYOR>");
    return OP_MAYOR;
}
int op_mayor_ig()
{
    limpiar_token();
    strcpy(token,"<OP_MAYOR_IGUAL>");
    return OP_MAYOR_IGUAL;
}
int dos_puntos()
{
    strcpy(token,"<DOS_PUNTOS>");
    return DOS_PUNTOS;
}
int op_negar()
{
    strcpy(token,"<OP_NEGAR>");
}
int puntoycoma()
{
    strcpy(token,"<PUNTO_Y_COMA>");
    return PUNTO_Y_COMA;
}
int par_abre()
{
    strcpy(token,"<PARENTESIS_ABRE>");
    return PARENT_ABRE;
}
int par_cierra()
{
    strcpy(token,"<PARENTESIS_CIERRA>");
    return PARENT_CIERRA;
}
int llave_abre()
{
    strcpy(token,"<LLAVE_ABRE>");
    return LLAVE_ABRE;
}
int llave_cierra(tipo)
{
    strcpy(token,"<LLAVE_CIERRA>");
    return LLAVE_CIERRA;
}
int coma()
{
    strcpy(token,"<COMA>");
    return COMA;
}
int inic_id()
{
    limpiar_token();
    strcat(token,&caracter);
    return ID;
}
int cont_id()
{
    
    strcat(token,&caracter);
    return ID;
}
int fin_id()
{
    int i;
    char tmp[LONG_MAX + 1];
    *tmp = '\0';
    if (strlen(token) > LONG_MAX)
    {
        printf("identificador demasiado largo en linea: %d", linea);
        exit(2);
    } 
    i = esPalabraRes();
    if(i != -1)
    {
        strcpy(tmp, palabrasRes[i]);
        sprintf(token, "<PR: %s>", tmp);
        return NroPalabrasRes[i];
    }
    else
    {
        strcpy(tmp, token);
        sprintf(token, "<ID: %s>", tmp);
    }
    return ID;
}
int inic_cte()
{
    limpiar_token();
    strcat(token,&caracter);
    return CTE;
}
int cont_cte()
{
    strcat(token,&caracter);
    return CTE;
}
int fin_cte()
{
    int cte = atoi(token);
    if(cte > MAX_INT)
    {
        fprintf(salida, "Entero sobrepasa limite maximo en linea: %d\n", linea);
        *token='\0';
        return 0;
    }
    sprintf(token,"<CTE: %d>", cte);
    return CTE;
}
int inic_real()
{
    limpiar_token();
    strcat(token,&caracter);
    return REAL;
}
int cont_real()
{
    strcat(token,&caracter);
    return REAL;
}
int fin_real()
{
    float cte = atof(token);
    if(cte > MAX_REAL)
    {
        fprintf(salida, "Real sobrepasa limite maximo en linea: %d", linea);
        *token='\0';
        return -1;
    }
    sprintf(token,"<REAL: %f>", cte);
    return REAL;
}
int inic_string()
{
    limpiar_token();
    //strcat(token,&caracter);
    return STRING;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int cont_string()
{
    strcat(token,&caracter);
    return STRING;
}

int fin_string()
{
    char tmp[LONG_MAX + 1];
    if (strlen(token) > LONG_MAX - 1)
    {
        fprintf(salida, "String demasiado largo en linea: %d\n", linea);
        *token='\0';
        return 0;
    }
    strcpy(tmp, token);
    sprintf(token, "<STRING: %s>", tmp);
    return STRING;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int punto()
{
    strcpy(token,"<PUNTO>");
    return CTE;
}
int salto_linea()
{
    //linea++;
}
int op_concaten()
{
    limpiar_token();
    strcpy(token,"<CONCATENAR>");
    return OP_CONCATENAR;
}
int op_igualdad()
{
    limpiar_token();
    strcpy(token,"<IGUALDAD>");
    return OP_IGUAL;
}
int op_distinto()
{
    limpiar_token();
    strcpy(token,"<DISTINTO>");
    return OP_DISTINTO;
}
int nada()
{

}

int error()
{
    char _elementos_esperados[300] = {'\0'};
    /* como es un error, descarto el contenido de token */
    *token = '\0';

    // en caso de un fin de archivo inesperado muestro mensaje y salgo
    if (caracter == EOF) {
        fprintf (stderr, "Fin de archivo inesperado \n");
        return ERROR;
    }

    // obtengo elementos esperados
    get_elementos_esperados (_elementos_esperados); 
    // muestro mensaje de error 
    fprintf (stderr, "Error lexico en linea: %d, cerca del elemento inesperado:\ 
 \"%c\"\n", linea, caracter);
    // muestro elementos esperados
    fprintf (stderr, ">>>> Elementos esperados: %s\n", _elementos_esperados);
    return ERROR;
}

void get_elementos_esperados(char *esperados)
{
    int i;
    for (i = 0; i < CANT_TERMINALES; i++)
        /* busco los terminales que no me deriven en un error, evitando EOF */
        if (i != T_EOF && proceso [estado][i] != error) {
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
