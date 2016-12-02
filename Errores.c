#include <stdio.h>
#include <stdlib.h>
#include "Errores.h"
#include "Standar.h"
#include <string.h>
#include <jni.h>

char *errores[58] = {
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
    "012 Error: Se esta escrbiendo una cadana en lugar de un caracter \n",
    "013 El archivo a incluir no se reconoce\n",
    "014 Se esperaba un punto y coma\n",
    "015 Se esperaba un dato cadena\n",
    "016 Se esperaba un modificador de acceso\n",
    "017 Se esperaba la palabra reservada ‘clase’\n",
    "018 Se esperaba un identificador\n",
    "019 Clase no declarada\n",
    "020 El identificador de la clase ya ha sido declarada en su proyecto\n",
    "021 Se esperaba llave de apertura\n",
    "022 Se esperaba llave de cierre\n",
    "023 Atributo no existente\n",
    "024 Clase fuera de ámbito o no declarada\n",
    "025 Se esperaba la apertura del tipo del arreglo (<)\n",
    "026 Se esperaba el cierre del tipo del arreglo (>)\n",
    "027 No es un tipo de dato valido\n",
    "028 Método no declarado\n",
    "029 Se esperaba corchete de apertura\n",
    "030 Tipo de retorno invalido, la definición ya estaba aquí\n",
    "031 Se esperaba corchete de cierre\n",
    "032 Se esperaba el símbolo de asignación (=)\n",
    "033 Se esperaba una instrucción\n",
    "034 Se esperaba la palabra reservada ‘si’\n",
    "035 Se esperaba corchete de cierre o coma\n",
    "036 Se esperaba una coma\n",
    "037 Se esperaba la palabra reservada ‘concatenar’\n",
    "038 Se esperaba una llamada a función de archivo\n",
    "039 Se alcanzo el fin del archivo inesperadamente\n",
    "040 No se puede registra la existencias del archivo. Si recibe este error por favor contacte a los desarrolladores de ARMUS \n",
    "041 La definicion ya estaba aquí\n",
    "042 Se esperaba paréntesis de cierre\n",
    "043 Se esperaba un factor\n",
    "044 Se esperaba una llamada al sistema\n",
    "045 Se esperaba una punto\n",
    "046 Se esperaba la palabra reservada Sistema\n",
    "047 Se esperaba la palabra reservada mientras\n",
    "048 Se esperada la palabra reservada romper\n",
    "049 Se esperaba una llamada a un metodo de arreglo\n",
    "050 Se esperaban dos puntos (:)\n",
    "051 Se esperaba la palabra reservada caso\n",
    "052 Se esperaba la palabra reservada probar\n",
    "053 Se esperaba la palabra reservada para\n",
    "054 Se esperaba la palabra reservada paraCada\n",
    "055 Se está inentando utilizar un metodo o propiedad en una varible primtiva\n",
    "056 Error fatal: se intenta definir una clase privado o escribir un metodo o atributo fuera de una clase \n",
    "057 Se esperaba una expresión\n"



};
int primerError = 0;
char **erroresEncontrados = NULL;
char *archivoActual;

void log_error(int error) {
    if (error != -1) {
        FILE *f;
        if (primerError == 0) {
            f = fopen("log.txt", "a");
        } else {
            f = fopen("log.txt", "a");
        }
        if (f == NULL) {
            printf("Verifique los permisos sobre el directorio\n");
            return;
        }
        printf("Rerservando memoria para guardar error\n");
        /*char ** erroresEncontradosAux = realloc(erroresEncontrados, primerError + 2);
        printf("RerservandA\n");
        erroresEncontradosAux[primerError] = malloc(strlen(errores[error]) + 3 + sizeof (int)*2 + strlen(archivoActual));
        printf("Rerservar texto\n");
        sprintf(erroresEncontradosAux[primerError], "%s,%d,%d,%s\n", archivoActual, LineaActual, PosicionLineaActual, errores[error]);
        primerError++;
        erroresEncontradosAux[primerError] = malloc(1);
        erroresEncontradosAux[primerError][0] = '\0';

        erroresEncontrados = erroresEncontradosAux;
*/
        primerError++;
        fprintf(f, "%s,%d,%d,%s", archivoActual, LineaActual, PosicionLineaActual, errores[error]);

        fclose(f);
        printf("%d,%d,%s", LineaActual, PosicionLineaActual, errores[error]);
        // exit(0);
    }
}
