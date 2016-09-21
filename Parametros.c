#include <stdlib.h>
#include <stdio.h>
#include "Standar.h";

int MAX_LINEA = 1024;
int MAX_ID = 1024;
int MAX_DIGIT  = 20;
int MAX_FILE = 150;
int MAX_NAME_FILE = 1024;
int MAX_CADENA = 123456;
int MAX_SIZE_FILE = 99999;
FILE *fp;
//Asume un archiv correcto
void iniciarParamentros(){
    int tmp_lines = MAX_LINEA;
    linea  = malloc(tmp_lines);
    lex = malloc(MAX_ID +1);
    valorCadena = malloc(MAX_CADENA);
    
    fp = fopen("/opt/armus/setings.txt","r");
    if(fp == NULL){
        return;
    }
    getLine(linea, tmp_lines);
    MAX_LINEA = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_ID = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_DIGIT = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_FILE = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_NAME_FILE = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_CADENA = atoi(linea);
    getLine(linea, tmp_lines);
    MAX_SIZE_FILE = atoi(linea);
    
    free(linea);
    free(lex);
    free(valorCadena);
    linea  = malloc(MAX_LINEA);
    lex = malloc(MAX_ID +1);
    valorCadena = malloc(MAX_CADENA);
    
}