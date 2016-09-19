#ifndef LEXICO_H
#define LEXICO_H

#ifdef __cplusplus
extern "C" {
#endif
#include "Parametros.h"

//lexemes de las palabras reservadas
extern char *lexpal[MAXPAL];

//lista de tokens de ARMUS
enum simbolo {nulo,ident,numero,mas,menos,por,div,llaveI,llaveF,parentI,parentF,corcheteI,cortecheF,punto,coma,
puntoycoma,comilla,comillas,asginacion,mei,mai,myr,mnr,igl,nig,negacion,y,o,hexaTok,referencia,enteroTok,byteTok,
realTok,vacioTok,booleanoTok,cadenaTok,caracterTok,objetoTok,archivoTok,siTok,sinoTok,probarTok,casoTok,defectoTok,
romperTok,mientrasTok,paraTok,hacerTok,paracadaTok,sistemaTok,obtenerEnteroTok,obtenerRealTok,obtenerCadenaTok,
obtenerCaracterTok,mostrarTok,publicaTok,privadaTok,retornarTok,arregloTok,agregarTok,obtenerTok,cuantosTok,
quitarTok,abrirTok,leerLineaTok,volcadoTok,cerrarTok,concatenarTok,parteEnteraTok,compararTok,mayorTok,menorTok,
esParTok,decimalBinTok,potenciaTok,absolutoTok,moduloTok,longitudCadenaTok,claseTok,incluirTok,obtenerBooleanoTok,
falsoTok,verdaderoTok }; //definido aquí en el encabezado

extern enum simbolo token;

//tabla de tokens de palabras reservadas
extern enum simbolo tokpal [MAXPAL]; 

//tabla de tokens correspondientes a operadores y símbolos especiales
extern enum simbolo espec[255] ;



#ifdef __cplusplus
}
#endif

#endif /* LEXICO_H */

