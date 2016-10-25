/* 
 * File:   tds.h
 * Author: nextor
 *
 * Created on 21 de octubre de 2016, 10:20 PM
 */

#ifndef TDS_H
#define TDS_H

#ifdef __cplusplus
extern "C" {
#endif
#define TRUE 1
#define FALSE 0

    enum tipoDato {
        VACIO, ENTERO, REAL, CADENA, CARACTER, BOOLEANO, OBJETO, ARREGLO, ARCHIVO, BYTE
    };

    // los comentarios con * es por que realmente no se a tomado la decision
    // de usarlo o no 

    //Creo que se deberia nombrar como variable

    struct atributo {
        int esPrimitivo;
        enum tipoDato tipoContenidoArreglo; // Si es otro arreglo valor apuntara a otro atributo
        char *ident;
        void *valor;
        enum tipoDato tipo;
        int hashClase; //para saber de que clase es  *
        int hash; //para diferencias instacias  *
        int parametro;
        int esPorReferencia;
        int alcanze;
    };

    struct listaAtributo {
        struct atributo *atributo;
        struct listaAtributo *sig;
    };

    struct metodo {
        char *ident;
        struct listaAtributo *parametros; // se reutiliza listaAtributo pues es justo lo que se quiere guardar
        enum tipoDato tipoRetorno;
        int esFuncion;
    };

    struct listaMetodo {
        struct metodo *metodo;
        struct listaMetodo *sig;
    };

    struct clase {
        struct listaMetodo *lsMetodo;
        struct listaAtributo *lsAtributo;
        int esLocal;
        char *ident;
        int esClasePrincipal;
        int hash; //id Unico de la clase *
    };

    struct listaClase {
        struct clase *clase;
        struct listaClase *sig;
    };

    struct nodoArchivo {
        struct listaClase *lsClase;
        char **incluidos;
        char *nombre;
    };

    struct arbolArchivo {
        struct nodoArchivo *valor;
        struct arbolArchivo *izq;
        struct arbolArchivo *dch;
    };

    typedef struct arbolArchivo tds;

    extern tds tabla;
    extern int hash;


    void instarArchivoTDS(char *nombreArchivo, tds *t, struct nodoArchivo **arch);
    void instarIncluidosArchivo(char *incluido, struct nodoArchivo *miArchivo);
    void insertarTDSClase(struct nodoArchivo *archivo, char * nombre, int alcanze, struct clase **clase);
    void insertarTDSAtributo(struct clase *clase, struct atributo *atributo);
    void insertarTDSMetodo(struct clase *clase, struct metodo *metodo);
    void buscarArchivoTDS(struct nodoArchivo ** valor, tds *tabla, char *);
    int evitarRedefinicionClase(char *lex, struct nodoArchivo *miArchivo, tds *tabla);
    void obtenerClase(struct nodoArchivo *miArchivo, struct clase ** clase,char *lex);
    int puedoUsarEsteTipo(char *lex,struct nodoArchivo *miArchivo, struct clase *clase,tds *tabla);
    void buscarAtributo(struct atributo **atr, struct clase *clase,char *buscado);
    void buscarMetodo(struct metodo **metodo,struct clase* clase, char * bucada);
#ifdef __cplusplus
}
#endif

#endif /* TDS_H */

