#include "tds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Lexico.h"
#include "Util.h"
int hash = 1;

void instarArchivoTDS(char *nombreArchivo, tds *t, struct nodoArchivo **arch) {
    tds *tAux = t;
    //Se busca en donde instar
    while (tAux->valor != NULL) {
        int cmp = strcmp(obtenerNombreBase(nombreArchivo), tAux->valor->nombre);
        //esto no deberia de pasar
        // si pasa hay que revisar el preScanner
        if (cmp == 0) {
            printf("Ya existe");
            return;
        }

        if (cmp > 0) {
            if (tAux->dch == NULL) {
                tAux->dch =
                        (struct arbolArchivo *)
                        malloc(sizeof (struct arbolArchivo));
                tAux->dch->valor = NULL;
                tAux->dch->dch = NULL;
                tAux->dch->izq = NULL;
            }
            tAux = tAux->dch;

        } else {
            if (tAux->izq == NULL) {
                tAux->izq =
                        (struct arbolArchivo *)
                        malloc(sizeof (struct arbolArchivo));
                tAux->izq->valor = NULL;
                tAux->izq->dch = NULL;
                tAux->izq->izq = NULL;
            }
            tAux = tAux->izq;
        }
    }

    tAux->valor = (struct nodoArchivo *) malloc(sizeof (struct nodoArchivo));

    struct nodoArchivo *a = tAux->valor;

    a->nombre = (char *) malloc(sizeof (char)* strlen(obtenerNombreBase(nombreArchivo) + 1));
    strcpy(a->nombre, obtenerNombreBase(nombreArchivo));


    a->incluidos = (char **) malloc(sizeof (char *) * 1);
    a->incluidos[0] = (char *) malloc(sizeof (char) * 1);
    a->incluidos[0][0] = '\0';
    a->lsClase = NULL;
    *arch = a;
}

void instarIncluidosArchivo(char *incluido, struct nodoArchivo *miArchivo) {
    int i = 0;
    while (miArchivo->incluidos[i][0] != '\0' && ++i);
    // printf("Se insertara en la posición %i\n", i);

    free(miArchivo->incluidos[i]);
    miArchivo->incluidos = realloc(miArchivo->incluidos, sizeof (char *)*(i + 1));
    miArchivo->incluidos[i] = (char *) malloc(sizeof (char)* strlen(obtenerNombreBase(incluido) + 1));
    strcpy(miArchivo->incluidos[i], obtenerNombreBase(incluido));
    miArchivo->incluidos[i + 1] = (char *) malloc(sizeof (char) * 1);
    miArchivo->incluidos[i + 1][0] = '\0';

    //falta insterar en incluidos la ruta absoluta 

}

void insertarTDSClase(struct nodoArchivo *archivo, char * nombre, int alcanze, struct clase **clase) {

    if (archivo->lsClase == NULL) {
        //POr que al principio no hay nada inicializado
        //  printf("Creando espacio para la clase\n");
        struct listaClase *ins;
        ins = (struct listaClase*) malloc(sizeof (struct listaClase));
        ins->sig = NULL;
        ins->clase = NULL;
        archivo->lsClase = ins;
        ins->clase = (struct clase *) malloc(sizeof (struct clase));
        ins->clase->esLocal = (alcanze == localTok) ? TRUE : FALSE;
        ins->clase->ident = (char *) malloc(sizeof (char) * strlen(nombre));
        ins->clase->esClasePrincipal = 0;
        ins->clase->lsAtributo = NULL;
        ins->clase->lsMetodo = NULL;
        strcpy(ins->clase->ident, nombre);

        ins->clase->hash = hash;
        hash++;
        *clase = ins->clase;
        printf("SE inserto primera clase en el archiv\n");
        return;
    }
    printf("SE inserto Z clase  en el archiv\n");
    //Algoritmo de insercion como tal
    struct listaClase *s = archivo->lsClase;
    while (s->sig != 0) {
        printf("Saltando %s\n", s->clase->ident);
        s = s->sig;
    }
    printf("Saltando %s\n", s->clase->ident);
    struct listaClase *ins;
    ins = (struct listaClase*) malloc(sizeof (struct listaClase));
    ins->sig = NULL;
    ins->clase = NULL;
    s->sig = ins;
    // printf("Guardando clase\n");
    ins->clase = (struct clase *) malloc(sizeof (struct clase));
    ins->clase->esLocal = (alcanze == localTok) ? TRUE : FALSE;
    ins->clase->ident = (char *) malloc(sizeof (char) * strlen(nombre));
    ins->clase->esClasePrincipal = 0;
    ins->clase->lsAtributo = NULL;
    ins->clase->lsMetodo = NULL;
    strcpy(ins->clase->ident, nombre);

    ins->clase->hash = hash;
    hash++;

    *clase = s->sig->clase;

}

void insertarTDSAtributo(struct clase *clase, struct atributo *atributo) {
    if (clase->lsAtributo == NULL) {
        clase->lsAtributo = (struct listaAtributo *) malloc(sizeof (struct listaAtributo));
        clase->lsAtributo->atributo = atributo;
        clase->lsAtributo->sig = NULL;
        printf("\n\t\t\tSe inserto el altributo %s\n", clase->lsAtributo->atributo->ident);
    } else {
        struct listaAtributo *ls = clase->lsAtributo;
        while (ls->sig != NULL) {
            ls = ls->sig;
        }
        ls->sig = (struct listaAtributo *) malloc(sizeof (struct listaAtributo));
        ls->sig->atributo = atributo;
        ls->sig->sig = NULL;
        printf("\n\t\t\tSe inserto el altributo %s\n", ls->sig->atributo->ident);
    }

}

