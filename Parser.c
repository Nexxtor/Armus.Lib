#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tds.h"
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "Lexico.h"
#include "Parser.h"

tds tabla;
void clearScanner();
int programaP1(struct nodoArchivo *archivo);
int cuerpoP1(struct clase *clase);
int tipoDP1(struct atributo *atributo);
void copiarValor(struct atributo *dest, struct atributo *org);
int programa(struct nodoArchivo *);
int cuerpo(struct nodoArchivo* miArchivo, struct clase *clase);
int tipo(struct nodoArchivo *miArchivo, struct clase *clase);
int metodo(struct nodoArchivo* miArchivo, struct clase *clase);
int parametro(struct nodoArchivo *miArchivo, struct clase *clase, struct metodo*);
int bloque(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int instruccion(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int instruccion_romper();
int instruccion_si(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int asignacionf();
int expresion();
int funcion_cadena();
int instruccion_mientras(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int instruccion_para();
int instruccion_es();
int expresion_numerica();
int expresion_conjuncion();
int expresion_relacional();
int expresion_aritmetrica();
int termino();
int factor();
int valor_cadena();
int valor_caracter();
int funcion_arreglo();
int funcion_archivo();
int llamada_metodo();
int funcion_num_numcad();
int instruccion_probar(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int instruccion_hacer(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
int instruccion_paraCada(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);

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
    }

    tabla.izq = NULL;
    tabla.dch = NULL;
    tabla.valor = NULL;

    if (!pasada1(lsfiles, cant)) {
        return NULL; // COnveritri arreglo y retornar
    }

    int b = pasada2(lsfiles, cant);

    //Relase Java things
    for (i = 0; i < cant; i++) {
        (*env)->ReleaseStringUTFChars(env, *((jstring *) jstrings[i]), lsfiles[i]);
    }

    return NULL;
}

int pasada1(char **lsfiles, int cant) {
    int i;

    FILE * f;
    f = fopen("log.txt", "w");
    fprintf(f, "");
    fclose(f);

    clearScanner();

    //Hacerlo con cada archivo
    for (i = 0; i < cant; i++) {

        FILE * f;
        f = fopen("log.txt", "a");
        fprintf(f, "%s\n", lsfiles[i]);
        fclose(f);

        fp = fopen(lsfiles[i], "r");
        if (fp == NULL) {
            return -1; //panico si no se puede abrir el archivo
        }

        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;
        LineaActual = 0;
        PosicionLineaActual = 0;
        struct nodoArchivo *miArchivo;
        instarArchivoTDS(lsfiles[i], &tabla, &miArchivo);

        obtoken();
        if (!programaP1(miArchivo)) {
            fclose(fp);
            fp = NULL;
            return 0;
        };

        fclose(fp);
        fp = NULL;
    }
    return 1;
}

int pasada2(char **lsfiles, int cant) {
    printf("------------------------------------------------------\n");
    int i;
    clearScanner();
    FILE * f;
    f = fopen("log.txt", "w");
    fprintf(f, "");
    fclose(f);
    //Hacerlo con cada archivo
    for (i = 0; i < cant; i++) {
        FILE * f;
        f = fopen("log.txt", "a");
        fprintf(f, "%s\n", lsfiles[i]);
        fclose(f);
        fp = fopen(lsfiles[i], "r");
        if (fp == NULL) {
            return -1; //panico si no se puede abrir el archivo
        }

        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;
        LineaActual = 0;
        PosicionLineaActual = 0;
        struct nodoArchivo *valor = NULL;
        buscarArchivoTDS(&valor, &tabla, lsfiles[i]);
        // printf("Archivo %s \n", lsfiles[i]);
        if (valor == NULL) {
            //printf("EL ARCHIVO NO SE REGISTRO BIEN PANICO TOTAL LA SEGUNDA PASADA PROBABLEMETE FUE EJECUTADA DE FORMA INDEPENDIENTO O SE CAMBIARON LA LISTA DE ARCHIVOS");
            log_error(40);
        }
        obtoken();
        if(!programa(valor)){
            fclose(fp);
            fp = NULL;
            return 0;
        }

        fclose(fp);
        fp = NULL;
    }
    return 1;
}

void clearScanner() {
    primerError = 0;
    inicializar_espec();
    inicializarArbolPalabras();
    iniciarParamentros();
}

int programaP1(struct nodoArchivo *archivo) {
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
                    log_error(14); // falto punto y coma;
                    return 0;
                }
            } else {
                log_error(15); // No esta bien escrita la cadena;
                return 0;
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

                    //printf("Se inserto la clase %s con hash %d\n", claseActual->ident, claseActual->hash);
                    //Aunque no interese mucho que esta 100% bien 
                    // escrita si tiene que usar { cuerpo }
                    // para poder detectar todas las clases
                    // del mismo archivo
                    // Y evitar ambiguedades en el cuerpo
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        //printf("\tRevisando el cuerpo\n");
                        if (!cuerpoP1(claseActual)) {
                            return 0;
                        }
                        if (token == llaveF) {
                            // printf("\tClase bien escrita\n");
                            obtoken();
                        } else {
                            log_error(22); //falto llave de cierre
                            return 0;
                        }
                    } else {
                        log_error(21); //falta llave de apertura
                        return 0;
                    }
                } else {
                    log_error(18); //Falta indentificador
                    return 0;
                }
            } else {
                log_error(17); // no puso la palabra clave clase         
                return 0;
            }
        } while (token != -1);
    } else {
        log_error(16);
        return 0;
    }

    return 1;
}

