#ifndef ERRORES_H
#define ERRORES_H

#ifdef __cplusplus
extern "C" {
#endif
    char *errores[4] = {
        "Error el archivo no pudo ser abierto","","","",""
    };
    void log_error(int error);
    #include "Errores.c"
#ifdef __cplusplus
}
#endif

#endif /* ERRORES_H */
