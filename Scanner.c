#include <jni.h>
#include <stdio.h>
#include "Scanner.h"

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsFiles
  (JNIEnv *env, jobject obt, jstring pathFirtsFile){
    const char *strFirstFile = (*env)->GetStringUTFChars(env,pathFirtsFile,NULL);
    printf("%s", strFirstFile);
    return NULL;                      
}
