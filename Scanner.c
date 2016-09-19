#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "Scanner.h"

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsTokens
  (JNIEnv *env, jobject obj, jobjectArray allFiles){
     printf("Hola mundo");
     return NULL;
 }