int cuerpoP1(struct clase *clase) {
    // Se aumentara cada vez que exista una llaver de apertura
    // Se disminuira cuando se cierre
    int countLlaveI = 0;
    if (token == llaveF) {
        //printf("\tClase vacia no se avanzara un token\n");
        return 1;
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
                    // printf("\t\tRegistrando metodo %s\n", nombre);
                    struct metodo *metodo;
                    metodo = (struct metodo*) malloc(sizeof (struct metodo));
                    metodo->ident = (char *) malloc(sizeof (char)*strlen(nombre) + 1);
                    strcpy(metodo->ident, nombre);

                    insertarTDSMetodo(clase, metodo);

                    obtoken();
                    if (token == corcheteF) {
                        //metodo sin parametros
                        metodo->parametros = NULL;
                        //printf("\t\t\t sin parametros \n");
                        obtoken();
                        if (token == arregloTok || token == objetoTok || token == archivoTok
                                || token == caracterTok || token == cadenaTok || token == enteroTok
                                || token == realTok || token == byteTok || token == booleanoTok || token == ident) {
                            //printf("\t\t\t y con retorno %d\n", token);
                            metodo->tipoRetorno = token; //Cambiar esto por el tipo en mayusculas
                            metodo->esFuncion = TRUE;
                        } else {
                            if (token == llaveI) countLlaveI++;
                            if (token == llaveF) countLlaveI--;
                        }

                    } else {
                        //Tiene paramentros
                        //Preparamos el espacio para los paramentros
                        struct listaAtributo *parametros
                                = (struct listaAtributo *) malloc(sizeof (struct listaAtributo));
                        metodo->parametros = parametros;
                        metodo->parametros->sig = NULL;
                        int i = 0;
                        do {
                            struct atributo *parametroX = (struct atributo *) malloc(sizeof (struct atributo));
                            if (i == 1) {
                                obtoken();
                                //Es el 2 o mas
                                parametros->sig = (struct listaAtributo *) malloc(sizeof (struct listaAtributo));
                                parametros = parametros->sig;
                                parametros->sig = NULL;

                            }

                            parametros->atributo = parametroX;

                            //printf("\t\t\tSe detecto parametro de tipo ");
                            if (!tipoDP1(parametroX)) {
                                return 0;
                            }

                            if (token == por) {
                                // printf(" es por referencia ");
                                parametroX->esPorReferencia = TRUE;
                                obtoken();
                            }

                            if (token == ident) {
                                parametroX->ident = (char *) malloc(sizeof (char) * strlen(lex) + 1);
                                strcpy(parametroX->ident, lex);
                                obtoken();
                                i = 1;
                                //printf("Se inserta el parametro \n");
                            } else {
                                log_error(18); // Se esperaba un ident
                                return 0;
                                //printf("Error \n");
                            }
                        } while (token == coma);

                        if (token == corcheteF) {
                            obtoken();
                            if (token == arregloTok || token == objetoTok || token == archivoTok
                                    || token == caracterTok || token == cadenaTok || token == enteroTok
                                    || token == realTok || token == byteTok || token == booleanoTok || token == ident) {
                                // printf("\t\t\t y con retorno %d\n", token);
                                metodo->tipoRetorno = token;
                                metodo->esFuncion = -1;
                            } else {
                                if (token == llaveI) countLlaveI++;
                                if (token == llaveF) countLlaveI--;
                            }
                            //printf("\t\t\ty esta bien escrito\n");
                        } else {
                            log_error(31); // si esperaba corchete de cierre
                            return 0;
                        }
                    }


                }
            }

            //Es propiedad
            if (propiedad == 2 || propiedad == 1) {
                // printf("\t\tPropiedad de tipo");

                struct atributo *atributo = (struct atributo*) malloc(sizeof (struct atributo));
                atributo->alcanze = alcanze;
                //ES un propieda cuyo tipo es un objeto
                if (propiedad == 2) {
                    //En la version completa buscar el hashClase
                    // printf(" %s que es un objeto ", nombre);
                    atributo->esPrimitivo = 0;
                    atributo->tipo = OBJETO;

                } else {
                    if (!tipoDP1(atributo)) {
                        return 0;
                    }
                }
                //Para que verificar el tipo
                //Y ademas venga el inicio del registro
                //De tds


                if (token == ident) {
                    // printf("El nombre es %s ", lex);
                    atributo->ident = (char *) malloc(sizeof (char)* strlen(lex) + 1);
                    strcpy(atributo->ident, lex);
                    insertarTDSAtributo(clase, atributo);
                    obtoken();
                    do {
                        //viene otra priopiedad 
                        // del mismo tipo
                        if (token == coma) {
                            obtoken();
                            //Y se llama
                            if (token == ident) {
                                struct atributo *atributo2 = (struct atributo*) malloc(sizeof (struct atributo));
                                atributo2->esPrimitivo = atributo->esPrimitivo;
                                atributo2->tipo = atributo->tipo;
                                atributo2->ident = (char *) malloc(sizeof (char)* strlen(lex) + 1);
                                atributo2->tipoContenidoArreglo = atributo->tipoContenidoArreglo;
                                atributo2->alcanze = atributo->alcanze;
                                atributo2->valor = atributo->valor; //copiarValor(atributo2,atributo); //No son copia es el mismo
                                //tenemos que hacer

                                strcpy(atributo2->ident, lex);
                                //printf("\t\t luego esta %s ", lex);
                                insertarTDSAtributo(clase, atributo2);
                            } else {
                                log_error(18);
                                return 0;
                            }
                        }
                        if (token == puntoycoma) {
                            break;
                        }
                        obtoken();
                    } while (token != puntoycoma && token != -1);
                    if (token == -1) {
                        log_error(14);
                        return 0;
                    }

                } else {
                    log_error(18);
                    return 0;
                }
            }

        }
        obtoken();
        if (token == llaveI) countLlaveI++;
        if (token == llaveF) countLlaveI--;

    } while (countLlaveI >= 0 && token != -1);

    if (token == -1) {
        log_error(39);
        return 0;
    } else {
        return 1;
    }
}

int tipoDP1(struct atributo *atributo) {
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
            atributo->esPrimitivo = 0;
            //printf(" Arreglo que almacena de tipo <");
            obtoken();
            if (token == mnr) {
                struct atributo *valor = (struct atributo*) malloc(sizeof (struct atributo));
                obtoken();
                if (!tipoDP1(valor)) {
                    return 0;
                }

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
                    //printf(">. ");
                } else {
                    log_error(26); //se esperaba cierre
                    return 0;
                }
            } else {
                log_error(25);
                return 0;
            }
            break;
        default:
            // printf("Error en tipo \n");
            log_error(27); //NO es un tipo de dato valido
            return 0;
    }
    obtoken();

    return 1;
}

void copiarValor(struct atributo *dest, struct atributo *org) {
    switch (org->tipo) {
        case ENTERO:
            dest->valor = malloc(sizeof (int));
            //Para que solo copie lo del int 
            int *valorO = (int *) org->valor;
            int *valorD = (int *) org->valor;
            *valorD = *valorO;
            break;
        case REAL:
            dest->valor = malloc(sizeof (float));
            //Para que solo copie lo del int 
            float *valorOf = org->valor;
            float *valorDf = org->valor;
            *valorDf = *valorOf;
            break;
        case BYTE:
            dest->valor = malloc(sizeof (char));
            //Para que solo copie lo del int 
            char *valorOB = org->valor;
            char *valorDB = org->valor;
            *valorDB = *valorOB;
            break;
        case BOOLEANO:
            dest->valor = malloc(sizeof (int));
            //Para que solo copie lo del int 
            int *valorOBo = org->valor;
            int *valorDBo = org->valor;
            *valorDBo = *valorOBo;
            break;
        case CADENA:
            dest->valor = malloc(sizeof (char) * strlen(org->valor) + 1);
            //Para que solo copie lo del int 
            char **valorOC = org->valor;
            char **valorDC = org->valor;
            strcpy(*valorDC, *valorOC);
            break;
        case CARACTER:
            dest->valor = malloc(sizeof (char));
            //Para que solo copie lo del int 
            char *valorOCa = org->valor;
            char *valorDCa = org->valor;
            *valorDCa = *valorOCa;
            break;
        case ARREGLO:
            if (org->tipoContenidoArreglo != ARREGLO) {
                //Caso facil
            } else {
                //caso dificil me recursion 
            }
            break;
        default:
            //FALTA DEFINIR objeto y archivo
            break;
    }
}

