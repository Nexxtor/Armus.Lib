#include "Parametros.h"

//conjunto de sincronización de arranque
extern int set_arranque[NOTOKENS]; 
extern int set_clases[NOTOKENS]; //Sirve para estabilizar cuando no escribe el accesor
extern int set_ins[NOTOKENS];

void inicializa_conjuntos(),init_set(int conjunto[]),copia_set(int conjunto1[],int conjunto2[]);
void union_set(int conjunto1[],int conjunto2[],int conjunto3[]),test (int conjunto1[],int conjunto2[],int n);