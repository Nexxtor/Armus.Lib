#include <stdio.h>
#include "Errores.h"

char *errores[5] = {"Error el archivo no pudo ser abierto\n", "", "", "", ""};
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

