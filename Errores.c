#include <stdio.h>
#include "Errores.h"

char *errores[7] = {
    "000 Error importante: un archivo no pudo ser abierto \n",
    "001 Error: existe un enter entre incluir y \" \n",
    "002 Error: falto comillia (\") de cierre en un incluir \n",
    "003 Error: falto un ; al final de una declaración incluir \n",
    "004 Error: salto de linea inesperado en una declaración incluir \n",
    "005 Error: nombre o ruta del archivo es demasiado largo \n",
    "006 Error importante: se alcanzo la cantidad maxima de archivos a incluir \n"};
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
    fprintf(f,"%s",errores[0]);
    fclose(f);
}

