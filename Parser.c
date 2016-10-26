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
void programaP1(struct nodoArchivo *archivo);
void cuerpoP1(struct clase *clase);
void tipoDP1(struct atributo *atributo);
void copiarValor(struct atributo *dest, struct atributo *org);
void programa(struct nodoArchivo *);
void cuerpo(struct nodoArchivo* miArchivo, struct clase *clase);
int tipo(struct nodoArchivo *miArchivo, struct clase *clase);
void metodo(struct nodoArchivo* miArchivo, struct clase *clase);
void parametro(struct nodoArchivo *miArchivo, struct clase *clase, struct metodo*);
void bloque(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void instruccion(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void instruccion_romper();
void instruccion_si(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void asignacionf();
void expresion();
void funcion_cadena();
void instruccion_probar();
void instruccion_mientras(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void instruccion_para();
void instruccion_hacer();
void instruccion_paraCada();
void instruccion_es();
void expresion_numerica();
void expresion_conjuncion();
void expresion_relacional();
void expresion_aritmetrica();
void termino();
void factor();
void valor_cadena();
void valor_caracter();
void funcion_arreglo();
void funcion_archivo();
void llamada_metodo();
void funcion_num_numcad();
void instruccion_probar(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void instruccion_hacer(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);
void instruccion_paraCada(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo);


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
    int b = pasada2(lsfiles, cant);
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
        programaP1(miArchivo);
        fclose(fp);
        fp = NULL;
    }
    return 1;
}

int pasada2(char **lsfiles, int cant) {
    printf("------------------------------------------------------\n");
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

        struct nodoArchivo *valor = NULL;
        buscarArchivoTDS(&valor, &tabla, lsfiles[i]);
        printf("Archivo %s \n", lsfiles[i]);
        if (valor == NULL) {
            printf("EL ARCHIVO NO SE REGISTRO BIEN PANICO TOTAL LA SEGUNDA PASADA PROBABLEMETE FUE EJECUTADA DE FORMA INDEPENDIENTO O SE CAMBIARON LA LISTA DE ARCHIVOS");
            log_error(1);

        }
        obtoken();
        programa(valor);

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

void programaP1(struct nodoArchivo *archivo) {
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
                        cuerpoP1(claseActual);
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

void cuerpoP1(struct clase *clase) {
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
                    struct metodo *metodo;
                    metodo = (struct metodo*) malloc(sizeof (struct metodo));
                    metodo->ident = (char *) malloc(sizeof (char)*strlen(nombre) + 1);
                    strcpy(metodo->ident, nombre);

                    insertarTDSMetodo(clase, metodo);

                    obtoken();
                    if (token == corcheteF) {
                        //metodo sin parametros
                        metodo->parametros = NULL;
                        printf("\t\t\t sin parametros \n");
                        obtoken();
                        if (token == arregloTok || token == objetoTok || token == archivoTok
                                || token == caracterTok || token == cadenaTok || token == enteroTok
                                || token == realTok || token == byteTok || token == booleanoTok || token == ident) {
                            printf("\t\t\t y con retorno %d\n", token);
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

                            printf("\t\t\tSe detecto parametro de tipo ");
                            tipoDP1(parametroX);

                            if (token == por) {
                                printf(" es por referencia ");
                                parametroX->esPorReferencia = TRUE;
                                obtoken();
                            }

                            if (token == ident) {
                                parametroX->ident = (char *) malloc(sizeof (char) * strlen(lex) + 1);
                                obtoken();
                                i = 1;
                                printf("Se inserta el parametro \n");
                            } else {
                                log_error(1); // Se esperaba un ident
                                printf("Error \n");
                            }
                        } while (token == coma);

                        if (token == corcheteF) {
                            obtoken();
                            if (token == arregloTok || token == objetoTok || token == archivoTok
                                    || token == caracterTok || token == cadenaTok || token == enteroTok
                                    || token == realTok || token == byteTok || token == booleanoTok || token == ident) {
                                printf("\t\t\t y con retorno %d\n", token);
                                metodo->tipoRetorno = token;
                                metodo->esFuncion = -1;
                            } else {
                                if (token == llaveI) countLlaveI++;
                                if (token == llaveF) countLlaveI--;
                            }
                            printf("\t\t\ty esta bien escrito\n");
                        } else {
                            log_error(1); // si esperaba corchete de cierre
                        }
                    }


                }

            }

            //Es propiedad
            if (propiedad == 2 || propiedad == 1) {
                printf("\t\tPropiedad de tipo");

                struct atributo *atributo = (struct atributo*) malloc(sizeof (struct atributo));
                atributo->alcanze = alcanze;
                //ES un propieda cuyo tipo es un objeto
                if (propiedad == 2) {
                    //En la version completa buscar el hashClase
                    printf(" %s que es un objeto ", nombre);
                    atributo->esPrimitivo = 0;
                    atributo->tipo = OBJETO;

                } else {
                    tipoDP1(atributo);
                }
                //Para que verificar el tipo
                //Y ademas venga el inicio del registro
                //De tds


                if (token == ident) {
                    printf("El nombre es %s ", lex);
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
                                printf("\t\t luego esta %s ", lex);
                                insertarTDSAtributo(clase, atributo2);
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

void tipoDP1(struct atributo *atributo) {
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
                tipoDP1(valor);

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
                } else {
                    log_error(1); //se esperaba cierre
                }
            }
            break;
        default:
            printf("Error en tipo \n");
            log_error(1); //NO es un tipo de dato valido
    }
    obtoken();

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

void programa(struct nodoArchivo* miArchivo) {

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
                    printf("El archivo a incluir no se reconoce\n");
                    log_error(13);
                }
                if (token == puntoycoma) {
                    printf("Incluir %s\n", valorCadena);
                    obtoken();
                } else {
                    log_error(14); //se esperaba ;
                }
            } else {
                log_error(15); //se espera cadena
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
                    }
                    printf("------------Soy la clase %s\n", yoClase->ident);

                    //verifica que la clase solo exita en este archivo
                    if (evitarRedefinicionClase(lex, miArchivo, &tabla) >= 1) {
                        printf("Esta clase exite en mas de un lugar\n");
                        log_error(20); //la calse estaba definida
                    }
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        cuerpo(miArchivo, yoClase);
                        if (token == llaveF) {
                            printf("-------------Clase bien escrita-----------------\n");
                            obtoken();
                        } else {
                            log_error(22); //se esperaba llaver de cierre
                        }
                    } else {
                        log_error(21); //se esperaba una llave de apertura
                    }
                } else {
                    log_error(18); //se eperaba nombre de clase
                }
            } else {
                log_error(17); //Se esperaba la declaracion de un clase
            }
        } else {
            log_error(16);
        }
    } while (token != -1);
}

