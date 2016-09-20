#ifndef LEXICO_H
#define LEXICO_H

#ifdef __cplusplus
extern "C" {
#endif
#include "Parametros.h"

    //lista de tokens de ARMUS
enum simbolo {nulo,numeroReal,numeroEntero,ident,mas,menos,por,barra,llaveI,llaveF,parentI,parentF,corcheteI,corcheteF,punto,coma,
puntoycoma,asignacion,mei,mai,myr,mnr,igl,nig,negacion,ytok,otok,referencia,enteroTok,byteTok,
realTok,vacioTok,booleanoTok,cadenaTok,caracterTok,objetoTok,archivoTok,siTok,sinoTok,probarTok,casoTok,defectoTok,
romperTok,mientrasTok,paraTok,hacerTok,paracadaTok,sistemaTok,obtenerEnteroTok,obtenerRealTok,obtenerCadenaTok,
obtenerCaracterTok,mostrarTok,publicaTok,privadaTok,retornarTok,arregloTok,agregarTok,obtenerTok,cuantosTok,
quitarTok,abrirTok,leerLineaTok,volcadoTok,cerrarTok,concatenarTok,parteEnteraTok,compararTok,mayorTok,menorTok,
esParTok,decimalBinTok,potenciaTok,absolutoTok,moduloTok,longitudCadenaTok,claseTok,incluirTok,obtenerBooleanoTok,
falsoTok,verdaderoTok}; //definido aquí en el encabezado

extern enum simbolo token;

//tabla de tokens de palabras reservadas
extern enum simbolo tokpal[MAXPAL]; 

//tabla de tokens correspondientes a operadores y símbolos especiales
extern enum simbolo espec[255] ;



typedef struct hoja{
    char value[MAX_FILE]; //Deberia tener el tamaño de la palabrea reservada mas grande
    enum simbolo token;
    struct hoja* izq;
   struct hoja* derecha;
} arbol;

extern arbol* lexpal;

void inicializarArbolPalabras();
int sumCadena(char *);
void insertarArbol(char *, arbol**, int);

#ifdef __cplusplus
}
#endif

#endif /* LEXICO_H */

