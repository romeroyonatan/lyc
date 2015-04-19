#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
//////////////////////////////


#define CANT_FILAS 34 //filas de la matriz de estados
#define CANT_COLUMNAS 25 //columnas de la matriz de estados
#define CANTPR 15 //cantidad de palabras reservadas
#define LARGOMAX 15//largo maximo de las palabras reservadas
#define LONG_MAX 30 //largo maximo de los string y nombre de id
#define MAX_INT 65535 //largo maximo de los enteros de 16 bit


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
int get_evento(char);
int esPalabraRes();

int nuevo_estado[CANT_FILAS][CANT_COLUMNAS]={
//        0     |  1    |  2    |  3    |  4    |  5    |  6    |  7    |  8    |  9    | 10    | 11    | 12    | 13    | 14    | 15    | 16    | 17    | 18    |  19   | 20    | 21    |  22  |    23  |   24  |
//        +     |  -    |  *    |  /    | Let   | Dig   |  =    |  <    |  >    |  ?    |  :    |  !    |  "    |  .    |  ;    |  car  | (     |   )   |  ,    | tab   |blanco | enter |  EOF |    {   |   }   |
/*E0*/  { 3 , 15  , 16  , 17  , 1 , 24  , 5 , 9 , 11  , 27  , 26  , 21  , 23  , 25  , 2 , 0 , 18  , 19  , 20  , 0 , 0 , 0 ,QFIN ,    7  ,   8   },
/*E1*/  {QFIN ,QFIN ,QFIN ,QFIN , 1 , 1 ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E2*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E3*/  { 4 ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E4*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E5*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 6 ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E6*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E7*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E8*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E9*/  {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 10  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E10*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E11*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 12  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E12*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E13*/ {QFIN , 14  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E14*/ { 14  ,  15 , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  , 14  ,     14,   14  },
/*E15*/ {QFIN ,  29 ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E16*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E17*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E18*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E19*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E20*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E21*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 22  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E22*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E23*/ { 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  ,28   , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  , 23  ,   23  ,   23  },
/*E24*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 24  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 25  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E25*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN , 25  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E26*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E27*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E28*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E29*/ {QFIN ,QFIN ,QFIN , 30  ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   },
/*E30*/ { 30  , 30  , 30  , 31  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  ,QFIN   ,QFIN , 30  , 30  },
/*E31*/ { 30  , 32  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  ,QFIN   ,QFIN , 30  , 30  },
/*E32*/ { 30  , 0   , 30  , 31  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  , 30  ,QFIN   ,QFIN , 30  , 30  },
/*E33*/ {QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN ,QFIN   ,QFIN   }
};

int (*proceso[CANT_FILAS][CANT_COLUMNAS])()={
//        0       |  1      |  2      |  3      |  4      |  5      |  6      |  7      |  8      |  9      | 10      | 11      | 12      | 13      | 14      | 15      | 16      | 17      | 18      |  19     | 20      | 21      |  22   |    23     |   24      |
//        +       |  -      |  *      |  /      | Let     | Dig     |  =      |  <      |  >      |  ?      |  :      |  !      |  "      |  .      |  ;      |  car    | (       |   )     |  ,      | tab     |blanco   | enter   |  EOF  |    {      |   }       |
/*E0*/  {op_suma  ,op_menos ,op_mul   ,op_div   ,inic_id  ,inic_cte ,op_asig  ,op_menor ,op_mayor ,nada   ,dos_puntos ,op_negar ,inic_string,inic_cte ,puntoycoma ,error    ,par_abre ,par_cierra ,coma   ,nada   ,nada   ,salto_linea,nada ,llave_abre ,llave_cierra},
/*E1*/  {fin_id   ,fin_id   ,fin_id   ,fin_id   ,cont_id  ,cont_id  ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id   ,fin_id ,nada       ,nada        },
/*E2*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E3*/  {op_concaten,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E4*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E5*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,op_igualdad,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E6*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E7*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E8*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E9*/  {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,op_menor_ig,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E10*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E11*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,op_mayor_ig,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E12*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E13*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E14*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E15*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E16*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E17*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E18*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E19*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error  ,nada       ,nada        },
/*E20*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E21*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,op_distinto,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E22*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E23*/ {cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,fin_string ,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,error  ,nada       ,nada        },
/*E24*/ {fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,cont_cte ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,cont_cte ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte,nada       ,nada        },
/*E25*/ {fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,cont_cte ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte  ,fin_cte,nada       ,nada        },
/*E26*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E27*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E28*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada ,nada       ,nada        },
/*E29*/ {op_menos2  ,op_menos2  ,op_menos2  ,inic_com ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,op_menos2  ,nada     ,nada   ,op_menos2,op_menos2   },
/*E30*/ {cont_com ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,error      ,error  ,cont_com   ,cont_com  },
/*E31*/ {cont_com ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,error      ,error  ,cont_com   ,cont_com  },
/*E32*/ {cont_com ,fin_com  ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,cont_com ,cont_com ,cont_com,  cont_com  ,cont_com ,cont_com ,error      ,error  ,cont_com   ,cont_com  },
/*E33*/ {nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,nada   ,error    ,nada ,nada       ,nada        }
};

/*----------VARIABLES GLOBALES-----------------*/

FILE * entrada,*salida,*pruebagral;
int tipo_token; //numero identificador del token
int linea =1; //linea por la que esta leyendo
int longitud; //longitud del string, id o cte
char token[200]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo
char archivo[50]; //linea que se lee del archivo principal
char archivo_entrada[50]; //nombre del archivo de entrada
char palabrasRes[CANTPR][LARGOMAX]={
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
    //Apertura del archivo que contiene los casos a ejecutar
    if((pruebagral = fopen("pruebagral.txt", "r"))==NULL){
        printf("No se puede abrir el archivo pruebagral.txt\n");
        exit(1);
    }

    while(!feof(pruebagral)){
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
                        if(*token);
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
    int estado=0;
    int estado_final=QFIN;
    while(estado!=estado_final)
    {
        if((caracter=fgetc(entrada)) != EOF)
        {
            int columna = get_evento(caracter);
            tipo_token = (proceso [estado] [columna]) ();
            estado = nuevo_estado [estado] [columna];
        }
        else
        {
            /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            tipo_token = (proceso [estado] [22]) ();
            /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
            estado=estado_final;
        }
    }
    if(!feof(entrada))
    {
        fseek(entrada,-sizeof(char),SEEK_CUR);
    }
    return tipo_token;
}

void limpiar_token()
{
  token[0] = '\0';
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
    longitud = 1;
    strcpy(token,"<ID: ");
    strcat(token,&caracter);
    return ID;
}
int cont_id()
{
    longitud++;
    if(longitud>LONG_MAX)
    {
        printf("identificador demasiado largo en linea: %d",linea);
        exit(2);
    }
    strcat(token,&caracter);
    return ID;
}
int fin_id()
{
    int i;
    i=esPalabraRes();
    if(i!=-1)
    {
        limpiar_token();
        strcpy(token,"<PR: ");
        strcat(token,palabrasRes[i]);
        strcat(token,">");
        return NroPalabrasRes[i];
    }
    else
        strcat(token,">");
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
        fprintf(salida, "Entero sobrepasa limite maximo en linea: %d",linea);
        *token='\0';
        return 0;
    }
    sprintf(token,"<CTE: %d>", cte);
    return CTE;
}
int inic_string()
{
    limpiar_token();
    longitud = 1;
    strcpy(token,"<STRING: ");
    //strcat(token,&caracter);
    return STRING;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int cont_string()
{
    longitud++;
    
    strcat(token,&caracter);
    return STRING;
}

int fin_string()
{
    if(longitud==LONG_MAX+1)
    {
        fprintf(salida, "String demasiado largo en linea: %d",linea);
        *token='\0';
        return 0;
    }
    token[9+longitud-1]='>';
    token[9+longitud]='\0';
    return STRING;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int punto()
{
    strcpy(token,"<PUNTO>");
    return CTE;
}
int salto_linea()
{
    linea++;
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
    //printf("Error en linea: %d\n",linea);
    fprintf(salida,"Error en linea: %d\n",linea);
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
    char aux[30];
    int i;
    //XXX: Sin efecto. Hay que hacer que toda la palabra pase a minuscula
    tolower(*token);
    for(i=0;i<CANTPR;i++)
    {
        if(strcmp(palabrasRes[i],&token[5])==0)
        {
            return i;
        }
    }

    return -1;
}