void cuerpo(struct nodoArchivo* miArchivo, struct clase *clase) {
    if (token == llaveF) {
        printf("\t No tenia nada");
        return;
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
                            expresion();
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
                                            expresion();
                                        }
                                    } else {
                                        printf("\t\t Se esta redefiniedo el atributo %s\n", lex);
                                        log_error(23); //este atributo no se detecto en la primera pasada 
                                    }
                                } else {
                                    log_error(18); //se esperaba un idnet de atributo
                                }
                            }
                        } while (token != puntoycoma);

                        if (token == puntoycoma) {
                            obtoken();
                            printf("----------Linea de declaracion bien escrita \n------");
                        } else {
                            printf("Falta ; \n");
                            log_error(14);
                        }
                    } else {
                        printf("\t\t Se esta redefiniedo el atributo %s\n", lex);
                        log_error(23); //este atributo no se detecto en la primera pasada
                    }
                } else {
                    log_error(18); //atributo mal escrito
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
                                expresion();
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
                                                expresion();
                                            }
                                        } else {
                                            printf("\t\t Se esta redefiniedo el atributo %s (objeto)\n", lex);
                                            log_error(23); //este atributo no se detecto en la primera pasada  
                                        }
                                    } else {
                                        log_error(18); //se esperaba un idnet de atributo
                                    }
                                }
                            } while (token != puntoycoma);

                            if (token == puntoycoma) {
                                obtoken();
                                printf("----------Linea de declaracion bien escrita (Objeto) \n------------------");
                            } else {
                                printf("Falta ;\n");
                                log_error(14); //falta ;
                            }
                        } else {
                            printf("\t\tSe esta redefiniedo el atributo %s\n", lex);
                            log_error(23); //este atributo no se detecto en la primera pasada
                        }

                    } else {
                        log_error(18); //se esperava un ident para el atriburo
                    }
                } else {
                    printf("\t\t---------Definicion de metodo-------------------- \n");
                    metodo(miArchivo, clase);
                    printf("\t\t---------Definicio de metodo completa---------------- \n");
                }

            }
        } else {
            log_error(16); //se eperaba modificador de acceso
        }

        if (token == llaveF) {
            return;
        }

    } while (token != -1);
    printf("\n------------------Se acabo el archivo-----------------\n");
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
                log_error(24); // se esperaba un identi de clase
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
                tipo(miArchivo, clase);
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
            return 0;
            log_error(27); //NO es un tipo de dato valido
    }
    return 0;
}

