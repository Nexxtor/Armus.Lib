#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "PreScanner.h"

FILE *fp = NULL;
char linea[MAX_LINEA];

JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsFiles
(JNIEnv *env, jobject obt, jstring pathFirtsFile) {
    //**** Extraer paramentros desde java
    const char *strFirstFile = (*env)->GetStringUTFChars(env, pathFirtsFile, NULL);

    char **listaPreliminar;
    int i = 1;
    listaPreliminar = obtenerArchivosIncluir(strFirstFile);
    while(listaPreliminar[i][0] != '\0'){
        char**masArchivos = obtenerArchivosIncluir(listaPreliminar[i]);
        //printf("%s \n",listaPreliminar[i]);
        if(masArchivos == NULL){
            printf("-------PAnico----\n");
            return NULL;
        }
        listaPreliminar = unirListaArchivos(listaPreliminar,masArchivos);
        i++;
    }
    jobjectArray respuesta = 0;
    jsize size = i;
    respuesta = (*env)->NewObjectArray(env,size,(*env)->FindClass(env,"java/lang/String"),0);
    i = 0;
    while(listaPreliminar[i][0] != '\0'){
        (*env)->SetObjectArrayElement(env,respuesta,i, (*env)->NewStringUTF(env,listaPreliminar[i]) );
        i++;
    }

    
    
    
    return respuesta;
}

char ** unirListaArchivos(char **lista, char **masArchivos) {
    int indices[MAX_FILE] , i = 0, w = 0;
    int encontrado = 0;
    //mas archivos en lista
    while(masArchivos[i][0] != '\0'){
        int j = 0;
        
        while(lista[j][0] != '\0'){
            if(strcmp( obtenerNombreBase(masArchivos[i]),obtenerNombreBase(lista[j]) ) == 0){
              encontrado = 1;
            }
            j++;
        }
        if(encontrado == 0){
            indices[w] = i;
            w++;
        }else{
            encontrado = 0;
        }
        i++;
    }
    
    
    
    char **respuestas = (char **) malloc(sizeof(char *) * MAX_FILE);
    int copia = 0;
    
    while(lista[copia][0] != '\0'){
        respuestas[copia] = (char *) malloc(sizeof(char ) * MAX_NAME_FILE * 3);
        strncpy(respuestas[copia],lista[copia], MAX_NAME_FILE * 3);
        copia++;
    }
    for(i = 0 ; i < w; i++){
        respuestas[copia] = (char *) malloc(sizeof(char ) * MAX_NAME_FILE * 3);
        strncpy(respuestas[copia],masArchivos[indices[i]], MAX_NAME_FILE * 3);
        copia++;
    }
    respuestas[copia] = malloc(2);
    respuestas[copia][0] = '\0';
    return respuestas;
    
}

