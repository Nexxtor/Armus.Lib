#ifndef PARAMETROS_H
//#define PARAMETROS_H
//#define MAX_LINEA 1024
//#define MAX_ID 1024
//#define MAX_DIGIT 20
//#define MAX_FILE 150
//#define MAX_NAME_FILE 256*4
#define MAXPAL 55  //numero de palabras reservadas
//#define MAX_CADENA 123456
//#define MAX_SIZE_FILE 99999


#ifdef __cplusplus
extern "C" {
#endif
    extern int MAX_LINEA;
    extern int MAX_ID;
    extern int MAX_DIGIT;
    extern int MAX_FILE;
    extern int MAX_NAME_FILE;
    extern int MAX_CADENA;
    extern int MAX_SIZE_FILE;

    void iniciarParamentros();
#ifdef __cplusplus
}
#endif

#endif /* PARAMETROS_H */