void metodo(struct nodoArchivo* miArchivo, struct clase *clase) {
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
                        }
                    }
                    printf("Metodo %s bien declarado \n", metodo->ident);
                    if (rs == 0) {
                        obtoken();
                    }
                    bloque(miArchivo, clase, metodo);
                } else {
                    parametro(miArchivo, clase, metodo);
                    while (token == coma) {
                        obtoken();
                        parametro(miArchivo, clase, metodo);
                    }
                    if (token == corcheteF) {
                        obtoken();
                        int rs = tipo(miArchivo, clase);
                        if (rs == 1) {
                            //tiene rotorno verificar si es el mismo
                            if (metodo->tipoRetorno != token) {
                                printf("Tipo de retono inesperado");
                                log_error(30);
                            }
                        }
                        printf("Metodo %s con parametros bien declarado \n", metodo->ident);

                        if (rs == 0) {
                            obtoken();
                        }

                        bloque(miArchivo, clase, metodo);
                    } else {
                        log_error(31); // se esperaba cierre de corchete
                    }

                }
            } else {
                log_error(29);
            }
        } else {
            printf("Se esta intentando redefinir el metodo %s\n", lex);
            log_error(28); // declaracion mal escrita
        }
    } else {
        log_error(18);
    }
}

void parametro(struct nodoArchivo *miArchivo, struct clase *clase, struct metodo *metodo) {
    if (tipo(miArchivo, clase) == 1) {

        if (token == por) {
            obtoken();
        }

        if (token == ident) {
            printf("Parametro %s detectado\n", lex);
            //Falta verificar
            obtoken();
        } else {
            log_error(18); // Se esperaba un ident

        }
    }
}

void bloque(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    printf("Entrando a bloque\n");
    if (token == llaveI) {
        obtoken();
        int seguir = 1;
        do {

            do {
                int rs;
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
                        printf("\nSe detecto declaracion");
                        obtoken();
                        if (token == asignacion) {
                            obtoken();
                            expresion();
                        }
                        while (token == coma) {
                            obtoken();
                            if (token == ident) {
                                printf(" Se detecto declaracion misma linea");
                                obtoken();
                                if (token == asignacion) {
                                    obtoken();
                                    expresion();
                                }
                                printf("\n");
                            } else {
                                log_error(18);
                            }
                        }
                    } else {
                        log_error(18);
                    }
                }
            } while (token != puntoycoma);
            if (seguir == -1 || seguir == 0) {
                break;
            }
            obtoken();
        } while (seguir == 1);
        instruccion(miArchivo, clase, metodo);
        if (token == llaveF) {
            printf("Metodo OK\n");
            obtoken();
        } else {
            log_error(22); //se esperaba una llave de cierre
        }
    } else {
        log_error(21); //se esperaba una llave de apertura
    }
}