//Programa necesita saber en el contexto de que archivo esta

int programa(struct nodoArchivo* miArchivo) {

    //Area de inclusion 
    do {
        if (token == incluirTok) {
            obtoken();
            if (token == datoCadena) {
                obtoken();
                //verificamos que lo que quiere incluir existe
                struct nodoArchivo *incluir = NULL;
                buscarArchivoTDS(&incluir, &tabla, valorCadena);
                //Verificamos pro que le la primera pasada solo se registor
                //pero no se sabia si existia o no
                if (incluir == NULL) {
                    // printf("El archivo a incluir no se reconoce\n");
                    log_error(13);
                    return 0;
                }
                if (token == puntoycoma) {
                    //printf("Incluir %s\n", valorCadena);
                    obtoken();
                } else {
                    log_error(14); //se esperaba ;
                    return 0;
                }
            } else {
                log_error(15); //se espera cadena
                return 0;
            }
        }
    } while (token == incluirTok);

    //Area de clase

    do {
        if (token == publicaTok || token == localTok) {
            int tokenAux = token;
            obtoken();
            if (token == claseTok) {
                int tokenAux2 = token;
                obtoken();
                if (token == ident) {
                    struct clase *yoClase = NULL;
                    obtenerClase(miArchivo, &yoClase, lex);
                    if (yoClase == NULL) {
                        //printf("Error clase no registrad en la primera pasada\n");
                        log_error(19);
                        return 0;
                    }
                    //printf("------------Soy la clase %s\n", yoClase->ident);

                    //verifica que la clase solo exita en este archivo
                    if (evitarRedefinicionClase(lex, miArchivo, &tabla) >= 1) {
                        // printf("Esta clase exite en mas de un lugar\n");
                        log_error(20); //la calse estaba definida
                        return 0;
                    }
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        if(!cuerpo(miArchivo, yoClase)){
                            return 0;
                        }
                        if (token == llaveF) {
                            printf("-------------Clase bien escrita-----------------\n");
                            obtoken();
                        } else {
                            log_error(22); //se esperaba llaver de cierre
                            return 0;
                        }
                    } else {
                        log_error(21); //se esperaba una llave de apertura
                        return 0;
                    }
                } else {
                    log_error(18); //se eperaba nombre de clase
                    return 0;
                }
            } else {
                log_error(17); //Se esperaba la declaracion de un clase
                return 0;
            }
        } else {
            log_error(16);
            return 0;
        }
    } while (token != -1);

    return 1;
}

int cuerpo(struct nodoArchivo* miArchivo, struct clase *clase) {
    if (token == llaveF) {
        printf("\t No tenia nada");
        return 1;
    }
    do {
        if (token == publicaTok || token == privadoTok) {
            printf("\tDeclaracion de atributo o metodo\n");
            obtoken();
            //Se sabe con facilidad que es un atributo
            if (token == arregloTok || token == objetoTok || token == archivoTok
                    || token == caracterTok || token == cadenaTok || token == enteroTok
                    || token == realTok || token == byteTok || token == booleanoTok) {
                //Se llama realmente porque puede ser un arreglo 
                int valor = tipo(miArchivo, clase);
                if (token == ident) {
                    struct atributo *atr = NULL;
                    buscarAtributo(&atr, clase, lex);
                    if (atr != NULL) {
                        printf("\t\tSe Declaro con exito %s\n", atr->ident);
                        obtoken();
                        if (token == asignacion) {
                            printf("\t\tSe le esta asignando un valor a %s \n", atr->ident);
                            obtoken();
                            if(!expresion()){
                                return 0;
                            }
                        }

                        do {
                            if (token == coma) {
                                obtoken();
                                if (token == ident) {
                                    struct atributo *atrAux = NULL;
                                    buscarAtributo(&atrAux, clase, lex);
                                    if (atrAux != NULL) {
                                        printf("\t\tSe Declaro con exito %s\n", atrAux->ident);
                                        obtoken();
                                        if (token == asignacion) {
                                            printf("\t\tSe le esta asignando un valor a %s \n", atrAux->ident);
                                            obtoken();
                                            if(!expresion()){
                                                return 0;
                                            }
                                        }
                                    } else {
                                        printf("\t\t Se esta redefiniedo el atributo %s\n", lex);
                                        log_error(23); //este atributo no se detecto en la primera pasada 
                                        return 0;
                                    }
                                } else {
                                    log_error(18); //se esperaba un idnet de atributo
                                    return 0;
                                }
                            }
                        } while (token != puntoycoma);

                        if (token == puntoycoma) {
                            obtoken();
                            printf("----------Linea de declaracion bien escrita \n------");
                        } else {
                            printf("Falta ; \n");
                            log_error(14);
                            return 0;
                        }
                    } else {
                        printf("\t\t Se esta redefiniedo el atributo %s\n", lex);
                        log_error(23); //este atributo no se detecto en la primera pasada
                        return 0;
                    }
                } else {
                    log_error(18); //atributo mal escrito
                    return 0;
                }
            } else {
                if (tipo(miArchivo, clase) == 1) {
                    if (token == ident) {
                        struct atributo *atr = NULL;
                        buscarAtributo(&atr, clase, lex);
                        if (atr != NULL) {
                            printf("\t\tSe Declaro con exito %s (objeto)\n", atr->ident);
                            obtoken();
                            if (token == asignacion) {
                                obtoken();
                                if(!expresion()){
                                    return 0;
                                }
                            }

                            do {
                                if (token == coma) {
                                    obtoken();
                                    if (token == ident) {
                                        struct atributo *atrAux = NULL;
                                        buscarAtributo(&atrAux, clase, lex);
                                        if (atrAux != NULL) {
                                            printf("\t\tSe Declaro con exito %s (objeto)\n", atrAux->ident);
                                            obtoken();
                                            if (token == asignacion) {
                                                obtoken();
                                                if(!expresion()){
                                                    return 0;
                                                }
                                            }
                                        } else {
                                            printf("\t\t Se esta redefiniedo el atributo %s (objeto)\n", lex);
                                            log_error(23); //este atributo no se detecto en la primera pasada  
                                            return 0;
                                        }
                                    } else {
                                        log_error(18); //se esperaba un idnet de atributo
                                        return 0;
                                    }
                                }
                            } while (token != puntoycoma);

                            if (token == puntoycoma) {
                                obtoken();
                                printf("----------Linea de declaracion bien escrita (Objeto) \n------------------");
                            } else {
                                printf("Falta ;\n");
                                log_error(14); //falta ;
                                return 0;
                            }
                        } else {
                            printf("\t\tSe esta redefiniedo el atributo %s\n", lex);
                            log_error(23); //este atributo no se detecto en la primera pasada
                            return 0;
                        }

                    } else {
                        log_error(18); //se esperava un ident para el atriburo
                        return 0;
                    }
                } else {
                    printf("\t\t---------Definicion de metodo-------------------- \n");
                    if (!metodo(miArchivo, clase)) {
                        return 0;
                    }
                    printf("\t\t---------Definicio de metodo completa---------------- \n");
                }

            }
        } else {
            log_error(16); //se eperaba modificador de acceso
            return 0;
        }

        if (token == llaveF) {
            return 1;
        }

    } while (token != -1);

    return 1;
}

