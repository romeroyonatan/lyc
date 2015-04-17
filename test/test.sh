#!/bin/bash

OUT=/tmp/out.txt

# Colores
Color_Off='\e[0m'       # Text Reset
BRed='\e[1;31m'         # Red
BGreen='\e[1;32m'       # Green

for ENTRADA in "pruebagral.txt"
do
    SALIDA=$(echo $ENTRADA | cut -d. -f1).out.txt
    echo $SALIDA
    ./lex $ENTRADA > $OUT
    diff $OUT $SALIDA > /dev/stderr
    
    if [ $? -eq 0 ]; then
        echo -e "$ENTRADA\t [${BGreen}OK${Color_Off}]"
    else 
        echo -e "$ENTRADA\t" "[${BRed}FAIL${Color_Off}]"
    fi
done