void instruccion(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    switch (token) {
        case ident:
            obtoken();
            //Es una llamada a metodo de una clase
            if (token == punto) {
                obtoken();
                if (token == abrirTok || token == leerLineaTok
                        || token == volcadoTok || token == cerrarTok) {
                    funcion_archivo();
                    if (token == puntoycoma) {
                        obtoken();
                    } else {
                        log_error(1); //se eperaba punto y coma
                    }
                } else {
                    //verificar si el identificador que tengo es de metodo
                    llamada_metodo();
                    if (token == puntoycoma) {
                        obtoken();
                        printf("EStaba escrita bien la llamada a metodo\n");
                    } else {
                        log_error(14); //se esperaba un punto y coma
                    }
                }
            } else {
                //Es una asignación
                if (token == asignacion) {
                    printf("Se esta haciendo una asignacion\n");
                    obtoken();
                    asignacionf();
                    if (token == puntoycoma) {
                        obtoken();
                        printf("EStaba escrita bien la asgnacion\n");
                    }
                } else {
                    log_error(32); //esperaba una llaamda a metodo o un asignacion;
                }
            }
            break;
        case romperTok:
            instruccion_romper();
            printf("SE encontro romper\n");
            break;
        case siTok:
            instruccion_si(miArchivo, clase, metodo);
            //printf("SE encontro si\n");
            break;
        case probarTok:
            instruccion_probar(miArchivo, clase, metodo);
            break;
        case mientrasTok:
            instruccion_mientras(miArchivo, clase, metodo);
            break;
        case paraTok:
            instruccion_para();
            break;
        case hacerTok:
            instruccion_hacer(miArchivo, clase, metodo);
            break;
        case paracadaTok:
            instruccion_paraCada(miArchivo, clase, metodo);
            break;
        case sistemaTok:
            instruccion_es();
            break;
        case retornarTok:
            obtoken();
            expresion();
            if (token == puntoycoma) {
                printf("REtornar bien escrito");
                obtoken();
            } else {
                log_error(14); //se esperaba punto y coma
            }
            break;
        case llaveF:
            printf("NO hAY INSTRUCCION"); // no se adelante por que encontro el siguiente
            return;
            break;
        default:
            log_error(33); //carater inseperado
            return;
            break;

    }
    instruccion(miArchivo, clase, metodo);
}

void instruccion_si(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == siTok) {
        printf("se encontro sitok\n");
        obtoken();
        if (token == corcheteI) {
            printf("se encontro corcheteI\n");
            obtoken();
            printf("a entrar a expresion numerica\n");
            expresion();
            if (token == coma) {
                printf("\nEncontre una coma\n");
                obtoken();

                asignacionf();
                printf("\nRegrese de asignacion en si corto\n");
                if (token == coma) {
                    printf("\nEncontre segunda coma\n");
                    obtoken();
                    asignacionf();
                    printf("\nRegrese de 2 asignacion en si corto\n");
                    if (token == corcheteF) {
                        printf("\nCorchet F de si corto\n");
                        obtoken();
                        if (token == puntoycoma) {
                            obtoken();
                            printf("Si corto bien\n");
                        } else {
                            log_error(14); //faltaba punto y coma
                        }
                    } else {
                        printf("Ultimo lex leido %s \n ", lex);
                        log_error(31); //faltaba corchete de cierre
                    }
                } else {
                    log_error(36); //se esperaba una coma
                }
            } else {
                if (token == corcheteF) {
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        instruccion(miArchivo, clase, metodo);
                        if (token == llaveF) {
                            obtoken();
                            if (token == sinoTok) {
                                printf("\n \t Sino\n");
                                obtoken();
                                if (token == llaveI) {
                                    printf("Sino llaveI OK\n");
                                    obtoken();
                                    instruccion(miArchivo, clase, metodo);
                                    if (token == llaveF) {
                                        obtoken();
                                        printf("\nSino bien escrito\n");
                                    } else {
                                        log_error(22); //se esperaba llave de cierre
                                    }
                                } else {
                                    printf("falta llave");
                                    log_error(21); //se esperaba llave de apertura
                                }
                            } else {
                                return;
                            }
                        } else {
                            log_error(22); //faltaba llave de cierre
                        }

                    } else {
                        log_error(21); //se esperaba llave de apertura
                    }
                } else {
                    log_error(35); //se esperaba una coma o corchete de cierre
                }
            }
        } else {
            log_error(29); //se esperaba una corchete de apertura
        }
    } else {
        log_error(34); // se esperaba el token si
        printf("Se epserab si tok");
    }
}

void asignacionf() {
    if (token == ident) {
        printf("\nEncontres un ident\n");
        obtoken();
        if (token == asignacion) {
            printf("\nEncontre un  = \n");
            obtoken();
            expresion();
        } else {
            log_error(32);
        }
    } else {
        log_error(18); //se esperaba un identificador
    }
}