int tipo(struct nodoArchivo *miArchivo, struct clase *clase) {
    switch (token) {

        case ident:
            //printf("A preguntar por tipo obejto\n");
            if (puedoUsarEsteTipo(lex, miArchivo, clase, &tabla) == 1) {
                obtoken();
                //  printf("Tipo Adecuado\n");
                return 1;
            } else {
                return 0;
            }
            break;
        case objetoTok:
        case archivoTok:
        case caracterTok:
        case cadenaTok:
        case enteroTok:
        case realTok:
        case byteTok:
        case booleanoTok:
            printf("ES de tipo primitivo");
            obtoken();
            return 1;
            break;
        case arregloTok:
            printf("Se declara un arreglo de tipo < ");
            obtoken();
            if (token == mnr) {
                obtoken();
                if(!tipo(miArchivo, clase)){
                    return 0;
                }
                if (token == myr) {
                    obtoken();
                    printf(" >\n");
                    return 1;
                } else {
                    log_error(26); //se espera simbolo de mayor
                    return 0;
                }
            } else {
                log_error(25); //se espera simbolo de menor
                return 0;
            }

            break;
        case llaveI: //especial para retorno

            return 2;
        case llaveF:
            return 3;
        default:

            log_error(27); //NO es un tipo de dato valido
            return 0;
    }
    return 0;
}

int metodo(struct nodoArchivo* miArchivo, struct clase *clase) {
    if (token == ident) {
        struct metodo *metodo = NULL;
        buscarMetodo(&metodo, clase, lex);
        if (metodo != NULL) {
            obtoken();
            if (token == corcheteI) {
                obtoken();
                if (token == corcheteF) {
                    obtoken();
                    int rs = tipo(miArchivo, clase);
                    if (rs == 1) {
                        //tiene rotorno verificar si es el mismo
                        if (metodo->tipoRetorno != token) {
                            printf("Tipo de retono inesperado");
                            log_error(30);
                            return 0;
                        }
                    }
                    printf("Metodo %s bien declarado \n", metodo->ident);
                    if (rs == 0) {
                        obtoken();
                    }
                    if (!bloque(miArchivo, clase, metodo)) {
                        return 0;
                    }
                } else {
                    if (!parametro(miArchivo, clase, metodo)) {
                        return 0;
                    }
                    while (token == coma) {
                        obtoken();
                        if (!parametro(miArchivo, clase, metodo)) {
                            return 0;
                        }
                    }
                    if (token == corcheteF) {
                        obtoken();
                        int rs = tipo(miArchivo, clase);
                        if (rs == 1) {
                            //tiene rotorno verificar si es el mismo
                            if (metodo->tipoRetorno != token) {
                                printf("Tipo de retono inesperado");
                                log_error(30);
                                return 0;
                            }
                        }
                        //printf("Metodo %s con parametros bien declarado \n", metodo->ident);

                        if (rs == 0) {
                            obtoken();
                        }

                        if (!bloque(miArchivo, clase, metodo)) {
                            return 0;
                        }
                    } else {
                        log_error(31); // se esperaba cierre de corchete
                        return 0;
                    }

                }
            } else {
                log_error(29);
                return 0;
            }
        } else {
            printf("Se esta intentando redefinir el metodo %s\n", lex);
            log_error(28); // declaracion mal escrita
            return 0;
        }
    } else {
        log_error(18);
        return 0;
    }
    return 1;
}

int parametro(struct nodoArchivo *miArchivo, struct clase *clase, struct metodo *metodo) {
    if (tipo(miArchivo, clase) == 1) {

        if (token == por) {
            obtoken();
        }

        if (token == ident) {
            printf("Parametro %s detectado\n", lex);
            if (1 == sePuedeUsarComoParametro(clase, metodo, lex)) {
                obtoken();
            } else {
                log_error(41); //se esta redefiniedo parametro
                return 0;
            }
        } else {
            log_error(18); // Se esperaba un ident
            return 0;
        }
    }
    return 1;
}

