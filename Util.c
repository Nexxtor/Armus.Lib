#include <stdlib.h>
#include "Util.h"
#include "Parametros.h"

char *obtenerDirectorio(const char *strFirstFile){     
     int posSlash = 0,i=0;
     while(strFirstFile[i] != '\0') {
         if(strFirstFile[i]== '/'){
             posSlash = i;
         }
         i++;
     }
     
     char *directorio = (char *) malloc((sizeof(char )* posSlash )+ 1);
     i = 0;
     while(i <= posSlash){
         directorio[i] = strFirstFile[i];
         i++;
     }
     directorio[i] = '\0';
     
     return directorio;
 }

 char *obtenerNombreBase(const char *strFirstFile){     
     int posSlash = 0,i=0, j=0;
     while(strFirstFile[i] != '\0') {
         if(strFirstFile[i]== '/'){
             posSlash = i;
         }
         i++;
     }
     
     char *directorio = (char *) malloc(MAX_NAME_FILE + 1);
     i = posSlash + 1;
     while(strFirstFile[i] != '\0'){
         directorio[j] = strFirstFile[i];
         i++;
         j++;
     }
     directorio[j] = '\0';
     
     return directorio;
 }
