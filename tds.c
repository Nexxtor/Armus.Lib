#include "tds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Lexico.h"

int hash = 1;

void instarArchivoTDS(char *nombreArchivo, tds *t, struct nodoArchivo **arch) {
    tds *tAux = t;
    //Se busca en donde instar
    while (tAux->valor != NULL) {
        int cmp = strcmp(nombreArchivo, tAux->valor->nombre);
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

    a->nombre = (char *) malloc(sizeof (char)* strlen(nombreArchivo));
    strcpy(a->nombre, nombreArchivo);


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
    miArchivo->incluidos[i] = (char *) malloc(sizeof (char)* strlen(incluido));
    strcpy(miArchivo->incluidos[i], incluido);
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
        //printf("Creado ... \n");
    }
    //Algoritmo de insercion como tal
    struct listaClase *s = archivo->lsClase;
    while (s->sig != NULL) s = s->sig;
    if (s->clase != NULL) { //caso considerado acausa del primer if
        //Significa que tengo que insertar el en siguiete
      //  printf("Sera en la sigueinte\n");
        struct listaClase *ins;
        ins = (struct listaClase *) malloc(sizeof (struct listaClase));
        ins->sig = NULL;
        ins->clase = NULL;
        s->sig = ins;
        s = s->sig;
    }
   // printf("Guardando clase\n");
    s->clase = (struct clase *) malloc(sizeof (struct clase));
    s->clase->esLocal = (alcanze == localTok) ? TRUE : FALSE;
    s->clase->ident = (char *) malloc(sizeof (char) * strlen(nombre));
    s->clase->esClasePrincipal = 0;
    s->clase->lsAtributo = NULL;
    s->clase->lsMetodo = NULL;
    strcpy(s->clase->ident, nombre);

    s->clase->hash = hash;
    hash++;

    *clase = s->clase;

}