int bloque(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    printf("Entrando a bloque\n");
    if (token == llaveI) {
        obtoken();
        int seguir = 1;
        do {

            do {
                int rs;
                char *claseName = (char *) malloc(sizeof (char)* strlen(lex) + 1);
                strcpy(claseName, lex);
                printf("Lex leido %s\n");
                int tokenTipo = token;
                rs = tipo(miArchivo, clase);


                if (rs == 0) {
                    seguir = 0;
                    break;
                }
                if (rs == 3) {
                    seguir = -1;
                    break;
                }
                if (rs == 1) {
                    if (token == ident) {
                        if (evitarRedefinicionBloque(clase, metodo, lex) <= 1) {
                            printf("\n\n\nSe detecto declaracion");
                            struct atributo *atr = (struct atributo *) malloc(sizeof (struct atributo));
                            atr->tipo = tokenTipo;
                            atr->esPrimitivo = 1;
                            if (tokenTipo == ident) {
                                atr->tipo = OBJETO;
                                atr->esPrimitivo = 0;
                                struct clase *clasePapa = NULL;
                                buscarClaseTDS(&clasePapa, &tabla, claseName);
                                if (clasePapa == NULL) {
                                    printf("NO se encontro una clase\n");
                                    log_error(19);
                                    return 0;
                                } else {
                                    atr->hash = clasePapa->hash;
                                }
                            }
                            atr->ident = (char *) malloc(sizeof (char) * strlen(lex) + 1);
                            strcpy(atr->ident, lex);

                            //SI es de tipo objeto ir a buscar el hash de la clase
                            //Y asignarle un hasd de instancia
                            insertarDefinicionLocal(metodo, atr);
                            obtoken();
                            if (token == asignacion) {
                                obtoken();
                                if(!expresion()){
                                    return 0;
                                }
                            }
                            while (token == coma) {
                                obtoken();
                                if (token == ident) {
                                    if (evitarRedefinicionBloque(clase, metodo, lex) <= 1) {
                                        struct atributo *atr = (struct atributo *) malloc(sizeof (struct atributo));
                                        atr->tipo = tokenTipo;
                                        atr->esPrimitivo = 1;
                                        if (tokenTipo == ident) {
                                            atr->tipo = OBJETO;
                                            atr->esPrimitivo = 0;
                                        }
                                        atr->ident = (char *) malloc(sizeof (char) * strlen(lex) + 1);
                                        strcpy(atr->ident, lex);

                                        //SI es de tipo objeto ir a buscar el hash de la clase
                                        //Y asignarle un hasd de instancia
                                        insertarDefinicionLocal(metodo, atr);
                                        printf(" Se detecto declaracion misma linea");
                                        obtoken();
                                        if (token == asignacion) {
                                            obtoken();
                                            if(!expresion()){
                                                return 0;
                                            }
                                        }
                                        printf("\n");
                                    } else {
                                        printf("Esta intentando redefinir un parametro o atributo\n");
                                        log_error(41);
                                        return 0;
                                    }
                                } else {
                                    log_error(18);
                                    return 0;
                                }
                            }
                        } else {
                            printf("Esta intentando redefinir un parametro o atributo\n");
                            log_error(41);
                            return 0;
                        }
                    } else {
                        log_error(18);
                        return 0;
                    }
                }
            } while (token != puntoycoma);
            if (seguir == -1 || seguir == 0) {
                break;
            }
            obtoken();
        } while (seguir == 1);
        if(!instruccion(miArchivo, clase, metodo)){
            return 0;
        }
        if (token == llaveF) {
            printf("Metodo OK\n");
            obtoken();
        } else {
            log_error(22); //se esperaba una llave de cierre
            return 0;
        }
    } else {
        log_error(21); //se esperaba una llave de apertura
        return 0;
    }
    return 1;
}

int instruccion(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    int es0 = 0;
    struct atributo *atr;
    switch (token) {
        case ident:

            es0 = esObjeto(clase, metodo, lex, &atr);
            obtoken();

            //Es una llamada a metodo de una clase
            if (token == punto) {
                obtoken();
                if (token == abrirTok || token == leerLineaTok
                        || token == volcadoTok || token == cerrarTok) {
                    if(!funcion_archivo()){
                        return 0;
                    }
                    if (token == puntoycoma) {
                        obtoken();
                    } else {
                        log_error(14); //se eperaba punto y coma
                        return 0;
                    }
                } else {
                    struct clase* actual;
                    if (es0 == 1) {
                        //obtenerClase(miArchivo, struct clase ** clase, char *lex)
                        if(!llamada_metodo()){
                            return 0;
                        }
                        if (token == puntoycoma) {
                            obtoken();
                            printf("EStaba escrita bien la llamada a metodo\n");
                        } else {
                            log_error(14); //se esperaba un punto y coma
                            return 0;
                        }
                    } else {
                        printf("Se esta inentando llamar a metodo o propiedad a un varible primtiva\n");
                        log_error(55); //se esperaba un punto y coma
                        return 0;
                    }
                }
            } else {
                //Es una asignación
                if (token == asignacion) {
                    printf("Se esta haciendo una asignacion\n");
                    obtoken();
                    if(!expresion()){
                        return 0;
                    }
                    if (token == puntoycoma) {
                        obtoken();
                        printf("EStaba escrita bien la asgnacion\n");
                    }
                } else {
                    log_error(32); //esperaba una llaamda a metodo o un asignacion;
                    return 0;
                }
            }
            break;
        case romperTok:
            if(!instruccion_romper()){
                return 0;
            }
            printf("SE encontro romper\n");
            break;
        case siTok:
            if(!instruccion_si(miArchivo, clase, metodo)){
                return 0;
            }
            //printf("SE encontro si\n");
            break;
        case probarTok:
            if(!instruccion_probar(miArchivo, clase, metodo)){
                return 0;
            }
            break;
        case mientrasTok:
            if(!instruccion_mientras(miArchivo, clase, metodo)){
                return 0;
            }
            break;
        case paraTok:
            if(!instruccion_para()){
                return 0;
            }
            break;
        case hacerTok:
            if(!instruccion_hacer(miArchivo, clase, metodo)){
                return 0;
            }
            break;
        case paracadaTok:
            if(!instruccion_paraCada(miArchivo, clase, metodo)){
                return 0;
            }
            break;
        case sistemaTok:
            if(!instruccion_es()){
                return 0;
            }
            break;
        case retornarTok:
            obtoken();
            if(!expresion()){
                return 0;
            }
            if (token == puntoycoma) {
                printf("REtornar bien escrito");
                obtoken();
            } else {
                log_error(14); //se esperaba punto y coma
                return 0;
            }
            break;
        case llaveF:
            printf("NO hAY INSTRUCCION"); // no se adelante por que encontro el siguiente
            return 1;
            break;
        default:
            log_error(33); //carater inseperado
            return 0;
            break;

    }
    if(!instruccion(miArchivo, clase, metodo)){
        return 0;
    }
    return 1;
}

