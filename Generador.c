#include "Generador.h"
#include <stdio.h>
#include <stdlib.h>
#include "tds.h"


struct cod codigo;

char *nM[7] = {"INS", "INO", "ALM", "OPR", "SAL", "SAC", "LIT"};

struct ins genIns(enum nemonico f, int n, int d, int p) {
    struct ins instruccion;
    instruccion.f = f;
    instruccion.n = n;
    instruccion.d = d;
    instruccion.p = p;


    printf("%s %d %d %d\n", nM[f], n, d, p);

    return instruccion;
}

struct ins genInsL(enum nemonico f, int n, void * d, int p) {
    struct ins instruccion;
    instruccion.f = f;
    instruccion.n = n;
    //    instruccion.d = d;
    instruccion.p = p;

    char *cadena = (char *) d;
    char *caracter = (char*) d;
    int *entero = (int *) d;
    double *real = (double *) d;
    printf("%s %d ", nM[f], n);
    switch (p) {
        case 1:
           // printf("cadena \n");
            instruccion.data.cadena = (char *) malloc(sizeof(char ) * MAX_CADENA);
            strcpy(instruccion.data.cadena, cadena);
            printf("%s %d \n", instruccion.data.cadena, p);
            break;
        case 2:
            instruccion.data.caracter = *caracter;
            printf("%c %d \n", instruccion.data.caracter, p);
            break;
        case 3:
            instruccion.data.entero = *entero;
            printf("%d %d \n", instruccion.data.entero, p);
            break;
        case 4:
            instruccion.data.doble = *real;
            printf("%f %d \n", instruccion.data.doble, p);
            break;
        case 5:
            //printf("Booleano \n");
            instruccion.data.byte = *entero;
            printf("%d %d \n", instruccion.data.byte, p);
            break;
        case 6:
            instruccion.data.booleano = *entero;
            printf("%d %d \n", instruccion.data.booleano, p);
            break;

    }

    return instruccion;
}

void addLsCode(struct ins codigo, struct lsCod *destino) {
    //printf("Intentando agregar a la lista\n");
    struct lsCod *ls = destino;
    struct lsCod *nuevo = (struct lsCod *) malloc(sizeof (struct lsCod)); //espacio para el nuevo
    //printf("Copiando valor \n");
    nuevo->instruccion = codigo; //copiamos el codigo
    nuevo->sig = NULL; //decimos que no hay siguiente
    //printf("Buscando el final de la lista \n");
    //Se busca el final
    while (ls->sig != NULL) {
        ls = ls->sig;
    }


    ls->sig = nuevo; //Insertamos al final
    //printf("Agregado \n");
}

void addGroupCode(struct lsCod lscod, struct groupCode *grupo) {
    struct groupCode *ls = grupo;
    struct groupCode *nuevo = (struct groupCode *) malloc(sizeof (struct groupCode));

    nuevo->value = lscod;
    nuevo->sig = NULL;

    while (ls->sig != NULL) {
        ls = ls->sig;
    }

    ls->sig = nuevo;
    nuevo->groupNum = ls->groupNum + 1;
}

void addClase(struct claseCod clase, struct cod *cod) {
    struct cod *ls = cod;
    struct cod *nuevo = (struct claseCod *) malloc(sizeof (cod));

    nuevo->clase = clase;
    nuevo->sig = NULL;

    while (ls->sig != NULL) {
        ls = ls->sig;
    }

    ls->sig = nuevo;
}

int codTipo(int org) {
    int tipo = 0;
    switch (org) {
        case CADENA:
            tipo = 1;
            break;
        case CARACTER:
            tipo = 2;
            break;
        case ENTERO:
            tipo = 3;
            break;
        case REAL:
            tipo = 4;
            break;
        case BYTE:
            tipo = 5;
            break;
        case BOOLEANO:
            tipo = 6;
            break;
        case OBJETO:
            tipo = 0;
            break;
        case ARCHIVO:
            tipo = -1;
            break;
        case ARREGLO:
            tipo = -2;
            break;
    }

    return tipo;
}

void genInstacia(int *numAtr, struct atributo *atr, struct lsCod *atributos) {
    if (atr->tipo != OBJETO && atr->tipo != ARCHIVO && atr->tipo != ARREGLO) {
        addLsCode(genIns(INS, 0, 1, codTipo(atr->tipo)), atributos);

    } else {
        int nAnidamiento = 0;
        struct atributo *atrAux = atr;
        switch (atr->tipo) {
            case ARCHIVO:
                addLsCode(genIns(INO, 0, codTipo(atr->tipo), 0), atributos);
                break;
            case OBJETO:
                //printf("%d\n" , atr->hashClase);
                addLsCode(genIns(INO, 0, atr->hashClase, 0), atributos);
                break;
            case ARREGLO:
                //Es un arreglo                
                while (atrAux->tipoContenidoArreglo == ARREGLO) {
                    ++nAnidamiento;
                    atrAux = (struct atributo *) atrAux->valor;
                }
                addLsCode(genIns(INO, nAnidamiento, codTipo(atr->tipo), codTipo(atrAux->tipoContenidoArreglo)), atributos);
                //addLsCode(genIns(ALM, 0, 4, numAtr), &atributos); 
                break;
        }
    }
    //printf("Finalizando instrucción para atributo %s\n",atr->ident);
    addLsCode(genIns(ALM, 0, 4, *numAtr), atributos);
    *numAtr = *numAtr + 1;
}