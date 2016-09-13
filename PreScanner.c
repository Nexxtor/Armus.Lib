#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Errores.h"
#include "Standar.h"
#include "PreScanner.h"

FILE *fp = NULL;
char linea[MAX_LINEA];
JNIEXPORT jobjectArray JNICALL Java_armus_lib_scanner_Scanner_lsFiles
(JNIEnv *env, jobject obt, jstring pathFirtsFile) {
    int posLinea = 0;
    //**** Extraer paramentros desde java
    const char *strFirstFile = (*env)->GetStringUTFChars(env, pathFirtsFile, NULL);
    
    char **listaPreliminar;
    listaPreliminar = obtenerArchivosIncluir(strFirstFile);
    
    return NULL;
}

char** obtenerArchivosIncluir(const char *strFirstFile){
    int posLinea = 0;
    
    char **lsArchivos = (char **) malloc(MAX_FILE * sizeof (char *));
    lsArchivos[0] = (char *) malloc(MAX_NAME_FILE * sizeof (char));
    strncpy(lsArchivos[0], strFirstFile, MAX_NAME_FILE);
    
    fp = fopen(lsArchivos[0], "r");
    if (fp == NULL) {
        //primer error
        log_error(0);
        return;
    }
    //Se lee una linea del archivo
    posLinea = getLine(linea, MAX_LINEA);
    int pos = 0, comillas = 0, file = 1, posRuta = 0, guardando = 0;

    while (posLinea > 0 && pos < posLinea && file < MAX_FILE) {
       //printf("Entrado al while pos = %d, posLinea = %d, linea[pos]=%c\n",pos, posLinea,linea[pos]);
       if(linea[pos] == '\n'){
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
                    log_error(2);
                    printf("\n Hay un enter entre incluir y \" \n");
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
                    printf("\tReservando espacio\n");
                    lsArchivos[file] = (char *) malloc(sizeof (char) * MAX_NAME_FILE);
                    lsArchivos[file][0] = '\0';
                }
            }
        }
        if (comillas == 1) {
            if (linea[pos] == ';') {
                //Falto comillas de cierre 
                log_error(3);
                  printf("\n Falto comillia (\") de cierre \n");
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
                    guardando= 0;
                   
                }else{
                    printf("Falto ; ");
                    //falto punto y coma
                    log_error(3);
                    lsArchivos[file][0] = '\0';
                    posRuta = 0;
                    guardando= 0;
                }
                //Por si se acaba la linea
                if (linea[pos] == '\n') {
                    printf("\n Falto comillia (\") de cierre salto de linea inesperado \n");
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
                printf("\n Salto de linea inesperado \n");
                posLinea = getLine(linea, MAX_LINEA);
                posRuta = 0;
                comillas = 0;
                lsArchivos[file][0] = '\0';
            } else {
                if(MAX_NAME_FILE > posRuta){
                    lsArchivos[file][posRuta] = linea[pos];
                    printf("\t Guardando (%d) linea[%d] = %c\n",posRuta, pos, linea[pos]);
                    posRuta++;
                }else{
                    log_error(2);
                    printf("\nNombre de archivo demasiado largo");
                }
            }

        }
        pos++;
    }
    return lsArchivos;
}

