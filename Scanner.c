#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Scanner.h"

#define MAX_FILE 150
#define MAX_NAME_FILE 256

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsFiles
  (JNIEnv *env, jobject obt, jstring pathFirtsFile){
    const char *strFirstFile = (*env)->GetStringUTFChars(env,pathFirtsFile,NULL);
        
    char **lsArchivos = (char **) malloc(MAX_FILE * sizeof(char *) );
    lsArchivos[0] = (char *) malloc(MAX_NAME_FILE * sizeof(char) );
    
    strncpy(lsArchivos[0], strFirstFile, MAX_NAME_FILE );
    
    printf("%s\n", lsArchivos[0]);
    
    return NULL;                      
}