void funcion_cadena() {
    if (token == concatenarTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            valor_cadena();
            if (token == coma) {
                obtoken();
                valor_cadena();
                if (token == corcheteF) {
                    obtoken();
                    printf("funcion concatenar completa\n");
                } else {
                    log_error(31);
                }
            } else {
                log_error(36); //se esperaba una coma
            }
        } else {
            log_error(29); //faltaba corchete de apertura
        }
    } else {
        log_error(37);
    }
}

void funcion_archivo() {
    if (token == abrirTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            valor_cadena();
            if (token == corcheteF) {
                obtoken();
                printf("funcion archivo bien escrita camino 1");
            } else {
                log_error(31); //se esperaba corchete de cierre
            }
        } else {
            log_error(29); //se esperaba corchete de apertura
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
                    }
                } else {
                    log_error(29); //se esperaba corchete de abierto
                }
                break;
            default:
                log_error(38); //NO deberia llegar aqui Se esperaba la llamada a funciones de archivos
        }
    }
}

void valor_cadena() {
    switch (token) {
        case datoCadena:
            printf("Hay una cadena\n");
            obtoken();
            break;
        case ident:
            obtoken();
            if (token == punto) {
                obtoken();
                llamada_metodo(); // o indetificador??
            }
            break;
        case concatenarTok:
            funcion_cadena();
            break;
        default: log_error(15); //se esperaba un valor cadena
    }


}

void valor_caracter() {
    switch (token) {
        case datoCaracter:
            obtoken();
            break;
        case ident:
            obtoken();
            if (token == punto) {
                obtoken();
                llamada_metodo(); // o indetificador??
            }
            break;
        default: log_error(15);
    }
}

void llamada_metodo() {
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
                        log_error(1); //se espera un ident
                    }
                } else {
                    expresion();
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
                            log_error(1); //se espera un ident
                        }
                    } else {
                        expresion();
                    }
                    i++;
                }
                if (token == corcheteF) {
                    printf("BUena llamada a metodo\n");
                    obtoken();
                } else {
                    log_error(1); //Falta corchete F
                }
            }

        } else {
            log_error(1); //falta corchete de apertura
        }
    } else {
        funcion_arreglo();
    }
}

void expresion() {
    if (token == verdaderoTok || token == falsoTok) {
        printf("Era un booleano \n");
        obtoken();
        return;
    }
    if (token == datoCadena) {
        printf("se encontro en la expresión una cadena\n");
        obtoken();
        return;
    }
    if (token == concatenarTok) {
        funcion_cadena();
        printf("se encontro una funcion concatenar\n");
        return;
    }
    if (token == datoCaracter) {
        printf("Encontramos un caracter\n");
        obtoken();
        return;
    }
    expresion_numerica();
}

void expresion_numerica() {

    expresion_conjuncion();
    while (token == otok) {
        obtoken();
        printf("En expresion numerica con %s \n", lex);
        expresion_conjuncion();
    }

}

void expresion_conjuncion() {

    expresion_relacional();
    while (token == ytok) {
        obtoken();
        printf("En expresion conjuncion con %s\n", lex);
        expresion_relacional();
    }

}

void expresion_relacional() {

    if (token == negacion) {
        printf("se encontro negación\n");
        obtoken();
    }

    expresion_aritmetrica();
    if (token == mnr || token == myr || token == mai || token == mei || token == igl || token == nig) {
        obtoken();
        printf("Exapresion relacional con %s\n", lex);
        expresion_aritmetrica();
    }

}

void expresion_aritmetrica() {
    termino();
    if (token == mas || token == menos) {
        do {
            obtoken();
            printf("Exapresion aritmetica con %s\n", lex);
            termino();
        } while (token == mas || token == menos);
    }

}

void termino() {
    factor();
    if (token == por || token == barra) {
        do {
            obtoken();
            factor();
        } while (token == por || token == barra);
    }
    printf("\nTerminio termino");
}

