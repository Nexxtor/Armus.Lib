#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tds.h"
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "Lexico.h"

tds tabla;
void clearScanner();

JNIEXPORT jobjectArray JNICALL Java_armus_lib_parser_Parser_run
(JNIEnv *env, jobject obj, jobjectArray jsLsFile) {

    //Cantidad de archivos mandados
    int cant = (*env)->GetArrayLength(env, jsLsFile);
    char **lsfiles; // Lista de archivos CONVERTIDAS a c
    void **jstrings;
    //reservado espacio para la conversion 
    lsfiles = (char **) malloc(sizeof (const char *) * cant);
    jstrings = (char **) malloc(sizeof (jstring *) * cant);
    int i;
    //Conversion 
    for (i = 0; i < cant; i++) {
        jstring string = (jstring) ((*env)->GetObjectArrayElement(env, jsLsFile, i));
        const char *file = (*env)->GetStringUTFChars(env, string, 0);

        lsfiles[i] = file;
        jstrings[i] = &string;
        printf("%s\n", lsfiles[i]);
    }

    tabla.izq = NULL;
    tabla.dch = NULL;
    tabla.valor = NULL;

    tds *incioTDS = &tabla;

    pasada1(lsfiles, cant);

    //Relase Java things
    for (i = 0; i < cant; i++) {
        (*env)->ReleaseStringUTFChars(env, *((jstring *) jstrings[i]), lsfiles[i]);
    }

    return NULL;
}

int pasada1(char **lsfiles, int cant) {
    int i;
    clearScanner();
    //Hacerlo con cada archivo
    for (i = 0; i < cant; i++) {
        fp = fopen(lsfiles[i], "r");
        if (fp == NULL) {
            return -1; //panico si no se puede abrir el archivo
        }

        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;
        struct nodoArchivo *miArchivo;
        instarArchivoTDS(lsfiles[i],&tabla,&miArchivo);

        printf("Se Inserto %s\n", miArchivo->nombre);
        obtoken();
        programa();
        fclose(fp);
        fp = NULL;
    }
    return 1;
}

int pasada2(char **lsfiles, int cant) {

}

void clearScanner() {
    primerError = 0;
    inicializar_espec();
    inicializarArbolPalabras();
    iniciarParamentros();
}

void programa(struct nodoArchivo archivo){
    do{
        if(token == incluirTok){
            obtoken();
            if(token == datoCadena){
                 obtoken();
                 if(token == puntoycoma){
                     //guardar en la tabla
                     printf("Se va a guardar en la tabla %s \n", valorCadena);
                     obtoken();
                 }else{
                     log_error(2); // falto punto y coma;
                 }
            }else{
                log_error(1); // No esta bien escrita la cadena;
            }
        }
    }while(token == incluirTok);
}
//hola