int instruccion_si(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == siTok) {
        printf("se encontro sitok\n");
        obtoken();
        if (token == corcheteI) {
            printf("se encontro corcheteI\n");
            obtoken();
            printf("a entrar a expresion numerica\n");
            if(!expresion()){
                return 0;
            }
            if (token == coma) {
                printf("\nEncontre una coma\n");
                obtoken();

                if(!asignacionf()){
                    return 0;
                }
                printf("\nRegrese de asignacion en si corto\n");
                if (token == coma) {
                    printf("\nEncontre segunda coma\n");
                    obtoken();
                    if(!asignacionf()){
                        return 0;
                    }
                    printf("\nRegrese de 2 asignacion en si corto\n");
                    if (token == corcheteF) {
                        printf("\nCorchet F de si corto\n");
                        obtoken();
                        if (token == puntoycoma) {
                            obtoken();
                            printf("Si corto bien\n");
                        } else {
                            log_error(14); //faltaba punto y coma
                            return 0;
                        }
                    } else {
                        printf("Ultimo lex leido %s \n ", lex);
                        log_error(31); //faltaba corchete de cierre
                        return 0;
                    }
                } else {
                    log_error(36); //se esperaba una coma
                    return 0;
                }
            } else {
                if (token == corcheteF) {
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        if(!instruccion(miArchivo, clase, metodo)){
                            return 0;
                        }
                        if (token == llaveF) {
                            obtoken();
                            if (token == sinoTok) {
                                printf("\n \t Sino\n");
                                obtoken();
                                if (token == llaveI) {
                                    printf("Sino llaveI OK\n");
                                    obtoken();
                                    if(!instruccion(miArchivo, clase, metodo)){
                                        return 0;
                                    }
                                    if (token == llaveF) {
                                        obtoken();
                                        printf("\nSino bien escrito\n");
                                    } else {
                                        log_error(22); //se esperaba llave de cierre
                                        return 0;
                                    }
                                } else {
                                    printf("falta llave");
                                    log_error(21); //se esperaba llave de apertura
                                    return 0;
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            log_error(22); //faltaba llave de cierre
                            return 0;
                        }

                    } else {
                        log_error(21); //se esperaba llave de apertura
                        return 0;
                    }
                } else {
                    log_error(35); //se esperaba una coma o corchete de cierre
                    return 0;
                }
            }
        } else {
            log_error(29); //se esperaba una corchete de apertura
            return 0;
        }
    } else {
        log_error(34); // se esperaba el token si
        return 0;
        printf("Se epserab si tok");
    }
    return 1;
}

int asignacionf() {
    if (token == ident) {
        printf("\nEncontres un ident\n");
        obtoken();
        if (token == asignacion) {
            printf("\nEncontre un  = \n");
            obtoken();
            if(!expresion()){
                return 0;
            }
        } else {
            log_error(32);
            return 0;
        }
    } else {
        log_error(18); //se esperaba un identificador
        return 0;
    }
    return 1;
}

int funcion_cadena() {
    if (token == concatenarTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if(!valor_cadena()){
                return 0;
            }
            if (token == coma) {
                obtoken();
                if(!valor_cadena()){
                    return 0;
                }
                if (token == corcheteF) {
                    obtoken();
                    printf("funcion concatenar completa\n");
                } else {
                    log_error(31);
                    return 0;
                }
            } else {
                log_error(36); //se esperaba una coma
                return 0;
            }
        } else {
            log_error(29); //faltaba corchete de apertura
            return 0;
        }
    } else {
        log_error(37);
        return 0;
    }
    return 1;
}

int funcion_archivo() {
    if (token == abrirTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if(!valor_cadena()){
                return 0;
            }
            if (token == corcheteF) {
                obtoken();
                printf("funcion archivo bien escrita camino 1");
            } else {
                log_error(31); //se esperaba corchete de cierre
                return 0;
            }
        } else {
            log_error(29); //se esperaba corchete de apertura
            return 0;
        }
    } else {
        switch (token) {
            case leerLineaTok:
            case volcadoTok:
            case cerrarTok:
                obtoken();
                if (token == corcheteI) {
                    obtoken();
                    if (token == corcheteF) {
                        obtoken();
                        printf("funcion archivo bien escrita camino 2");
                    } else {
                        log_error(31); //se esperaba corchete de cierre
                        return 0;
                    }
                } else {
                    log_error(29); //se esperaba corchete de abierto
                    return 0;
                }
                break;
            default:
                log_error(38); //NO deberia llegar aqui Se esperaba la llamada a funciones de archivos
                return 0;
        }
    }
    return 1;
}

int valor_cadena() {
    switch (token) {
        case datoCadena:
            printf("Hay una cadena\n");
            obtoken();
            break;
        case ident:
            obtoken();
            if (token == punto) {
                obtoken();
                if(!llamada_metodo()){ // o indetificador??
                    return 0;
                }
            }
            break;
        case concatenarTok:
            if(!funcion_cadena()){
                return 0;
            }
            break;
        default: log_error(15); //se esperaba un valor cadena
            return 0;
    }

    return 0;
}

int valor_caracter() {
    switch (token) {
        case datoCaracter:
            obtoken();
            break;
        case ident:
            obtoken();
            if (token == punto) {
                obtoken();
                if(!llamada_metodo()){ // o indetificador??
                    return 0;
                }
            }
            break;
        default: log_error(15);
            return 0;
    }
    return 0;
}

int llamada_metodo() {
    if (token == ident) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            //Vefiricar la cantidad de parametro de este metodo
            if (token == corcheteF) {
                printf("BUena llamada a metodo\n");
                obtoken();
            } else {
                if (token == referencia) {
                    obtoken();
                    if (token == ident) {
                        obtoken();
                        printf("BUena llamada a metodo 2\n");
                    } else {
                        log_error(18); //se espera un ident
                        return 0;
                    }
                } else {
                    if(!expresion()){
                        return 0;
                    }
                }
                int i = 2;
                while (token == coma) {
                    printf("Parametro %d\n", i);
                    obtoken();
                    if (token == referencia) {
                        obtoken();
                        if (token == ident) {
                            obtoken();
                        } else {
                            log_error(18); //se espera un ident
                            return 0;
                        }
                    } else {
                        if(!expresion()){
                            return 0;
                        }
                    }
                    i++;
                }
                if (token == corcheteF) {
                    printf("BUena llamada a metodo\n");
                    obtoken();
                } else {
                    log_error(31); //Falta corchete F
                    return 0;
                }
            }

        } else {
            log_error(29); //falta corchete de apertura
            return 0;
        }
    } else {
        if(!funcion_arreglo()){
            return 0;
        }
    }
    return 1;
}

int expresion() {
    if (token == verdaderoTok || token == falsoTok) {
        printf("Era un booleano \n");
        obtoken();
        return 1;
    }
    if (token == datoCadena) {
        printf("se encontro en la expresión una cadena\n");
        obtoken();
        return 1;
    }
    if (token == concatenarTok) {
        if(!funcion_cadena()){
            return 0;
        }
        printf("se encontro una funcion concatenar\n");
        return 1;
    }
    if (token == datoCaracter) {
        printf("Encontramos un caracter\n");
        obtoken();
        return 1;
    }
    if(!expresion_numerica()){
        return 0;
    }
}

