#include "Conjuntos.h"
#include "Parametros.h"
#include "Lexico.h"
#include "Errores.h"
#include "Standar.h"

//conjunto de sincronización de arranque
int set_arranque[NOTOKENS];
int set_clases[NOTOKENS];

void inicializa_conjuntos() {
    //creación de conjuntos de tokens iniciales
    init_set(set_arranque);
    set_arranque[incluirTok] = set_arranque[publicaTok] =set_arranque[localTok] = 1;
    init_set(set_clases);
    set_clases[claseTok] = set_clases[publicaTok] = set_clases[localTok] = 1;
    
}

//init_set                     
//crear al conjunto vacío
//0 en todas las posiciones del array

void init_set(int conjunto[]) {
    int i;
    for (i = 0; i < NOTOKENS; ++i)
        conjunto[i] = 0;
}

//copia_set
//copia el segundo conjunto en el primero

void copia_set(int conjunto1[], int conjunto2[]) {
    int i;
    for (i = 0; i < NOTOKENS; ++i)
        conjunto1[i] = conjunto2[i];
}

//union_set
//la unión de conjuntos: conjunto1=conjunto2+conjunto3

void union_set(int conjunto1[], int conjunto2[], int conjunto3[]) {
    int i;
    copia_set(conjunto1, conjunto2);
    for (i = 0; i < NOTOKENS; ++i)
        if (conjunto3[i] == 1)
            conjunto1[i] = 1;
}

//test
//rutina de comprobación (test) y posible salto de texto sobre el programa fuente

void test(int conjunto1[], int conjunto2[], int n) {
    int conj_union[NOTOKENS];

    if (conjunto1[token] == 0) {
        //el token no está en el conjunto1
        log_error(n); //se marca el error
        //se arma un conjunto de estabilización
        union_set(conj_union, conjunto1, conjunto2);
        //se salta texto de manera "inteligente"
        while (conj_union[token] == 0)
            obtoken();
    }
}