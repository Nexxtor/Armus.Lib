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

    a->nombre = (char *) malloc(sizeof (char)* strlen(obtenerNombreBase(nombreArchivo)+ 1));
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
    miArchivo->incluidos[i] = (char *) malloc(sizeof (char)* strlen(obtenerNombreBase(incluido) +1 ));
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

void insertarTDSAtributo(struct clase *clase, struct atributo *atributo){
    if(clase->lsAtributo == NULL){
        clase->lsAtributo =  (struct listaAtributo *) malloc(sizeof(struct listaAtributo));
        clase->lsAtributo->atributo = atributo;
        clase->lsAtributo->sig = NULL;
        printf("\n\t\t\tSe inserto el altributo %s\n",clase->lsAtributo->atributo->ident);
    }else{
       struct listaAtributo *ls =  clase->lsAtributo;
       while(ls->sig != NULL) {
           ls = ls->sig;
       }
       ls->sig =  (struct listaAtributo *) malloc(sizeof(struct listaAtributo));
       ls->sig->atributo = atributo;
       ls->sig->sig = NULL;
       printf("\n\t\t\tSe inserto el altributo %s\n",ls->sig->atributo->ident);
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

void buscarArchivoTDS( struct nodoArchivo **archivo,tds *tabla ,char * buscado){
     tds *s = tabla;
     if(s == NULL){
         *archivo = NULL;
         return;
     }
     
     int cmp = strcmp(obtenerNombreBase(buscado),s->valor->nombre);
     //printf("Se esta comparando %s con %s %d\n",obtenerNombreBase(buscado),s->valor->nombre, cmp);
     if(cmp == 0){
        *archivo = s->valor;
      //  printf("Se encontro el archivo\n");
     }else{
         if(cmp >0){
            buscarArchivoTDS(archivo,s->dch ,buscado); 
         }else{
            buscarArchivoTDS( archivo,s->izq ,buscado);  
         }
     }
 }

int evitarRedefinicionClase(char *lex, struct nodoArchivo *miArchivo, tds *tabla){
    if(tabla == NULL){
        return 0;
    }else{
        int drch = evitarRedefinicionClase(lex, miArchivo, tabla->dch);
        int izq = evitarRedefinicionClase(lex, miArchivo, tabla->izq);
        
        if(tabla->valor == NULL ){
            return drch + izq;
        }
        int actual = 0;
        struct listaClase *s = tabla->valor->lsClase;
        while(s != NULL){
            if(strcmp(s->clase->ident,lex) == 0){
                if(!(s->clase->esLocal == TRUE && tabla->valor != miArchivo )){
                    actual++;
                }
                
            }
            s = s->sig;
        }
        
        if(tabla->valor == miArchivo){
            actual--;
        }
         return drch + izq + actual;
    }
}