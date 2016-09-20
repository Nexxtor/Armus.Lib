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
    int cant = (*env)->GetArrayLength(env, allFiles);


    printf("\n\nCompilador de armus version 0.1\n");
    printf("CAntidad de archivos %d\n",cant);
    //inicializacion de tokens de símbolos especiales 
    inicializar_espec();
    inicializarArbolPalabras();

    int i = 0;
    for (i = 0; i < cant; i++) {
        jstring string = (jstring) ((*env)->GetObjectArrayElement(env, allFiles, i));
        const char *rawString = (*env)->GetStringUTFChars(env, string, 0);
        // Don't forget to call `ReleaseStringUTFChars` when you're done.
        fp = fopen(rawString, "r"); //abrir el fuente solo para lectura
        if(fp == NULL){
            return NULL;
        }
        printf("--------------------%s -----------------------\n", rawString);
        //inicializacion de otras variables 
        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;

        //tokenizar el programa fuente
        while (1) {

            obtoken();
            if (primerError == 1) {
                printf("PANICO \n");
                return NULL;
            }
            if (fin_de_archivo == 2) {
                break;
            }
            if (fin_de_archivo == 1) {
                fin_de_archivo++;
            }
            imprime_token();
        }
        fp = NULL;
    }
    return NULL;
}