int expresion_numerica() {

    if(!expresion_conjuncion()){
        return 0;
    }
    while (token == otok) {
        obtoken();
        printf("En expresion numerica con %s \n", lex);
        if(!expresion_conjuncion()){
            return 0;
        }
    }

}

int expresion_conjuncion() {

    if(!expresion_relacional()){
        return 0;
    }
    while (token == ytok) {
        obtoken();
        printf("En expresion conjuncion con %s\n", lex);
        if(!expresion_relacional()){
            return 0;
        }
    }

}

int expresion_relacional() {

    if (token == negacion) {
        printf("se encontro negación\n");
        obtoken();
    }

    if(!expresion_aritmetrica()){
        return 0;
    }
    if (token == mnr || token == myr || token == mai || token == mei || token == igl || token == nig) {
        obtoken();
        printf("Exapresion relacional con %s\n", lex);
        if(!expresion_aritmetrica()){
            return 0;
        }
    }

}

int expresion_aritmetrica() {
    if(!termino()){
        return 0;
    }
    if (token == mas || token == menos) {
        do {
            obtoken();
            printf("Exapresion aritmetica con %s\n", lex);
            if(!termino()){
                return 0;
            }
        } while (token == mas || token == menos);
    }

}

int termino() {
    if(!factor()){
        return 0;
    }
    if (token == por || token == barra) {
        do {
            obtoken();
            if(!factor()){
                return 0;
            }
        } while (token == por || token == barra);
    }
    //printf("\nTerminio termino");
}

int factor() {
    while (token == mas || token == menos) {
        obtoken();
    }

    if (token == numeroEntero || token == numeroReal) {
        obtoken();
        //printf("\n\tEncontre un numero\n");
        return 1;
    }
    if (token == parentI) {
        obtoken();
        if(!expresion_numerica()){
            return 0;
        }
        if (token == parentF) {
            obtoken();
            //printf("Se encontro una expresion numerica con parentesis\n");
            return 1;
        } else {
            log_error(42); //se esperaba un token de cierre
            return 0;
        }
    }

    if (token == sistemaTok) {
        obtoken();
        if(!funcion_num_numcad()){
            return 0;
        }
        return 1;
    }
    if (token == ident) {
        obtoken();
        printf("se encontro una variable en factor\n");
        if (token != punto) {
            return 1;
        } else {
            //falta llamada a metodo
        }
        //obtoken();
        //FALTA LLAMADA A METODO O IDENT (TDS)
    }
    log_error(43); //se esperaba un factor
    return 0;

}

int funcion_num_numcad() {
    if (token == sistemaTok) {
        obtoken();
        if (token == punto) {
            obtoken();
            switch (token) {
                case parteEnteraTok:
                case esParTok:
                case decimalBinTok:
                case absolutoTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!expresion_numerica()){
                            return 0;
                        }
                        if (token == corcheteF) {
                            obtoken();
                        } else {
                            log_error(31); //Se esperaba corchete de cierre
                            return 0;
                        }
                    } else {
                        log_error(29); //Se esperaba corchet apertura
                        return 0;
                    }
                    break;
                case mayorTok:
                case menorTok:
                case potenciaTok:
                case moduloTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!expresion_numerica()){
                            return 0;
                        }
                        if (token == coma) {
                            obtoken();
                            if(!expresion_numerica()){
                                return 0;
                            }
                            if (token == corcheteF) {
                                obtoken();
                            } else {
                                log_error(31); //Se esperaba corchete de cierre
                                return 0;
                            }
                        } else {
                            log_error(36); //falto coma
                            return 0;
                        }
                    } else {
                        log_error(29); //Se esperaba corchet apertura
                        return 0;
                    }
                    break;
                case longitudCadenaTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!valor_cadena()){
                            return 0;
                        }
                        if (token == corcheteF) {
                            obtoken();
                        } else {
                            log_error(31); //Se esperaba corchete de cierre
                            return 0;
                        }
                    } else {
                        log_error(29); //Se esperaba corchet apertura
                        return 0;
                    }
                    break;
                case compararTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!valor_cadena()){
                            return 0;
                        }
                        if (token == coma) {
                            obtoken();
                            if(!valor_cadena()){
                                return 0;
                            }
                            if (token == corcheteF) {
                                obtoken();
                            } else {
                                log_error(31); //Se esperaba corchete de cierre
                                return 0;
                            }
                        } else {
                            log_error(36); //falto coma
                            return 0;
                        }
                    } else {
                        log_error(29); //Se esperaba corchet apertura
                        return 0;
                    }
                    break;
                default:
                    log_error(44); // se esperaba llamada a sistema;
                    return 0;
                    break;
            }
        } else {
            log_error(45); // se esperaba un punto
            return 0;
        }
    } else {
        log_error(46); //Se esperaba palabra reservada sistema
        return 0;
    }
    return 1;
}

int instruccion_mientras(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == mientrasTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if(!expresion()){
                return 0;
            }
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    if(!instruccion(miArchivo, clase, metodo)){
                        return 0;
                    }
                    if (token == llaveF)
                        obtoken();
                    else {
                        log_error(22); //no esta }
                        return 0;
                    }
                } else {
                    log_error(21); //no esta {
                    return 0;
                }
            } else {
                log_error(31); //no esta el ]
                return 0;
            }
        } else {
            log_error(29); //no esta el [
            return 0;
        }
    } else {
        log_error(47); // no es el token mientras
        return 0;
    }
    return 1;
}

int instruccion_romper() {
    if (token == romperTok) {
        obtoken();
        if (token == puntoycoma)
            obtoken();
        else {
            log_error(14); //no esta el ;
            return 0;
        }
    } else {
        log_error(48); // no esta el token romper
        return 0;
    }
    return 1;
}

int funcion_arreglo() {
    switch (token) {
        case agregarTok:
        case obtenerTok:
        case quitarTok: obtoken();
            if (token == corcheteI) {
                obtoken();
                if(!expresion()){
                    return 0;
                }
                if (token == corcheteF) {
                    obtoken();
                    printf("funcion arreglo ok\n");
                    return 1;
                } else {
                    log_error(31); //se esperaba corchete de cierre
                    return 0;
                }
            } else {
                log_error(29); //se esperaba corchete de apertura
                return 0;
            }
            break;
        case cuantosTok:
            obtoken();
            if (token == corcheteI) {
                obtoken();
                if (token == corcheteF) {
                    obtoken();
                    //printf("funcion cuantos ok\n");
                    return 1;
                } else {
                    log_error(31); //se esperaba corchete de cierre
                    return 0;
                }
            } else {
                log_error(29); //se esperaba corchete de apertura
                return 0;
            }
            break;
        default: log_error(49); //no es una funcion arreglo
                return 0;
    }
    return 1;
}

