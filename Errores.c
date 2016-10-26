#include <stdio.h>
#include <stdlib.h>
#include "Errores.h"
#include <jni.h>
#include "Standar.h"
char *errores[13] = {
    "000 Error importante: un archivo no pudo ser abierto \n",
    "001 Error: existe un enter entre incluir y \" \n",
    "002 Error: falto comillia (\") de cierre en un incluir \n",
    "003 Error: falto un ; al final de una declaración incluir \n",
    "004 Error: salto de linea inesperado en una declaración incluir \n",
    "005 Error: nombre o ruta del archivo es demasiado largo \n",
    "006 Error importante: se alcanzo la cantidad maxima de archivos a incluir \n",
    "007 Error: se alcanzo el limetes de numeros enteros \n",
    "008 Error: los numeros no pueden tener mas de un punto decimal \n",
    "009 Error: Los numero hexadesimales empiezan por # \n",
    "010 Error: Se alcanzo el maxime de la cadena \n",
    "011 Error: Se alcanzo fin de un archivo \n",
    "012 Error: Se esta escrbiendo una cadana en lugar de un caracter \n"};
int primerError = 0;

void log_error(int error){
    FILE *f;
    if(primerError == 0){
        f=fopen("log.txt","w");
        primerError = 1;
    }else{
        f=fopen("log.txt","a");
    }
    if(f==NULL){
        printf("Verifique los permisos sobre el directorio\n");
        return;
    }
    fprintf(f,"%s",errores[1]);

    fclose(f);
    printf("Error %d en la linea %d posicion %d",error, LineaActual,PosicionLineaActual);
    exit(0);
}
