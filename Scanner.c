#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Scanner.h"
#include "Errores.h"

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsFiles
  (JNIEnv *env, jobject obt, jstring pathFirtsFile){
    int poslinea=0;
    FILE *fp;
    const char *strFirstFile = (*env)->GetStringUTFChars(env,pathFirtsFile,NULL);
        
    char **lsArchivos = (char **) malloc(MAX_FILE * sizeof(char *) );
    lsArchivos[0] = (char *) malloc(MAX_NAME_FILE * sizeof(char) );
    
    strncpy(lsArchivos[0], strFirstFile, MAX_NAME_FILE );
    
    
    fp=fopen(lsArchivos[0],"r");
    if(fp==NULL){
        //primer error
        log_error(0);
    }
    poslinea = getline(linea,MAX_LINEA);    
    
    printf("%s\n", lsArchivos[0]);
    
    return NULL;                      
}


//getline: lee la siguiente línea del fuente y regresa su tamaño (incluyendo '\n') o 0 si EOF. (por ejemplo: para VAR regresa 4)
//es probablemente la rutina de más bajo nivel del compilador
//tomada de "El Lenguaje de programación C" - Kernighan & Ritchie - pag 28                        

int getline(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getc(fp)) != EOF && c != '\n'; ++i)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';
    return (i);
}
