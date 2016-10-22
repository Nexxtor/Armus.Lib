#include "tds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
                tAux->dch->valor = tAux->dch->dch =tAux->dch->izq= NULL;
            } 
            tAux = tAux->dch;
            
        } else {
            if (tAux->izq == NULL) {
                tAux->izq =
                         (struct arbolArchivo *)
                         malloc(sizeof (struct arbolArchivo));
                tAux->izq->valor = tAux->izq->dch =tAux->izq->izq= NULL;
            } 
            tAux = tAux->izq;
        }
    }
    
    tAux->valor = (struct nodoArchivo *) malloc(sizeof(struct nodoArchivo));
    
    struct nodoArchivo *a = tAux->valor;
    
    a->nombre = (char *) malloc(sizeof(char)* strlen(nombreArchivo));
    strcpy(a->nombre,nombreArchivo);
   
    
    a->incluidos= a->lsClase = NULL;
    *arch  = a;
}
