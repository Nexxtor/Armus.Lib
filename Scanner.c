#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "Scanner.h"
#include "Lexico.h"

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsTokens
(JNIEnv *env, jobject obj, jobjectArray allFiles) {
    int cant = 1;

    while (cant > 0) {
        fp = fopen("/home/nextor/Escritorio/proyecto/main.acl", "r"); //abrir el fuente solo para lectura

        printf("\n\nCompilador de armus version 0.1\n");
        //inicializacion de tokens de símbolos especiales 
        inicializar_espec();
        inicializarArbolPalabras();

        //inicializacion de otras variables 
        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;

        //tokenizar el programa fuente
        while (1) {
          
            obtoken();
            if(primerError == 1){
                printf("PANICO \n");
                return NULL;
            }
            if(fin_de_archivo == 2){
                break;
            }
            if(fin_de_archivo == 1){
                fin_de_archivo++;
            }
            imprime_token(); 
        }
        cant--;
        
    }
    return NULL;
}
