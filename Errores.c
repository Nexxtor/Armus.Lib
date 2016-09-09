#include <stdio.h>
#include <stdlib.h>

void log_error(int error){
    FILE *f=fopen("log.txt","a");
    if(f==NULL){
        printf("Verifique los permisos sobre el directorio\n");
        return;
    }
    fprintf(f,"%s",errores[error]);
    fclose(f);
}

