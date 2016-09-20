#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "Lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
arbol *lexpal;
//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal[MAXPAL] = {siTok, byteTok, realTok, sinoTok, casoTok, paraTok, vacioTok, hacerTok,
    abrirTok, mayorTok, menorTok, esParTok, claseTok, falsoTok, enteroTok, cadenaTok, objetoTok,
    probarTok, romperTok, quitarTok, cerrarTok, moduloTok, archivoTok, defectoTok, sistemaTok,
    mostrarTok, publicaTok, privadaTok, arregloTok, agregarTok, obtenerTok, cuantosTok, volcadoTok,
    incluirTok, booleanoTok, caracterTok, mientrasTok, retornarTok, compararTok, absolutoTok,
    paracadaTok, leerLineaTok, potenciaTok, verdaderoTok, concatenarTok, decimalBinTok,
    obtenerRealTok, parteEnteraTok, obtenerEnteroTok, obtenerCadenaTok, longitudCadenaTok,
    obtenerCaracterTok, obtenerBooleanoTok};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255];

void inicializarArbolPalabras() {
    char *lexPalAux[MAXPAL] = {"si", "byte", "real", "sino", "caso", "para", "vacio", "hacer",
        "abrir", "mayor", "menor", "esPar", "clase", "falso", "entero", "cadena", "Objeto",
        "probar", "romper", "quitar", "cerrar", "modulo", "Archivo", "defecto", "Sistema",
        "mostrar", "publica", "privada", "Arreglo", "agregar", "obtener", "cuantos", "volcado",
        "incluir", "booleano", "caracter", "mientras", "retornar", "comparar", "absoluto",
        "paraCada", "leerLinea", "potencia", "verdadero", "concatenar", "decimalBin",
        "obtenerReal", "parteEntera", "obtenerEntero", "ObtenerCadena", "longitudCadena",
        "obtenerCaracter", "obtenerBooleano"};

    int mitad = MAXPAL / 2, i = 0;

    arbol* raiz = NULL;
    raiz = (arbol *) malloc(sizeof (arbol));

    strncpy(raiz->value, lexPalAux[mitad], MAX_FILE);
    raiz->derecha = NULL;
    raiz->izq = NULL;
    raiz->token = tokpal[mitad];

    for (i = 0; i < MAXPAL; i++) {
        if (mitad != i) {
            printf("%d %s\n",i,lexPalAux[i]);
            insertarArbol(lexPalAux[i], &raiz, i);
        }
    }
    lexpal = raiz;

}

int sumCadena(char *cad) {
    int suma = 0, i = 0;
    int larg = strlen(cad);
    for(i = 0 ; i< larg ; i ++){
        suma += cad[i];
    }
    return suma;
}

void insertarArbol(char * valor, arbol **raiz, int pos) {
    arbol *nuevo;
    if ( strcmp(valor,(*raiz)->value) > 0) {
        if ((*raiz)->derecha == NULL) {
            nuevo = (arbol *) malloc(sizeof (arbol));
            strncpy(nuevo->value,valor,MAX_FILE);
            nuevo->derecha = NULL;
            nuevo->izq = NULL;
            nuevo->token = tokpal[pos];
            (*raiz)->derecha =nuevo;
        } else {
            insertarArbol(valor, &((*raiz)->derecha),pos);
        }
    } else {
        if ((*raiz)->izq == NULL) {
            nuevo = (arbol *) malloc(sizeof (arbol));
             strncpy(nuevo->value,valor,MAX_FILE);
            nuevo->derecha = NULL;
            nuevo->izq = NULL;
            (*raiz)->izq =nuevo;
            nuevo->token = tokpal[pos];
        } else {
            insertarArbol(valor, &((*raiz)->izq),pos);
        }
    }
}