void factor() {
    while (token == mas || token == menos) {
        obtoken();
    }

    if (token == numeroEntero || token == numeroReal) {
        obtoken();
        printf("\n\tEncontre un numero\n");
        return;
    }
    if (token == parentI) {
        obtoken();
        expresion_numerica();
        if (token == parentF) {
            obtoken();
            printf("Se encontro una expresion numerica con parentesis\n");
            return;
        } else {
            log_error(1); //se esperaba un token de cierre
        }
    }

    if (token == sistemaTok) {
        obtoken();
        funcion_num_numcad();
        return;
    }
    if (token == ident) {
        obtoken();
        printf("se encontro una variable en factor\n");
        if (token != punto) {
            return;
        } else {
            //falta llamada a metodo
        }
        //obtoken();
        //FALTA LLAMADA A METODO O IDENT (TDS)
    }
    log_error(1); //se esperaba un factor

}

void funcion_num_numcad() {
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
                        expresion_numerica();
                        if (token == corcheteF) {
                            obtoken();
                        } else {
                            log_error(1); //Se esperaba corchete de cierre
                        }
                    } else {
                        log_error(1); //Se esperaba corchet apertura
                    }
                    break;
                case mayorTok:
                case menorTok:
                case potenciaTok:
                case moduloTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        expresion_numerica();
                        if (token == coma) {
                            obtoken();
                            expresion_numerica();
                            if (token == corcheteF) {
                                obtoken();
                            } else {
                                log_error(1); //Se esperaba corchete de cierre
                            }
                        } else {
                            log_error(1); //falto coma
                        }
                    } else {
                        log_error(1); //Se esperaba corchet apertura
                    }
                    break;
                case longitudCadenaTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        valor_cadena();
                        if (token == corcheteF) {
                            obtoken();
                        } else {
                            log_error(1); //Se esperaba corchete de cierre
                        }
                    } else {
                        log_error(1); //Se esperaba corchet apertura
                    }
                    break;
                case compararTok:
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        valor_cadena();
                        if (token == coma) {
                            obtoken();
                            valor_cadena();
                            if (token == corcheteF) {
                                obtoken();
                            } else {
                                log_error(1); //Se esperaba corchete de cierre
                            }
                        } else {
                            log_error(1); //falto coma
                        }
                    } else {
                        log_error(1); //Se esperaba corchet apertura
                    }
                    break;
                default:
                    log_error(1); // se esperaba llamada a sistema;
                    break;
            }
        } else {
            log_error(1); // se esperaba un punto
        }
    } else {
        log_error(1); //Se esperaba palabra reservada sistema
    }
}

void instruccion_mientras(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == mientrasTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            expresion();
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    instruccion(miArchivo, clase, metodo);
                    if (token == llaveF)
                        obtoken();
                    else
                        log_error(1); //no esta }
                } else
                    log_error(1); //no esta {
            } else
                log_error(1); //no esta el ]
        } else
            log_error(1); //no esta el [
    } else
        log_error(1); // no es el token mientras       
}

void instruccion_romper() {
    if (token == romperTok) {
        obtoken();
        if (token == puntoycoma)
            obtoken();
        else
            log_error(1); //no esta el ;
    } else
        log_error(1); // no esta el token romper
}

void funcion_arreglo() {
    switch (token) {
        case agregarTok:
        case obtenerTok:
        case quitarTok: obtoken();
            if (token == corcheteI) {
                obtoken();
                expresion();
                if (token == corcheteF) {
                    obtoken();
                    printf("funcion arreglo ok\n");
                    return;
                } else {
                    log_error(1); //se esperaba corchete de cierre
                }
            } else {
                log_error(1); //se esperaba corchete de apertura
            }
            break;
        case cuantosTok:
            obtoken();
            if (token == corcheteI) {
                obtoken();
                if (token == corcheteF) {
                    obtoken();
                    printf("funcion cuantos ok\n");
                    return;
                } else {
                    log_error(1); //se esperaba corchete de cierre
                }
            } else {
                log_error(1); //se esperaba corchete de apertura
            }
            break;
        default: log_error(1); //no es una funcion arreglo
    }
}

