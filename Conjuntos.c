#include "Conjuntos.h"
#include "Parametros.h"
#include "Lexico.h"
#include "Errores.h"
#include "Standar.h"

//conjunto de sincronización de arranque
int set_arranque[NOTOKENS];
int set_clases[NOTOKENS];
int set_ins[NOTOKENS];

void inicializa_conjuntos() {
    //creación de conjuntos de tokens iniciales
    init_set(set_arranque);
    set_arranque[incluirTok] = set_arranque[publicaTok] =set_arranque[localTok] = 1;
    init_set(set_clases);
    set_clases[claseTok] = set_clases[publicaTok] = set_clases[localTok] = 1;
    init_set(set_ins);
    set_ins[ident] = set_ins[romperTok] = set_ins[retornarTok] = set_ins[agregarTok]  =1;
    set_ins[obtenerTok] =  set_ins[quitarTok] = set_ins[cuantosTok] = set_ins[siTok] = 1;
    set_ins[probarTok] = set_ins[mientrasTok] = set_ins[paraTok] = set_ins[hacerTok] =1;
    set_ins[paracadaTok] = set_ins[sistemaTok] = 1;
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
   // printf("TEST\n");
    int conj_union[NOTOKENS];
    
    log_error(n); //se marca el error
    if (token != -1 && conjunto1[token] == 0) {
        //el token no está en el conjunto1
        
        //se arma un conjunto de estabilización

        union_set(conj_union, conjunto1, conjunto2);
        //se salta texto de manera "inteligente"
        while (token != -1 && conj_union[token] == 0 ){
            
            obtoken();
           // printf("avanzado %d \n" , token);
        }
    }
}