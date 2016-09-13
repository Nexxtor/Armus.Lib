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

    char **lsArchivos = (char **) malloc(MAX_FILE * sizeof (char *));
    lsArchivos[0] = (char *) malloc(MAX_NAME_FILE * sizeof (char));

    strncpy(lsArchivos[0], strFirstFile, MAX_NAME_FILE);
    ///Fin de extraer

    fp = fopen(lsArchivos[0], "r");
    if (fp == NULL) {
        //primer error
        log_error(0);
        return NULL;
    }
    //Se lee una linea del archivo
    posLinea = getLine(linea, MAX_LINEA);

    int pos = 0, comillas = 0, file = 1, posRuta = 0;

    while (posLinea > 0 && pos <= posLinea) {
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
                    //trabajamos con la siguente linea
                    posLinea = getLine(linea, MAX_LINEA);
                    pos = 0;
                    posRuta = 0; 
                    comillas = 0;
                }

                if (linea[pos] == '\"') {
                    comillas = 1;
                    pos++;
                    lsArchivos[file] == (char *) malloc(sizeof (char) * MAX_NAME_FILE);
                }
            }
        }
        if (comillas == 1) {
            if (linea[pos] == ';') {
                //Falto comillas de cierre 
                log_error(3);
                //HAy un error asumo que lo que sigue esta bueno
                posRuta = 0;
                comillas = 0;
                lsArchivos[file][0] = '\0';
            }
            if (linea[pos] == '\"') {
                comillas = 0;
                //OMite espacio entre ruta y ;
                while (pos <= posLinea
                        && (linea[pos] == ' ' || linea[pos] == '\t')
                        && linea[pos] != ';')
                    pos++;
                //Por si se acaba la linea
                if (linea[pos] == '\n') {
                    //falta punto y coma
                    log_error(3);
                    //trabajamos con la siguente linea
                    posLinea = getLine(linea, MAX_LINEA);
                    pos = 0;
                    posRuta = 0;
                }

                //Se prepara para la siguiente ruta
                if (linea[pos] == ';') {
                    // MArco el fin de la ruta
                    lsArchivos[file][posRuta] = '\0';
                    file++;
                    posRuta = 0;
                }
                pos++;
                continue;
            }
            ///PREGUNTAR SI SE ENCUETRA UN ENTER 
            if (linea[pos] == '\n') {
                //probable cadena multilinea
                log_error(4);
                posLinea = getLine(linea, MAX_LINEA);
                posRuta = 0;
                comillas = 0;
                lsArchivos[file][0] = '\0';
            } else {
                lsArchivos[file][posRuta] = linea[pos];
                posRuta++;
            }

        }
        pos++;
    }

    printf("Hola");
    return NULL;
}
