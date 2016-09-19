#include "Lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={"si","byte","real","sino","caso","para","vacio","hacer",
"abrir","mayor","menor","esPar","clase","falso","entero","cadena","Objeto",
"probar","romper","quitar","cerrar","modulo","Archivo","defecto","Sistema",
"mostrar","publica","privada","Arreglo","agregar","obtener","cuantos","volcado",
"incluir","booleano","caracter","mientras","retornar","comparar","absoluto",
"paraCada","leerLinea","potencia","verdadero","concatenar","decimalBin",
"obtenerReal","parteEntera","obtenerEntero","ObtenerCadena","longitudCadena",
"obtenerCaracter","obtenerBooleano"};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
enum simbolo tokpal[MAXPAL]={siTok,byteTok,realTok,sinoTok,casoTok,paraTok,vacioTok,hacerTok,
abrirTok,mayorTok,menorTok,esParTok,claseTok,falsoTok,enteroTok,cadenaTok,objetoTok,
probarTok,romperTok,quitarTok,cerrarTok,moduloTok,archivoTok,defectoTok,sistemaTok,
mostrarTok,publicaTok,privadaTok,arregloTok,agregarTok,obtenerTok,cuantosTok,volcadoTok,
incluirTok,booleanoTok,caracterTok,mientrasTok,retornarTok,compararTok,absolutoTok,
paracadaTok,leerLineaTok,potenciaTok,verdaderoTok,concatenarTok,decimalBinTok,
obtenerRealTok,parteEnteraTok,obtenerEnteroTok,obtenerCadenaTok,longitudCadenaTok,
obtenerCaracterTok,obtenerBooleanoTok};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;