char** obtenerArchivosIncluir(const char *strFirstFile) {
    int posLinea = 0;

    char **lsArchivos = (char **) malloc(MAX_FILE * sizeof (char *));
    lsArchivos[0] = (char *) malloc(MAX_NAME_FILE * 3 * sizeof (char));
    
    strncpy(lsArchivos[0], strFirstFile, MAX_NAME_FILE);

    fp = fopen(lsArchivos[0], "r");
    if (fp == NULL) {
        log_error(0);
        return NULL;
    }
    char *rutaAbsoluta = obtenerDirectorio(strFirstFile);
    //Se lee una linea del archivo
    posLinea = getLine(linea, MAX_LINEA);
    int pos = 0, comillas = 0, file = 1, posRuta = 0, guardando = 0;

    while (posLinea > 0 && pos < posLinea) {
        //printf("Entrado al while pos = %d, posLinea = %d, linea[pos]=%c\n",pos, posLinea,linea[pos]);
        if (linea[pos] == '\n') {
            pos = comillas = posRuta = guardando = 0;
            posLinea = getLine(linea, MAX_LINEA);
        }
        if (linea[pos] == 'i' && comillas == 0) {
            if (pos + 6 <= posLinea
                    && linea[pos + 1 ] == 'n'
                    && linea[pos + 2 ] == 'c'
                    && linea[pos + 3 ] == 'l'
                    && linea[pos + 4 ] == 'u'
                    && linea[pos + 5 ] == 'i'
                    && linea[pos + 6 ] == 'r') {
                pos = pos + 7;
                //Omite todos los espacios despus de incluir
                while (pos <= posLinea
                        && (linea[pos] == ' ' || linea[pos] == '\t')
                        && linea[pos] != '\"')
                    pos++;
                //Por si se acaba la linea
                if (linea[pos] == '\n') {
                    //mal escrito el incluir falta archivo
                    log_error(1);
                    //printf("\n Hay un enter entre incluir y \" \n");
                    //trabajamos con la siguente linea
                    posLinea = getLine(linea, MAX_LINEA);
                    pos = 0;
                    posRuta = 0;
                    comillas = 0;
                }

                if (linea[pos] == '\"') {
                    comillas = 1;
                    guardando = 1;
                    pos++;
                    //printf("\tReservando espacio\n");
                    lsArchivos[file] = (char *) malloc(sizeof (char) * MAX_NAME_FILE * 3);
                    if(linea[pos] != '/'){
                        //printf("-------------------Copiando cosas --------\n ");
                        int largo = strlen(rutaAbsoluta);
                        
                        strncpy(lsArchivos[file], rutaAbsoluta, MAX_NAME_FILE * 3);
                        
                        posRuta = largo;
                        //printf("%s %s %d",rutaAbsoluta, lsArchivos[file], largo);
                         //printf("---------------- Terminando Copiando cosas --------\n ");
                    }else
                        lsArchivos[file][0] = '\0';
                }
            }
        }
        if (comillas == 1 && file < MAX_FILE) {
            if (linea[pos] == ';') {
                //Falto comillas de cierre 
                log_error(2);
               // printf("\n Falto comillia (\") de cierre \n");
                //HAy un error asumo que lo que sigue esta bueno
                posRuta = 0;
                comillas = 0;
                lsArchivos[file][0] = '\0';
            }
            if (linea[pos] == '\"') {
                comillas = 0;
                pos++;
                //OMite espacio entre ruta y ;
                while (pos <= posLinea
                        && (linea[pos] == ' ' || linea[pos] == '\t')
                        && linea[pos] != ';')
                    pos++;
                //Se prepara para la siguiente ruta
                if (linea[pos] == ';') {
                    // MArco el fin de la ruta
                    lsArchivos[file][posRuta] = '\0';
                    file++;
                    posRuta = 0;
                    guardando = 0;

                } else {
                    //printf("Falto ; ");
                    //falto punto y coma
                    log_error(3);
                    lsArchivos[file][0] = '\0';
                    posRuta = 0;
                    guardando = 0;
                }
                //Por si se acaba la linea
                if (linea[pos] == '\n') {
                    //printf("\n Falto comillia (\") de cierre salto de linea inesperado \n");
                    //trabajamos con la siguente linea
                    posLinea = getLine(linea, MAX_LINEA);
                    pos = 0;
                    posRuta = 0;
                }
                pos++;
                continue;
            }
            ///PREGUNTAR SI SE ENCUETRA UN ENTER 
            if (linea[pos] == '\n') {
                //probable cadena multilinea
                log_error(4);
                //printf("\n Salto de linea inesperado \n");
                posLinea = getLine(linea, MAX_LINEA);
                posRuta = 0;
                comillas = 0;
                lsArchivos[file][0] = '\0';
            } else {
                if (MAX_NAME_FILE * 3 > posRuta) {
                    //printf("posRuta vale %d", posRuta);
                    lsArchivos[file][posRuta] = linea[pos];
                    lsArchivos[file][posRuta+1]  = '\0';
                    //printf("\t Guardando %c %s\n",linea[pos],  lsArchivos[file]);
                    posRuta++;
                } else {
                    log_error(5);
                   // printf("\nNombre de archivo demasiado largo");
                }
            }

        }
        pos++;
    }
    
    //registar cantidad extesa de archivos
    if(file >= MAX_FILE){
        log_error(6);
        //printf("\nSe alcanzo la cantidad maxima de archivos");
        return NULL;
    }
    
    lsArchivos[file] = malloc(2);
    lsArchivos[file][0] = '\0';
    
    fclose(fp);
    fp = NULL;
    return lsArchivos;
}

 char *obtenerDirectorio(const char *strFirstFile){     
     int posSlash = 0,i=0;
     while(strFirstFile[i] != '\0') {
         if(strFirstFile[i]== '/'){
             posSlash = i;
         }
         i++;
     }
     
     char *directorio = (char *) malloc((sizeof(char )* posSlash )+ 1);
     i = 0;
     while(i <= posSlash){
         directorio[i] = strFirstFile[i];
         i++;
     }
     directorio[i] = '\0';
     
     return directorio;
 }

 char *obtenerNombreBase(const char *strFirstFile){     
     int posSlash = 0,i=0, j=0;
     while(strFirstFile[i] != '\0') {
         if(strFirstFile[i]== '/'){
             posSlash = i;
         }
         i++;
     }
     
     char *directorio = (char *) malloc(MAX_NAME_FILE + 1);
     i = posSlash + 1;
     while(strFirstFile[i] != '\0'){
         directorio[j] = strFirstFile[i];
         i++;
         j++;
     }
     directorio[j] = '\0';
     
     return directorio;
 }
