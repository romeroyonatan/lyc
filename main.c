#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//TOKENS
#define QFIN 29
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
//////////////////////////////


#define CANT_FILAS 30 //filas de la matriz de estados
#define CANT_COLUMNAS 25 //columnas de la matriz de estados
#define CANTPR 12 //cantidad de palabras reservadas
#define LARGOMAX 15//largo maximo de las palabras reservadas
#define LONG_MAX 30 //largo maximo de los string y nombre de id


//Funciones de la matriz
int op_suma();
int op_menos();
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
/*E0*/	{	3	,	15	,	16	,	17	,	1	,	24	,	5	,	9	,	11	,	27	,	26	,	21	,	23	,	-1	,	2	,	-1	,	18	,	19	,	20	,	0	,	0	,	0	,QFIN	,    7  ,   8   },
/*E1*/	{QFIN	,QFIN	,QFIN	,QFIN	,	1	,	1	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E2*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E3*/	{	4	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E4*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E5*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	6	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E6*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E7*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E8*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E9*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	10	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E10*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E11*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	12	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E12*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E13*/	{QFIN	,	14	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E14*/	{	14	,	15	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,	14	,     14,   14  },
/*E15*/	{	0	,	14	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,   0   ,   0   },
/*E16*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E17*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E18*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E19*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E20*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E21*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	22	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E22*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E23*/	{	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,28 	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,	23	,   23  ,   23  },
/*E24*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	24	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	25	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E25*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,	25	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E26*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E27*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E28*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   },
/*E29*/	{QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN	,QFIN   ,QFIN   }
};

int (*proceso[CANT_FILAS][CANT_COLUMNAS])()={
//        0     	|  1    	|  2    	|  3    	|  4    	|  5    	|  6    	|  7    	|  8    	|  9    	| 10    	| 11    	| 12    	| 13    	| 14    	| 15    	| 16    	| 17    	| 18    	|  19   	| 20    	| 21    	|  22  	|    23     |   24      |
//        +     	|  -    	|  *    	|  /    	| Let   	| Dig   	|  =    	|  <    	|  >    	|  ?    	|  :    	|  !    	|  "    	|  .    	|  ;    	|  car  	| (     	|   )   	|  ,    	| tab   	|blanco 	| enter 	|  EOF 	|    {      |   }       |
/*E0*/	{op_suma		,op_menos	,op_mul		,op_div		,inic_id	,inic_cte	,op_asig	,op_menor	,op_mayor	,nada		,dos_puntos	,op_negar	,inic_string,error		,puntoycoma	,error		,par_abre	,par_cierra	,coma		,nada		,nada		,salto_linea,error	,llave_abre ,llave_cierra},
/*E1*/	{fin_id		,fin_id		,fin_id		,fin_id		,cont_id	,cont_id	,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,fin_id		,error	,nada       ,nada        },
/*E2*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E3*/	{op_concaten,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E4*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E5*/	{nada		,nada		,nada		,nada		,nada		,nada		,op_igualdad,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E6*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E7*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E8*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E9*/	{nada		,nada		,nada		,nada		,nada		,nada		,op_menor_ig,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E10*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E11*/	{nada		,nada		,nada		,nada		,nada		,nada		,op_mayor_ig,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E12*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E13*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E14*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E15*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E16*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E17*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E18*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E19*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E20*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E21*/	{nada		,nada		,nada		,nada		,nada		,nada		,op_distinto,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E22*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E23*/	{cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,fin_string	,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,cont_string,error	,nada       ,nada        },
/*E24*/	{fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,cont_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,cont_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,error	,nada       ,nada        },
/*E25*/	{fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,cont_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,fin_cte	,error	,nada       ,nada        },
/*E26*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E27*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E28*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        },
/*E29*/	{nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,nada		,error	,nada       ,nada        }
};

/*----------VARIABLES GLOBALES-----------------*/

FILE * entrada,*salida,*asembler;
int tipo_token; //numero identificador del token
int linea =1; //linea por la que esta leyendo
int longitud; //longitud del string, id o cte
char token[25]; //Nombre del token identificado
char caracter; //caracter que se lee del archivo
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
	{"or"}
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
    //if((entrada = fopen(argv[1], "r"))==NULL)
    if((entrada = fopen("entrada.txt", "r"))==NULL)
    {
        //printf("No se puede abrir el archivo %s\n",argv[1]);
        printf("No se puede abrir el archivo entrada.txt\n");
        exit(1);
    }
	 //Apertura del archivo de salida
    if((salida = fopen("intermedia.txt", "w"))==NULL)
    {
        printf("No se puede crear el archivo intermedia.txt");
        exit(2);
    }
    if((asembler = fopen("Final.asm", "w"))==NULL)
    {
        printf("No se puede crear el archivo asembler.asm");
        exit(2);
    }
    while(!feof(entrada))
    {
        tipo_token = yylex();
        printf("%s\n",token);
    }
    fclose(entrada);
    return 0;
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
    longitud = 1;
    strcpy(token,"<CTE: ");
    strcat(token,&caracter);
    return CTE;
}
int cont_cte()
{
    longitud++;
    strcat(token,&caracter);
    return CTE;
}
int fin_cte()
{
    strcat(token,">");
    return CTE;
}
int inic_string()
{
    longitud = 1;
    strcpy(token,"<STRING: ");
    //strcat(token,&caracter);
    return STRING;
}
int cont_string()
{
    longitud++;
    if(longitud>LONG_MAX)
    {
        printf("string demasiado largo en linea: %d",linea);
        exit(2);
    }
    strcat(token,&caracter);
    return STRING;
}
int fin_string()
{
    token[9+longitud-1]='>';
    return STRING;
}
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
    strcpy(token,"<CONCATENAR>");
    return OP_CONCATENAR;
}
int op_igualdad()
{
    strcpy(token,"<IGUALDAD>");
    return OP_IGUAL;
}
int op_distinto()
{
    strcpy(token,"<DISTINTO>");
    return OP_DISTINTO;
}
int nada()
{

}
int error()
{
    printf("Error en linea: %d",linea);
    exit(1);
}

int yylex()
{
    int estado=0;
    int estado_final=QFIN;
    while(estado!=estado_final)
    {
        //char caracter;
        if(fscanf(entrada,"%c",&caracter)!=EOF)
        {
            //printf("%c",caracter);
            int columna = get_evento(caracter);
            tipo_token = (proceso [estado] [columna]) ();
            estado = nuevo_estado [estado] [columna];
        }
        else
            estado=estado_final;
    }
    if(!feof(entrada))
    {
        fseek(entrada,-sizeof(char),SEEK_CUR);
    }
    return tipo_token;
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
            break;
        case '-':
            return 1;
            break;
        case '*':
            return 2;
            break;
        case '/':
            return 3;
            break;
        case '=':
            return 6;
            break;
        case '<':
            return 7;
            break;
        case '>':
            return 8;
            break;
        case '?':
            return 9;
            break;
        case ':':
            return 10;
            break;
        case '!':
            return 11;
            break;
        case '"':
            return 12;
            break;
        case '.':
            return 13;
            break;
        case ';':
            return 14;
            break;
        case '(':
            return 16;
            break;
        case ')':
            return 17;
            break;
        case ',':
            return 18;
            break;
        case '\t':
            return 19;
            break;
        case ' ':
            return 20;
            break;
        case '\n':
            return 21;
            break;
        case EOF:
            return 22;
        case '{':
            return 23;
            break;
        case '}':
            return 24;
            break;
        default:
            return 15;
    }
}

int esPalabraRes()
{
    char aux[30];
    int i;
    tolower(token);
    for(i=0;i<CANTPR;i++)
    {
        if(strcmp(palabrasRes[i],&token[5])==0)
        {
            return i;
        }
    }

    return -1;
}