void insertarTDSMetodo(struct clase *clase, struct metodo *metodo) {
    if (clase->lsMetodo == NULL) {
        clase->lsMetodo = (struct listaMetodo*) malloc(sizeof (struct listaMetodo));
        clase->lsMetodo->metodo = metodo;
        clase->lsMetodo->sig = NULL;
        printf("\t\t\t SE REGISTRO el metodo %s\n", clase->lsMetodo->metodo->ident);
    } else {
        struct listaMetodo *s = clase->lsMetodo;
        while (s->sig != NULL) {
            s = s->sig;
        }
        s->sig = (struct listaMetodo*) malloc(sizeof (struct listaMetodo));
        s->sig->metodo = metodo;
        s->sig->sig = NULL;
        printf("\t\t\t SE REGISTRO el metodo %s\n", s->sig->metodo->ident);
    }


}

void buscarArchivoTDS(struct nodoArchivo **archivo, tds *tabla, char * buscado) {
    tds *s = tabla;
    if (s == NULL) {
        *archivo = NULL;
        return;
    }

    int cmp = strcmp(obtenerNombreBase(buscado), s->valor->nombre);
    //printf("Se esta comparando %s con %s %d\n",obtenerNombreBase(buscado),s->valor->nombre, cmp);
    if (cmp == 0) {
        *archivo = s->valor;
        //  printf("Se encontro el archivo\n");
    } else {
        if (cmp > 0) {
            buscarArchivoTDS(archivo, s->dch, buscado);
        } else {
            buscarArchivoTDS(archivo, s->izq, buscado);
        }
    }
}

int evitarRedefinicionClase(char *lex, struct nodoArchivo *miArchivo, tds *tabla) {
    if (tabla == NULL) {
        return 0;
    } else {
        int drch = evitarRedefinicionClase(lex, miArchivo, tabla->dch);
        int izq = evitarRedefinicionClase(lex, miArchivo, tabla->izq);

        if (tabla->valor == NULL) {
            return drch + izq;
        }
        int actual = 0;
        struct listaClase *s = tabla->valor->lsClase;
        while (s != NULL) {
            if (strcmp(s->clase->ident, lex) == 0) {
                /*if (!(s->clase->esLocal == TRUE && tabla->valor != miArchivo)) {*/
                    actual++;
                //}

            }
            s = s->sig;
        }

        if (tabla->valor == miArchivo) {
            actual--;
        }
        return drch + izq + actual;
    }
}

void obtenerClase(struct nodoArchivo *miArchivo, struct clase ** clase, char *lex) {
    struct listaClase *ls = miArchivo->lsClase;
    if (ls == NULL) {
        *clase = NULL;
        return;
    }

    while (ls->sig != NULL) {
       // printf("Comparando %s, %s \n", ls->clase->ident, lex);
        if (strcmp(ls->clase->ident, lex) == 0) {
            *clase = ls->clase;
            return;
        }
        ls = ls->sig;
    }

    if (strcmp(ls->clase->ident, lex) == 0) {
        *clase = ls->clase;
        return;
    }

    *clase = NULL;
}

int puedoUsarEsteTipo(char *buscado, struct nodoArchivo *miArchivo, struct clase *clase, tds *tabla) {
    //SE busca en el Archivo actula
    struct clase * resultado = NULL;
    obtenerClase(miArchivo, &resultado, buscado);
    if (resultado != NULL) {
        return 1;
    }

    //Se busca en los archivos que se incluyeron
    int i = 0;
    while (miArchivo->incluidos[i][0] != '\0') {
        struct nodoArchivo *archivo = NULL;
        buscarArchivoTDS(&archivo, tabla, miArchivo->incluidos[i]);
        struct clase * resultado = NULL;
        obtenerClase(miArchivo, &resultado, buscado);
        if (resultado != NULL) {
            if (resultado->esLocal == TRUE)
                return 0;
            return 1;
        }
        i++;
    }
    return 0;
}

void buscarAtributo(struct atributo **atr, struct clase *clase, char *buscado) {
    if(clase == NULL){
        *atr = NULL;
        return;
    }
    
     
     struct listaAtributo *ls = clase->lsAtributo;
     int  i = 0;
     while(ls!= NULL){
         //printf("Comparando %s,%s\n",ls->atributo->ident,buscado);
         if(strcmp(ls->atributo->ident,buscado) == 0 ){
             *atr = ls->atributo;
             i++;
         }
         ls = ls->sig;
     }
     if(i != 1) *atr = NULL;
    
}

void buscarMetodo(struct metodo **metodo,struct clase* clase, char * buscado){
    
    struct listaMetodo *ls = clase->lsMetodo;
    
    int i = 0;
    while(ls != NULL){
        if(strcmp(ls->metodo->ident,buscado) == 0){
            *metodo =  ls->metodo;
            i++;
        }
        ls = ls->sig;
    }
    if(i != 1) *metodo  = NULL;
    
}