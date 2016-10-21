#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

JNIEXPORT jobjectArray JNICALL Java_armus_lib_parser_Parser_run
  (JNIEnv *env, jobject obj, jobjectArray jsLsFile){
    
    //Cantidad de archivos mandados
    int cant = (*env)->GetArrayLength(env, jsLsFile);
    char **lsfiles; // Lista de archivos CONVERTIDAS a c
    void **jstrings;
    //reservado espacio para la conversion 
    lsfiles = (char **) malloc(sizeof(const char *) * cant);
    jstrings = (char **) malloc(sizeof(jstring *) * cant);
    int i;
    //Conversion 
    for(i= 0 ; i <cant ; i++){
        jstring string = (jstring) ((*env)->GetObjectArrayElement(env, jsLsFile, i));
        const char *file = (*env)->GetStringUTFChars(env, string, 0);
        
        lsfiles[i] = file;
        jstrings[i] = &string;
        printf("%s\n",lsfiles[i]);
    }
    
    
    //Relase Java things
    for(i= 0 ; i <cant ; i++){
         (*env)->ReleaseStringUTFChars(env, *((jstring *) jstrings[i]), lsfiles[i]);
    }
     
    return NULL;
}

void pasada1(char **lsfiles){
    
}
void pasada2(char **lsfiles){
    
}