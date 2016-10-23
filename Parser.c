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
void programa(struct nodoArchivo *archivo);

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

//    tds *incioTDS = &tabla;

    int a =  pasada1(lsfiles, cant);
    printf("%d\n",a);
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
        instarArchivoTDS(lsfiles[i], &tabla, &miArchivo);

        printf("Se Inserto %s\n", miArchivo->nombre);
        obtoken();
        programa(miArchivo);
        fclose(fp);
        fp = NULL;
    }
    return 1;
}

int pasada2(char **lsfiles, int cant) {
    return 1;
}

void clearScanner() {
    primerError = 0;
    inicializar_espec();
    inicializarArbolPalabras();
    iniciarParamentros();
}

void programa(struct nodoArchivo *archivo) {
    do {
        if (token == incluirTok) {
            obtoken();
            if (token == datoCadena) {
                obtoken();
                if (token == puntoycoma) {
                    //guardar en la tabla
                    printf("Se va a guardar en la tabla %s \n", valorCadena);
                    instarIncluidosArchivo(valorCadena, archivo);
                    obtoken();
                } else {
                    log_error(2); // falto punto y coma;
                }
            } else {
                log_error(1); // No esta bien escrita la cadena;
            }
        }
    } while (token == incluirTok);

    //Si es publica podria estar escribiendo
    //Un metodo o atributo en un mal lugar
    if (token == publicaTok || token == localTok) {
        do {
            enum simbolo tokeAux = token;
            obtoken();

            if (token == claseTok) {
                //OK si es una clase
                obtoken();
                if (token == ident) {
                    //y Esta decentemente escrita
                    printf("\tA guardar la clase %s\n", lex);
                    //Guardamos la definicion de clase
                    // Aunque pueda estar mal escrita
                    // Solo intereza su exitencia
                    struct clase *claseActual;
                    insertarTDSClase(archivo, lex,tokeAux, &claseActual);
                    
                    printf("Se inserto la clase %s con hash %d\n",claseActual->ident, claseActual->hash);
                    //Aunque no intese mucho que esta 100% bien 
                    // escrita si tiene que usar { cuerpo }
                    // para poder detectar todas las clases
                    // del mismo archivo
                    obtoken();
                    if (token == llaveI) {
                        //cuerpo(claseActual);
                        printf("\t\tRevisando el cuerpo\n");
                        if (token == llaveF) {
                            printf("\t\tClase bien escrita\n");
                            obtoken();
                        }else{
                            log_error(1); //falto llave de cierre
                        }
                    } else {
                        log_error(1); //falta llave de apertura
                    }
                }
            } else {
                //Es ambiguo en este punto por eso se da los 
                //dos mensaje por que no se logra adivinar 
                //la intencion del programador
                if (tokeAux == publicaTok) {
                    log_error(1); // esta declarando una propiedad o metodo
                    // fuera de la clase  
                }

                log_error(1); // no puso la palabra clabe clase         
            }
        } while (token == publicaTok || token == localTok || token == privadoTok);
    }
}