/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
    enum tipoDato {ENTERO,REAL,OBJETO};
    
    // los comentarios con * es por que realmente no se a tomado la decision
    // de usarlo o no 
    
    //Creo que se deberia nombrar como variable
    struct atributo{
        int esPrimitivo;
        int esArreglo;
        char *ident;
        void *valor;       
        enum tipoDato tipo;
        int hashClase; //para saber de que clase es  *
        int hash; //para diferencias instacias  *
    };

    struct listaAtributo{
        struct atributo atributo;
        struct listaAtributo *sig;
    };
    
    struct metodo{
        char *ident;
        struct listaAtributo *parametros; // se reutiliza listaAtributo pues es justo lo que se quiere guardar
        enum tipoDato tipoRetorno;
        int esFuncion;
    };
    
    struct listaMetodo{
        struct metodo metodo;
        struct listaMetodo *sig;
    };
    
    struct clase{
        struct listaMetodo lsMetodo;
        struct listaAtributo lsAtributo;
        int esLocal;
        char *ident;
        int esClasePrincipal;
        int hash; //id Unico de la clase *
    };
    
    struct listaClase{
        struct clase clase;
        struct listaClase *sig;
    };
    
    struct nodoArchivo{
        struct listaClase lsClase;
        char **incluidos;
    };
    
    struct arbolArchivo{
        struct nodoArchivo valor;
        struct arbolArchivo *izq;
        struct arbolArchivo *dch;
    };
    
    typedef struct arbolArchivo tds;
    
    extern tds tabla;
#ifdef __cplusplus
}
#endif

#endif /* TDS_H */

