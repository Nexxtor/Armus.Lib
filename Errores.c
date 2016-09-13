#include <stdio.h>
#include "Errores.h"

char *errores[5] = {"Error el archivo no pudo ser abierto", "", "", "", ""};

void log_error(int error){
    FILE *f=fopen("log.txt","a");
    if(f==NULL){
        printf("Verifique los permisos sobre el directorio\n");
        return;
    }
    fprintf(f,"%s",errores[0]);
    fclose(f);
}