void instruccion_es() {
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
                                    log_error(1); //Se esperaba punto y coma
                                }
                            } else {
                                log_error(1); // se esperaba corchete de cierre
                            }
                        } else {
                            log_error(1); //se esperaba indentificador
                        }
                    } else {
                        log_error(1); // se esperaba corchet ede apartura
                    }
                    break;
                case mostrarTok:
                    if (token == corcheteI) {
                        obtoken();
                        expresion();
                        if (token == coma) {
                            obtoken();
                            expresion();
                        }
                        if (token == corcheteF) {
                            obtoken();
                            if (token == puntoycoma) {
                                obtoken();
                            } else {
                                log_error(1); //Se esperaba punto y coma
                            }
                        } else {
                            log_error(1); // se esperaba corchete de cierre
                        }
                    } else {
                        log_error(1); // se esperaba corchet ede apartura
                    }
                    break;
                default:
                    log_error(1); //se esperaba un token de ontener o mostrar
            }
        } else {
            log_error(1); // se esperaba un punto
        }
    } else {
        log_error(1); //se esperaba el token sistema
    }
}

void instruccion_probar(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == probarTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            expresion();
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    if (token == casoTok) {
                        while (token == casoTok) {
                            obtoken();
                            expresion();
                            if (token == dosPuntos) {
                                obtoken();
                                instruccion(miArchivo, clase, metodo);
                            } else {
                                log_error(1); //faltan :
                            }
                        }
                        if (token == defectoTok) {
                            obtoken();
                            if (token == dosPuntos) {
                                obtoken();
                                instruccion(miArchivo, clase, metodo);
                            } else
                                log_error(1); //falta :
                        }
                        if (token == llaveF) {
                            obtoken();
                        } else
                            log_error(1); //falta}
                    } else
                        log_error(1); //falta caso
                } else
                    log_error(1); //falta {
            } else
                log_error(1); //falta]
        } else
            log_error(1); //falta [
    } else
        log_error(1); //falta probar
}

void instruccion_para(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == paraTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if (token == ident) {
                //verificar que exista en este metodo o clase
                obtoken();
                if (token == asignacion) {
                    obtoken();
                    expresion_numerica();
                    if (token == coma) {
                        obtoken();
                        expresion_numerica();
                        if (token == coma) {
                            obtoken();
                            expresion_numerica();
                            if (token == corcheteF) {
                                obtoken();
                                if (token == puntoycoma) {
                                    obtoken();
                                } else {
                                    if (token == llaveI) {
                                        obtoken();
                                        instruccion(miArchivo, clase, metodo);
                                        if (token == llaveF) {
                                            obtoken();
                                        } else {
                                            log_error(1); //falta }
                                        }
                                    } else {
                                        log_error(1); // falta el ;   
                                    }
                                }
                            } else {
                                log_error(1); //falta]
                            }
                        } else {
                            log_error(1); //falta,
                        }
                    } else {
                        log_error(1); //falta ,
                    }
                } else {
                    log_error(1); //falta =
                }
            } else {
                log_error(1); //no es un identificador
            }
        } else {


            log_error(1); //no es [
        }
    } else {
        log_error(1); //no es la palabra para
    }
}

void instruccion_hacer(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == hacerTok) {
        obtoken();
        if (token == llaveI) {
            obtoken();
            instruccion(miArchivo, clase, metodo);
            if (token == llaveF) {
                obtoken();
                if (token == mientrasTok) {
                    obtoken();
                    if (token == corcheteI) {
                        obtoken();
                        expresion_numerica();
                        if (token == corcheteF) {
                            obtoken();
                            if (token == puntoycoma)
                                obtoken();
                            else
                                log_error(1); //falta ;
                        } else
                            log_error(1); //falta ]
                    } else
                        log_error(1); //falta[
                } else
                    log_error(1); //falta token mientras
            } else
                log_error(1); //falta }
        } else
            log_error(1); //falta {
    }
}

void instruccion_paraCada(struct nodoArchivo* miArchivo, struct clase *clase, struct metodo *metodo) {
    if (token == paracadaTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            tipo(miArchivo,clase);
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
                                 instruccion(miArchivo, clase, metodo);
                                if (token == llaveF) {
                                    obtoken();
                                } else
                                    log_error(1); //falta }
                            } else
                                log_error(1); //falta {
                        } else
                            log_error(1); //falta]
                    } else
                        log_error(1); //no es un identificador
                } else
                    log_error(1); //falta ,
            } else
                log_error(1); // no es un identificador
        } else
            log_error(1); // no esta el [
    } else
        log_error(1); //no esta token paracada 

}
