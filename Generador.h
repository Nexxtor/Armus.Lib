/* 
 * File:   Generador.c
 * Author: nextor
 *
 * Created on 5 de diciembre de 2016, 10:58 AM
 */

#ifndef GENERADOR_C
#define GENERADOR_C

#include "Parametros.h"


#ifdef __cplusplus
extern "C" {
#endif

    /* Estructura para escribir codigo */

    enum nemonico {
        INS, INO, ALM, OPR, SAL, SAC, LIT, CAR, CRR
    };

    extern char *nM[9];

    union Data {
        double doble;
        char *cadena;
        int entero, booleano, byte;
        char caracter;
    };

    struct ins {
        enum nemonico f;
        int n, d, p;
        union Data data;
    };

    struct lsCod {
        struct ins instruccion;
        struct lsCod *sig;
    };

    struct groupCode {
        int groupNum;
        struct lsCod value;
        struct groupCode *sig;
    };

    struct claseCod {
        int hash;
        struct lsCod atributos; // Codigo para instanciar los atributos
        struct groupCode metodos; //Codigo que se encutara en llamada a metodos
    };

    struct cod {
        struct claseCod clase;
        struct cod *sig;
    };

    extern struct cod codigo; //aqui va estar guardado el codigo
    extern int numeroLinea;



    struct ins genIns(enum nemonico f, int n, int d, int p);
    struct ins genInsL(enum nemonico f, int n, void * d, int p);
    void addLsCode(struct ins codigo, struct lsCod *destino);
    void addGroupCode(struct lsCod lscod, struct groupCode *grupo);
    void addClase(struct claseCod clase, struct cod *cod);
    void mostrar_clase(struct claseCod clase);
    void mostrar_groupCode(struct groupCode grupo);
    void mostrar_lsCode(struct lsCod ls);

#ifdef __cplusplus
}
#endif

#endif /* GENERADOR_C */