int instruccion_es() {
    if (token == sistemaTok) {
        obtoken();
        if (token == punto) {
            obtoken();
            switch (token) {
                case obtenerEnteroTok:
                case obtenerRealTok:
                case obtenerCadenaTok:
                case obtenerCaracterTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if (token == ident) {
                            obtoken();
                            if (token == corcheteF) {
                                obtoken();
                                if (token == puntoycoma) {
                                    obtoken();
                                } else {
                                    log_error(14); //Se esperaba punto y coma
                                    return 0;
                                }
                            } else {
                                log_error(31); // se esperaba corchete de cierre
                                return 0;
                            }
                        } else {
                            log_error(18); //se esperaba indentificador
                            return 0;
                        }
                    } else {
                        log_error(29); // se esperaba corchete de apertura
                        return 0;
                    }
                    break;
                case mostrarTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!expresion()){
                            return 0;
                        }
                        if (token == coma) {
                            obtoken();
                            if(!expresion()){
                                return 0;
                            }
                        }
                        if (token == corcheteF) {
                            obtoken();
                            if (token == puntoycoma) {
                                obtoken();
                            } else {
                                log_error(14); //Se esperaba punto y coma
                                return 0;
                            }
                        } else {
                            log_error(2); // se esperaba corchete de cierre
                            return 0;
                        }
                    } else {
                        log_error(3); // se esperaba corchet ede apartura
                        return 0;
                    }
                    break;
                default:
                    log_error(4); //se esperaba un token de ontener o mostrar
                    return 0;
            }
        } else {
            log_error(5); // se esperaba un punto
            return 0;
        }
    } else {
        log_error(6); //se esperaba el token sistema
        return 0;
    }
    return 1;
}

int instruccion_probar(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == probarTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if(!expresion()){
                return 0;
            }
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    if (token == casoTok) {
                        while (token == casoTok) {
                            obtoken();
                            if(!expresion()){
                                return 0;
                            }
                            if (token == dosPuntos) {
                                obtoken();
                                if(!instruccion(miArchivo, clase, metodo)){
                                    return 0;
                                }
                            } else {
                                log_error(50); //faltan :
                                return 0;
                            }
                        }
                        if (token == defectoTok) {
                            obtoken();
                            if (token == dosPuntos) {
                                obtoken();
                                if(!instruccion(miArchivo, clase, metodo)){
                                    return 0;
                                }
                            }else{
                                log_error(50); //falta :
                                return 0;
                            }
                        }
                        if (token == llaveF) {
                            obtoken();
                        } else{
                            log_error(22); //falta }
                            return 0;
                        }
                    } else{
                        log_error(51); //falta caso
                        return 0;
                    }
                } else{
                    log_error(21); //falta {
                    return 0;
                }
            } else{
                log_error(31); //falta ]
                return 0;
            }
        } else{
            log_error(29); //falta [
            return 0;
        }
    } else{
        log_error(52); //falta probar
        return 0;
    }
    return 1;
}

int instruccion_para(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == paraTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if (token == ident) {
                //verificar que exista en este metodo o clase
                obtoken();
                if (token == asignacion) {
                    obtoken();
                    if(!expresion_numerica()){
                        return 0;
                    }
                    if (token == coma) {
                        obtoken();
                        if(!expresion_numerica()){
                            return 0;
                        }
                        if (token == coma) {
                            obtoken();
                            if(!expresion_numerica()){
                                return 0;
                            }
                            if (token == corcheteF) {
                                obtoken();
                                if (token == puntoycoma) {
                                    obtoken();
                                } else {
                                    if (token == llaveI) {
                                        obtoken();
                                        if(!instruccion(miArchivo, clase, metodo)){
                                            return 0;
                                        }
                                        if (token == llaveF) {
                                            obtoken();
                                        } else {
                                            log_error(22); //falta }
                                            return 0;
                                        }
                                    } else {
                                        log_error(14); // falta el ; 
                                        return 0;
                                    }
                                }
                            } else {
                                log_error(31); //falta ]
                                return 0;
                            }
                        } else {
                            log_error(36); //falta ,
                            return 0;
                        }
                    } else {
                        log_error(36); //falta ,
                        return 0;
                    }
                } else {
                    log_error(32); //falta =
                    return 0;
                }
            } else {
                log_error(18); //no es un identificador
                return 0;
            }
        } else {
            log_error(29); //no es [
            return 0;
        }
    } else {
        log_error(53); //no es la palabra para
        return 0;
    }
    return 1;
}

int instruccion_hacer(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == hacerTok) {
        obtoken();
        if (token == llaveI) {
            obtoken();
            if(!instruccion(miArchivo, clase, metodo)){
                return 0;
            }
            if (token == llaveF) {
                obtoken();
                if (token == mientrasTok) {
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        if(!expresion_numerica()){
                            return 0;
                        }
                        if (token == corcheteF) {
                            obtoken();
                            if (token == puntoycoma)
                                obtoken();
                            else{
                                log_error(14); //falta ;
                                return 0;
                            }
                        } else{
                            log_error(31); //falta ]
                            return 0;
                        }
                    } else{
                        log_error(29); //falta [
                        return 0;
                    }
                } else{
                    log_error(47); //falta token mientras
                    return 0;
                }
            } else{
                log_error(22); //falta }
                return 0;
            }
        } else{
            log_error(21); //falta {
            return 0;
        }
    }
    return 1;
}

int instruccion_paraCada(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == paracadaTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if(!tipo(miArchivo, clase)){
                return 0;
            }
            if (token == ident) {
                //verificar si exites
                obtoken();
                if (token == coma) {
                    obtoken();
                    if (token == ident) {
                        //verificar si existe 
                        obtoken();
                        if (token == corcheteF) {
                            obtoken();
                            if (token == llaveI) {
                                obtoken();
                                if(!instruccion(miArchivo, clase, metodo)){
                                    return 0;
                                }
                                if (token == llaveF) {
                                    obtoken();
                                }else{
                                    log_error(22); //falta }
                                    return 0;
                                }
                            } else{
                                log_error(21); //falta {
                                return 0;
                            }
                        } else{
                            log_error(31); //falta ]
                            return 0;
                        }
                    } else{
                        log_error(18); //no es un identificador
                        return 0;
                    }
                } else{
                    log_error(36); //falta ,
                    return 0;
                }
            } else{
                log_error(18); // no es un identificador
                return 0;
            }
        } else{
            log_error(29); // no esta el [
            return 0;
        }
    } else{
        log_error(54); //no esta token paracada
        return 0;
    }
    return 1;
}
