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
void programaP1(struct nodoArchivo *archivo);
void cuerpoP1(struct clase *clase);
void tipoDP1(struct atributo *atributo);
void copiarValor(struct atributo *dest, struct atributo *org);
void programa( struct nodoArchivo *);

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
        buscarArchivoTDS(&valor,&tabla,lsfiles[i]);
        printf("Archivo %s \n",lsfiles[i]);
        if(valor == NULL){
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
                            metodo->tipoRetorno =  token; //Cambiar esto por el tipo en mayusculas
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
                            
                            parametros->atributo =  parametroX;
                            
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
                                metodo->tipoRetorno = -1;
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
void programa( struct nodoArchivo *miArchivo) {

    //Area de inclusion 
    do {
        if (token == incluirTok) {
            obtoken();
            if (token == datoCadena) {
                obtoken();
                //verificamos que lo que quiere incluir existe
                struct nodoArchivo *incluir = NULL;
                buscarArchivoTDS(&incluir,&tabla,valorCadena);
                //Verificamos pro que le la primera pasada solo se registor
                //pero no se sabia si existia o no
                if(incluir == NULL){
                    printf("El archivo a incluir no se reconoce\n");
                    log_error(1);
                }
                if (token == puntoycoma) {
                    printf("\tincluir %s\n",valorCadena);
                    obtoken();
                } else {
                    log_error(1); //se esperaba ;
                }
            } else {
                log_error(1); //se espera cadena
            }
        }
    } while (token == incluirTok);

    //Area de clase

    do {
        if (token == publicaTok || token == localTok) {
            int tokenAux = token;
            obtoken();
            if (token == claseTok) {
                obtoken();
                if (token == ident) {
                    //verifica que la clase solo exita en este archivo
                    obtoken();
                    if (token == llaveI) {
                        obtoken();
                        cuerpo();
                        if (token == llaveF) {
                            printf("Clase bien escrita\n");
                            obtoken();
                        }
                    } else {
                        log_error(1); //se esperaba una llave de apertura
                    }
                } else {
                    log_error(1); //se eperaba nombre de clase
                }
            } else {
                log_error(1); //Se esperaba la declaracion de un clase
            }
        }
    } while (token != -1);
}
/*
void cuerpo() {
    if (token == llaveF) {
        printf("No tenia nada");
        return;
    }
    if (token == publicaTok || token == privadoTok) {
        obtoken();
        if (token == arregloTok || token == objetoTok || token == archivoTok
                || token == caracterTok || token == cadenaTok || token == enteroTok
                || token == realTok || token == byteTok || token == booleanoTok) {
            tipo();
        } else {
            if (token == ident) {
                //Verifica si en lex esta guardada un ident de clase
                //con la TDS
                if (buscarClaseTDS(lex) == 1) {
                    //significa que el ident leido es una clase
                    obtoken();
                    if (token == ident) {
                        //Verificar si yaesta registrada 
                        printf("Se tiene el atributo bien puesto");
                        obtoken();
                        if (token == igl) {
                            obtoken();
                            expresion();
                        }

                        do {
                            if (token == coma) {
                                obtoken();
                                if (token == ident) {
                                    //Verificar si yaesta registrada 
                                    obtoken();
                                    if (token == igl) {
                                        obtoken();
                                        expresion();
                                    }
                                } else {
                                    log_error(1); //se esperaba un idnet de atributo
                                }
                            }
                        } while (token != puntoycoma);

                        if (token == puntoycoma) {
                            obtoken();
                            printf("Declaracion bien escrita");
                        }
                    } else {
                        log_error(1); //se esperava un ident para el atriburo
                    }
                } else {
                    //puede ser un metodo
                    if (buscarMetodoTDS(clase) == 1) {
                        metodo();
                    } else {
                        log_error(1); /// error desconocido culpa de carlos
                    }
                }
            }
        }
    }

}*/
/*
void metodo() {
    if (token == ident) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if (token == corcheteF) {
                obtoken();
                if (token == arregloTok || token == objetoTok || token == archivoTok
                        || token == caracterTok || token == cadenaTok || token == enteroTok
                        || token == realTok || token == byteTok || token == booleanoTok) {
                    printf("La funcion estaba bien escrita\n");

                }

                if (token == ident) {
                    if (buscarClaseTDS(lex) == 1) {
                        printf("EL tipo de retorno es un objeto y valido");

                    } else {
                        printf("EL tipo de retorno es un objeto y no es valido");
                        log_error(1); // NO exite el tipo de dato lex
                    }
                }
                if (token != llaveI) {
                    obtoken();
                }
                bloque();
            } else {
                parametro();
                while (token == coma) {
                    obtoken();
                    parametro();
                }
                if (token == corcheteF) {
                    obtoken();
                    if (token == arregloTok || token == objetoTok || token == archivoTok
                            || token == caracterTok || token == cadenaTok || token == enteroTok
                            || token == realTok || token == byteTok || token == booleanoTok) {
                        printf("La funcion estaba bien escrita\n");

                    }

                    if (token == ident) {
                        if (buscarClaseTDS(lex) == 1) {
                            printf("EL tipo de retorno es un objeto y valido");

                        } else {
                            printf("EL tipo de retorno es un objeto y no es valido");
                            log_error(1); // NO exite el tipo de dato lex
                        }
                    }
                    if (token != llaveI) {
                        obtoken();
                    }
                    bloque();
                } else {
                    log_error(1); // se esperaba cierre de corchete
                }

            }
        } else log_error(1);
    } else log_error(1);
}

void parametro() {
    tipo();

    if (token == por) {
        obtoken();
    }

    if (token == ident) {
        obtoken();
    } else {
        log_error(1); // Se esperaba un ident
    }
}

void tipo() {
    switch (token) {

        case ident:
            if (buscarClaseTDS(lex) == 1) {
                printf("ES de tipo adecuedo");
            } else {
                log_error(1); // se esperaba un identi de clase
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
        case arregloTok: //Caso dificil
            printf("ES de tipo primitivo");
            break;
        default:
            log_error(1); //NO es un tipo de dato valido
    }
    obtoken();

}

void bloque() {
    if (token == llaveI) {
        obtoken();
        while (token == arregloTok || token == objetoTok || token == archivoTok
                || token == caracterTok || token == cadenaTok || token == enteroTok
                || token == realTok || token == byteTok || token == booleanoTok) {
            declaracion_variable();
            if (token == puntoycoma) {
                obtoken();
            } else {
                log_error(1); //se esperaba un punto y coma
            }
        }
        instruccion();
        if (token == llaveF) {
            obtoken();
        } else {
            log_error(1); //se esperaba una llave de cierre
        }
    }
}

void instruccion() {
    switch (token) {
        case ident:
            obtoken();
            //Es una llamada a metodo de una clase
            if (token == punto) {
                obtoken();
                if (token == abrirTok || token == leerLineaTok
                        || token == volcadoTok || token == cerrarTok) {
                    funcion_archivo();
                } else {
                    llamada_metodo();
                    if (token == puntoycoma) {
                        obtoken();
                        printf("EStaba escrita bien la llamada a metodo\n");
                    } else {
                        log_error(1); //se esperaba un punto y coma
                    }
                }
            } else {
                //Es una asignación
                if (token == igl) {
                    asignacion();
                    if (token == puntoycoma) {
                        obtoken();
                        printf("EStaba escrita bien la asgnacion\n");
                    }
                } else {
                    log_error(1); //esperaba una llaamda a metodo o un asignacion;
                }
            }
            break;
        case: romperTok:
            instruccion_romper();
            break;
        case siTok:
            instruccion_si();
            break;
        case probarTok:
            instruccion_probar();
            break;
        case mientrasTok:
            instruccion_mientras();
            break;
        case paraTok:
            instruccion_para();
            break;
        case hacerTok:
            instruccion_hacer();
            break;
        case paracadaTok:
            instruccion_paraCada();
            break;
        case sistemaTok:
            instruccion_es();
            break;
        case retornaTok:
            obtoken();
            expresion();
            if (token == puntoycoma) {
                printf("REtornar bien escrito");
                obtoken();
            } else {
                log_error(1); //se esperaba punto y coma
            }
            break;
        case llaveF:
            printf("Se acabo insturcion"); // no se adelante por que encontro el siguiente
            return;
            break;

    }
    instruccion();

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
                log_error(1); //se esperaba corchete de cierre
            }
        } else {
            log_error(1); //se esperaba corchete de apertura
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
                        log_error(1); //se esperaba corchete de cierre
                    }
                } else {
                    log_error(1); //se esperaba corchete de abierto
                }
                break;
            default:
                log_error(1); //NO deberia llegar aqui Se esperaba la llamada a funciones de archivos
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
                llamada_metodo();
            }
            break;
        case concatenarTok:
            funcion_cadena();
            break;
    }


}

void declaracion_principal() {
    if (token == publicaTok) {
        obtoken();
        if (token == principalTok) {
            obtoken();
            if (token == corcheteI) {
                obtoken();
                if (token == corcheteF) {
                    obtoken();
                    if (token == llaveI) {
                        //obtoken();
                        bloque();
                    }
                } else {
                    parametros_principal();

                    if (token == corcheteF) {
                        obtoken();
                    }
                }

            } else log_error(1);
        } else log_error(1);
    } else log_error(1);
}

void parametros_principal() {
    if (token == caracterTok || token == cadenaTok || token == enteroTok || token == realTok || token == byteTok || token == booleanoTok) {
        tipo_primitivo();
        obtoken();
        if (token == ident) {
            obtoken();
            if (token == coma) {
                obtoken();
                if (token == caracterTok || token == cadenaTok || token == enteroTok || token == realTok || token == byteTok || token == booleanoTok) {
                    tipo_primitivo();
                    if (token == ident) {
                        obtoken();
                    } else log_error(1);
                } else log_error(1);
            }

        } else log_error(1);
    } else log_error(1);


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
    !, +, -,

}

void expresion_numerica() {
    expresion_conjuncion();
    while (token == otok) {

        obtoken();
        expresion_conjuncion();
    }

}

void expresion_conjuncion() {
    expresion_relacional()
    while (token == ytok) {
        obtoken();
        expresion_relacional();
    }
}

void expresion_relacional() {
    if (token == negacion) {
        obtoken();
    }

    expresion_aritmetica();
    if (token == mnr || token == myr || token == mai || token == mei || token == igl || token == nig) {
        obtoken();
        expresion_aritmetica();
    }
}

void expresion_aritmetica() {
    termino();
    if (token == mas || token == menos) {
        do {
            obtoken();
            termino();
        } while (token == mas || token == menos);
    } else {
        obtoken();
    }
}

void termino() {
    factor();
    if (token == por || token == barra) {
        do {
            obtoken();
            factor();
        } while (token == por || token == barra);
    } else {
        obtoken();
    }
}

void factor(){
    while(token == mas || token == menos){
        obtoken();
    }
    
    if(token == numeroEntero || token == numeroReal){
        obtoken();
        printf("Encontre un numero\n");
        return;
    }
    
    if(token == ident){
        //GUardar este idnet
        //SI esta en la tds seguir
        //sino dar error
        obtoken();
        if(token == punto){
            obtoken();
            //si empeza con la llamada a los
            //token de funcion arreglo
            //llamar a llamada_metodo
            if(token == ident){
                
                
                //Sino con la tds verificar si es un metodo // llmar a llamada metodo
                //Y si una propiedad  buscar en la TDS si existe ok
                
                
                return;
            }else{
                log_error(1); //se espera un atriubuto o metodo
            }
            
        }
    }
    
    //Aqui va funcion
}

void instruccion_mientras() {
    if (token == mientrasTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            expresion_numerica();
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    Instruccion();
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

void instruccion_probar() {
    if (token == probarTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            Expresion();
            if (token == corcheteF) {
                obtoken();
                if (token == llaveI) {
                    obtoken();
                    if (token == casoTok) {
                        while (token == casoTok) {
                            obtoken();
                            Expresion();
                            if (token == dospuntos) {
                                obtoken();
                                Instruccion();
                            } else
                               log_error(1); //faltan :
                        }
                        if (token == defectoTok) {
                            obtoken();
                            if (token == dospuntos) {
                                obtoken();
                                Instruccion();
                            } else
                               log_error(1); //falta :
                        } else
                           log_error(1); //falta defecto
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

void instruccion_para() {
    int i;
    if (token == paraTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            if (token == ident) {
                i = posicion();
                if (i == 0)
                    error(11); //error 11: Identificador no declarado
                else
                    //if (tabla[i].tipo==PROCEDIMIENTO)
                    //   error(21); //error 21: Una expresi\F3n no debe contener un identificador de procedimiento
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
                            } else if (token == llaveI) {
                                obtoken();
                                Instruccion();
                                if (token == llaveF)
                                    obtoken();
                                else
                                   log_error(1); //falta }
                            } else
                               log_error(1); // falta el ;       
                        } else
                           log_error(1); //falta]
                    } else
                       log_error(1); //falta,
                } else
                   log_error(1); //falta ,
            } else
               log_error(1); //no es un identificador
        } else
           log_error(1); //no es [
    } else
        errror(); //no es la palabra para
}

void instruccion_hacer() {
    if (token == hacerTok) {
        obtoken();
        if (token == llaveI) {
            obtoken();
            Instruccion();
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

void instruccion_paraCada() {
    int i;
    if (token == paracadaTok) {
        obtoken();
        if (token == corcheteI) {
            obtoken();
            Tipo();
            if (token == ident) {
                i = posicion();
                if (i == 0)
                    error(11); //error 11: identificador no declarado 
                // else 
                //if (tabla[i].tipo != VARIABLE)
                //    error(12); //error 12: no est\E1n permitidas las asignaciones a constantes o a procedimientos
                obtoken();
                if (token == coma) {
                    obtoken();
                    if (token == ident) {
                        i = posicion();
                        if (i == 0)
                            error(11);
                        // else 
                        //if (tabla[i].tipo != VARIABLE)
                        //error(12); //error 12: no est\E1n permitidas las asignaciones a constantes o a procedimientos
                        obtoken();
                        if (token == corcheteF) {
                            obtoken();
                            if (token == llaveI) {
                                obtoken();
                                Instruccion();
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

void instruccion_obtener() {
    int i;
    if (token == sistemaTok) {
        obtoken();
        if (token == punto) {
            obtoken();
            if (token == obtenerEnteroTok || token == obtenerRealTok || token == obtenerCadenaTok || token == obtenerCaracterTok) {
                obtoken();
                if (token == corcheteI) {
                    obtoken();
                    if (token == ident) {
                        i = posicion();
                        if (i == 0)
                            error(11);
                        // else 
                        //if (tabla[i].tipo != VARIABLE)
                        // error(12); //error 12: no est\E1n permitidas las asignaciones a constantes o a procedimientos
                        obtoken();
                        if (token == corcheteF) {
                            obtoken();
                            if (token == puntoycoma) {
                                obtoken();
                            } else
                               log_error(1); //no es un ;
                        } else
                           log_error(1); //no es un ]
                    } else
                       log_error(1); //no es un identificador
                } else
                   log_error(1); //no es un [
            } else
               log_error(1); // no es entero, real, cadena, o caracter
        } else
           log_error(1); //no es .
    } else
       log_error(1); //no es el token sistema
}

void instruccion_mostrar() {
    if (token == sistemaTok) {
        obtoken();
        if (token == punto) {
            obtoken();
            if (token == mostrarTok) {
                obtoken();
                if (token == corcheteI) {
                    obtoken();
                    Expresion();
                    if (token == coma) {
                        Expresion();
                    }
                    if (token == corcheteF) {
                        obtoken();
                        if (token == puntoycoma)
                            obtoken();
                        else
                           log_error(1); //no es el ;
                    } else
                       log_error(1); //no es ]
                } else
                   log_error(1); //no es [
            } else
               log_error(1); //no es la palabra mostrar
        } else
           log_error(1); //no es un .
    } else
       log_error(1); //no es el token sistema
}
*/