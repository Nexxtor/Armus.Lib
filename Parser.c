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
void cuerpo(struct clase *clase);
void tipoD(struct atributo *atributo);

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

    int a = pasada1(lsfiles, cant);
    printf("%d\n", a);
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

        // printf("Se Inserto %s\n", miArchivo->nombre);
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
                    //printf("Se va a guardar en la tabla %s \n", valorCadena);
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
                    // printf("\tA guardar la clase %s\n", lex);
                    //Guardamos la definicion de clase
                    // Aunque pueda estar mal escrita
                    // Solo intereza su exitencia
                    struct clase *claseActual;
                    insertarTDSClase(archivo, lex, tokeAux, &claseActual);

                    printf("Se inserto la clase %s con hash %d\n", claseActual->ident, claseActual->hash);
                    //Aunque no interese mucho que esta 100% bien 
                    // escrita si tiene que usar { cuerpo }
                    // para poder detectar todas las clases
                    // del mismo archivo
                    // Y evitar ambiguedades en el cuerpo
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        printf("\tRevisando el cuerpo\n");
                        cuerpo(claseActual);
                        if (token == llaveF) {
                            printf("\tClase bien escrita\n");
                            obtoken();
                        } else {
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
        } while (token != -1);
    }
}

void cuerpo(struct clase *clase) {
    // Se aumentara cada vez que exista una llaver de apertura
    // Se disminuira cuando se cierre
    int countLlaveI = 0;
    if (token == llaveF) {
        printf("\tClase vacia no se avanzara un token\n");
        return;
    }
    do {
        if (token == publicaTok || token == privadoTok) {
            int alcanze = token;
            obtoken();
            char *nombre;
            int propiedad = 1;
            //Podria ser una propiedad o metodo
            if (token == ident) {
                //Se copia para no perderlo
                nombre = malloc(sizeof (char)* strlen(lex));
                strcpy(nombre, lex);
                obtoken();
                propiedad = 2;
                if (token == corcheteI) {
                    propiedad = 0;
                    //Ok es un metodo
                    printf("\t\tRegistrando metodo %s\n", nombre);
                    obtoken();
                    //Por el momento me salto los parametros
                    while (token != corcheteF && token != -1) {
                        obtoken();
                    }

                }

            }

            //Es propiedad
            if (propiedad == 2 || propiedad == 1) {
                printf("\t\tPropiedad de tipo");

                struct atributo *atributo = (struct atributo*) malloc(sizeof (struct atributo));
                //ES un propieda cuyo tipo es un objeto
                if (propiedad == 2) {
                    //En la version completa buscar el hashClase
                    printf(" %s que es un objeto ", nombre);
                    atributo->esPrimitivo = 0;
                    atributo->tipo = OBJETO;
                    
                } else {
                    tipoD(atributo);
                }
                //Para que verificar el tipo
                //Y ademas venga el inicio del registro
                //De tds


                if (token == ident) {
                    printf("El nombre es %s ", lex);
                    atributo->ident = (char *) malloc(sizeof(char )* strlen(lex)+ 1);
                    strcpy(atributo->ident, lex);
                    insertarTDSAtributo(clase,atributo);
                    obtoken();
                    do {
                        //viene otra priopiedad 
                        // del mismo tipo
                        if (token == coma) {
                            obtoken();
                            //Y se llama
                            if (token == ident) {
                                struct atributo *atributo2 = (struct atributo*) malloc(sizeof (struct atributo));
                                atributo2->esPrimitivo =  atributo->esPrimitivo;
                                atributo2->tipo = atributo->tipo ;
                                atributo2->ident = (char *) malloc(sizeof(char )* strlen(lex)+ 1);
                                atributo2->tipoContenidoArreglo = atributo->tipoContenidoArreglo;
                                
                                atributo2->valor = atributo->valor; //No son copia es el mismo
                                //tenemos que hacer
                                strcpy(atributo2->ident, lex);
                                printf("\t\t luego esta %s ", lex);
                                insertarTDSAtributo(clase,atributo2);
                            } else {
                                log_error(1);
                            }
                        }
                        if (token == puntoycoma) {
                            break;
                        }
                        obtoken();
                    } while (token != puntoycoma && token != -1);
                    printf("\n");

                }
            }

        }
        obtoken();
        if (token == llaveI) countLlaveI++;
        if (token == llaveF) countLlaveI--;

    } while (countLlaveI >= 0);
}

void tipoD(struct atributo *atributo) {
    atributo->esPrimitivo = 1;
    atributo->valor = NULL; // NO se crea la instaciona hasta que sea necesario
    switch (token) {
            //facil es un objeto;
        case ident: //Para este caso en la pasda1 intereza buscar el hash de la clase
            // Y hacer lo que hace obejo y añadir un numero de instancia
            printf(" Personalizado ");
        case objetoTok:
            printf(" Objeto ");
            atributo->esPrimitivo = 0;
            atributo->tipo = OBJETO;
            break;
        case archivoTok:
            printf(" Archivo ");
            atributo->esPrimitivo = 0;
            atributo->tipo = ARCHIVO;
            break;
        case caracterTok:
            printf(" Caracter ");
            atributo->tipo = CARACTER;
            break;
        case cadenaTok:
            printf(" Cadena ");
            atributo->tipo = CADENA;
            break;
        case enteroTok:
            printf(" Entero ");
            atributo->tipo = ENTERO;
            break;
        case realTok:
            printf(" Real ");
            atributo->tipo = REAL;
            break;
        case byteTok:
            printf(" Byte ");
            atributo->tipo = BYTE;
            break;
        case booleanoTok:
            printf(" Booleano ");
            atributo->tipo = BOOLEANO;
            break;
        case arregloTok: //Caso dificil
            printf(" Arreglo que almacena de tipo <");
            obtoken();
            if (token == mnr) {
                struct atributo *valor = (struct atributo*) malloc(sizeof (struct atributo));
                obtoken();
                tipoD(valor);

                if (token == myr) {
                    atributo->esPrimitivo = 0;
                    atributo->tipo = ARREGLO;

                    if (valor->tipo == ARREGLO) {
                        //Bueno es un arrelgo de arreglo
                        atributo->tipoContenidoArreglo = ARREGLO;
                        atributo->valor = (void *) valor;
                    } else {
                        //Contiene un elememto basico wii
                        atributo->tipoContenidoArreglo = valor->tipo;
                    }
                    printf(">. ");
                }else{
                    log_error(1); //se esperaba cierre
                }
            }
            break;
        default:
            log_error(1); //NO es un tipo de dato valido
    }
    obtoken();

}