#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

JNIEXPORT jobjectArray JNICALL Java_armus_lib_parser_Parser_run
  (JNIEnv *env, jobject obj, jobjectArray jsLsFile){
    printf("Hola mundo");
    return NULL;
}

void pasada1(char **lsfiles){
    
}
void pasada2(char **lsfiles){
    
}