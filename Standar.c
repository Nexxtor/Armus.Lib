#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Standar.h"
#include "Lexico.h"
#include "Errores.h"
#include "Scanner.h"


char *linea; //buffer de líneas 
int ll = 0; //contador de caracteres
int LineaActual = 0; //contador de linea actual del codigo fuente
int PosicionLineaActual = 0; //posicion en la linea actual del codigo fuente 
int offset = 0; //corrimiento en la lectura de los caracteres del programa fuente
int fin_de_archivo = 0; //bandera de fin de archivo (obtch)   
int ch = 0; //último caracter leído
char *lex; //último lexeme leído ( +1 para colocar "\0")
long int valor = 0; //valor numérico de una lexeme correspondiene a un número
double valorDoble = 0;
char valorCaracter = 'c';
char *valorCadena;
int simbolo = 0;
FILE* tokenList = NULL;


//getline: lee la siguiente línea del fuente y regresa su tamaño (incluyendo '\n') o 0 si EOF.
//(por ejemplo: para VAR regresa 4)
//es probablemente la rutina de más bajo nivel del compilador
//tomada de "El Lenguaje de programación C" - Kernighan & Ritchie - pag 28                        

int getLine(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getc(fp)) != EOF && c != '\n'; ++i)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
        ++LineaActual; //SE INCREMENTA LA LINEA ACTUAL DEL CODIGO FUENTE DESPUES DE UN ENTER
        PosicionLineaActual = 0; //SE REINICIA EL INDICADOR DE EL CARACTER ACTUAL
    }

    s[i] = '\0';
    return (i);
}

//inicializar_espec
//construcción de la tabla de tokens para operadores y símboles especiales de pl0

void inicializar_espec() {
    int i;
    for (i = 0; i <= 254; ++i)
        espec[i] = nulo;

    espec[43] = mas;
    espec[45] = menos;
    espec[42] = por;
    espec[47] = barra;
    espec[40] = parentI;
    espec[41] = parentF;
    espec['{'] = llaveI;
    espec['}'] = llaveF;
    espec['['] = corcheteI;
    espec[']'] = corcheteF;
    espec['!'] = negacion;
    espec['~'] = referencia;
    espec[':'] = dosPuntos;
    espec[46] = punto;
    espec[44] = coma;
    espec[59] = puntoycoma;
}

//imprime_token: transforma de enumerado a string. no aparecerá más en el compilador

void imprime_token() {
    char *token_string[] = {"nulo", "comentario", "numeroReal", "numeroEntero", "ident", "mas", "menos", "por", "barra", "llaveI",
        "llaveF", "parentI", "parentF", "corcheteI", "corcheteF", "punto", "coma", "puntoycoma",
        "asignacion", "mei", "mai", "myr", "mnr", "igl", "nig", "negacion", "ytok", "otok", "referencia",
        "enteroTok", "byteTok", "realTok", "vacioTok", "booleanoTok", "cadenaTok", "caracterTok", "objetoTok",
        "archivoTok", "siTok", "sinoTok", "probarTok", "casoTok", "defectoTok", "romperTok",
        "mientrasTok", "paraTok", "hacerTok", "paracadaTok", "sistemaTok", "obtenerEnteroTok",
        "obtenerRealTok", "obtenerCadenaTok", "obtenerCaracterTok", "mostrarTok", "publicaTok",
        "privadaTok", "retornarTok", "arregloTok", "agregarTok", "obtenerTok", "cuantosTok",
        "quitarTok", "abrirTok", "leerLineaTok", "volcadoTok", "cerrarTok", "concatenarTok",
        "parteEnteraTok", "compararTok", "mayorTok", "menorTok", "esParTok", "decimalBinTok",
        "potenciaTok", "absolutoTok", "moduloTok", "longitudCadenaTok", "claseTok", "incluirTok",
        "obtenerBooleanoTok", "falsoTok", "verdaderoTok", "datoCadena", "datoCaracter", "localTok"};

    if (token != comentario) {
        switch (token) {
            case numeroEntero: fprintf(tokenList, "%ld", valor);
                break;
            case numeroReal: fprintf(tokenList, "%f", valorDoble);
                break;
            case datoCaracter: fprintf(tokenList, "%c", valorCaracter);
                break;
            case datoCadena: fprintf(tokenList, "%s", valorCadena);
                break;
            case ytok: fprintf(tokenList, "&&");
                break;
            case otok: fprintf(tokenList, "||");
                break;
            case igl: fprintf(tokenList, "==");
                break;
            case nig: fprintf(tokenList, "<>");
                break;
            case asignacion: fprintf(tokenList, "=");
                break;
            case mas: fprintf(tokenList, "+");
                break;
            case menos: fprintf(tokenList, "-");
                break;
            case por: fprintf(tokenList, "*");
                break;
            case barra: fprintf(tokenList, "/");
                break;
            case llaveI: fprintf(tokenList, "{");
                break;
            case llaveF: fprintf(tokenList, "}");
                break;
            case parentI: fprintf(tokenList, "(");
                break;
            case parentF: fprintf(tokenList, ")");
                break;
            case corcheteI: fprintf(tokenList, "[");
                break;
            case corcheteF: fprintf(tokenList, "]");
                break;
            case punto: fprintf(tokenList, ".");
                break;
            case coma: fprintf(tokenList, ",");
                break;
            case puntoycoma: fprintf(tokenList, ";");
                break;
            case mai: fprintf(tokenList, ">=");
                break;
            case mei: fprintf(tokenList, "<=");
                break;
            case negacion: fprintf(tokenList, "!");
                break;
            case myr: fprintf(tokenList, ">");
                break;
            case mnr: fprintf(tokenList, "<");
                break;
            case referencia: fprintf(tokenList, "~");
                break;
            default: fprintf(tokenList, "%s", lex);
        }
        /*
                if (simbolo > 0) {
                    fprintf(tokenList, "%s", token_string[token]);
                } else {
                    fprintf(tokenList, "%s", lex);
                }
         */
        fprintf(tokenList, " => %s\n", token_string[token]);
    }
}

//obtoken: obtiene el siguiente token del programa fuente                   

void obtoken() {
    char lexid[MAX_ID + 1]; //+1 para colocar el marcador "\0"
    int i, j;
    simbolo = 0;
    //quitar blancos, caracter de cambio de línea y tabuladores
    while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') ch = obtch();

    if (ch == '/') {
        ch = obtch();
        if (ch == '/') {
            ch = obtch();
            while (ch != '\n') {
                ch = obtch();
            }
            ch = obtch();
            obtoken();
            return;
        } else {
            if (ch == '*') {
                ch = obtch();
                char c = ' ';
                while (1) {
                    if (c == '*' && ch == '/') {
                        break;
                    } else {
                        c = ch;
                    }
                    ch = obtch();

                    if (fin_de_archivo == 1) {
                        log_error(11); //mal cometario de bloque
                        token = nulo;
                        return;
                    }
                }
                ch = obtch();
                obtoken();
                return;
            } else {
                token = espec['/'];
                return;
            }
        }
    }

    //quitar blancos, caracter de cambio de línea y tabuladores
    //while (ch == ' ' || ch == '\n' || ch == '\t') ch = obtch();

    if (ch == '\0') {
        printf("SE termino el archivo\n");
        token = -1;
        return;
    } //hay que cambiar dearchivo

    //si la lexeme comienza con una letra, es identificador o palabra reservada
    if ((isalpha(ch) || ch == '_') && ch != '#') {
        lexid[0] = ch;
        i = 1;
        while (isalpha((ch = obtch())) || isdigit(ch) || ch == '_')
            if (i < MAX_ID) lexid[i++] = ch;
        lexid[i] = '\0';

        //¿es identificador o palabra reservada?.buscar en arbol de palabras reservadas
        token = buscarToken(lexid);

        strcpy(lex, lexid); //copiar en lex
    } else //si comienza con un dígito...
        if (isdigit(ch) || ch == '#') {
        i = j = 0;
        int hexa = 0;
        if (ch != '#') {
            lexid[i] = ch;
            i++;
        } else {
            hexa = 1;
        }
        int puntoDecimal = 0;
        while (isdigit((ch = obtch()))
                || (ch == '.' && hexa == 0 && puntoDecimal <1)
                || ((ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E'
                || ch == 'F') && hexa == 1)) {
            printf("Leyendo numero\n");
            if (i < MAX_DIGIT) lexid[i++] = ch;
            if (ch == '.') puntoDecimal++;
            j++;
        }
        lexid[i] = '\0';

        if (j > MAX_DIGIT)
            log_error(7); //este número es demasiado grande
        /*if (puntoDecimal > 1)
            log_error(8); //tiene mas de un punto decimal*/
        if (hexa == 0 && (ch == 'A' || ch == 'B' || ch == 'C'
                || ch == 'D' || ch == 'E' || ch == 'F'))
            log_error(9); //esta trantado de definir un hex sin #
        if (puntoDecimal == 1) {
            token = numeroReal;
            valorDoble = atof(lexid);
        } else {
            token = numeroEntero;
            valor = atol(lexid);
        }
        // printf("%s\n", lexid);
        if (hexa == 1) {
            valor = strtol(lexid, NULL, 16);
        } else {
            if (puntoDecimal == 1) {
                valorDoble = atof(lexid); //valor numérico de una lexeme correspondiene a un número	        
            } else {
                valor = atol(lexid); //valor numérico de una lexeme correspondiene a un número	        
            }

        }
    } else //reconocimiento de símbolos especiales, incluyendo pares de simbolos (aplicamos "lookahead symbol technique")
        if (ch == '<') {
        ch = obtch();
        if (ch == '=') {
            token = mei;
            ch = obtch();
        } else {
            if (ch == '>') {
                token = nig;
                ch = obtch();
            } else {
                token = mnr;
            }
        }
    } else {
        if (ch == '>') {
            ch = obtch();
            if (ch == '=') {
                token = mai;
                ch = obtch();
            } else {
                token = myr;
            }
        } else
            if (ch == '=') {
            ch = obtch();
            if (ch == '=') {
                token = igl;
                ch = obtch();
            } else {
                token = asignacion;
            }
        } else {
            if (ch == '&') {
                ch = obtch();
                if (ch == '&') {
                    token = ytok;
                    ch = obtch();
                } else {
                    token = nulo;
                }
            } else {
                if (ch == '|') {
                    ch = obtch();
                    if (ch == '|') {
                        token = otok;
                        ch = obtch();
                    } else {
                        token = nulo;
                    }
                } else {
                    if (ch == '\"') {
                        i = 0;
                        //hay que considerar que el ;  es malo antes de "
                        while ((ch = obtch()) != '\"' && (fin_de_archivo == 0)) {
                            if (i < MAX_CADENA) valorCadena[i++] = ch;

                        }
                        valorCadena[i] = '\0';

                        /*printf("Valor cadena = %s\n", valorCadena);
                        printf("\n fin de archiv0 %d", fin_de_archivo);
                        printf("caracter ultimo %c valor entero %d\n",ch,ch);*/

                        if (i > MAX_CADENA)
                            log_error(10); //se paso del tamaño de la cadena 

                        if (fin_de_archivo == 1) {
                            printf("en cadena");
                            log_error(11); //se acano el archivo
                            token = nulo;
                            return;
                        } else {
                            token = datoCadena;

                        }
                        ch = obtch();

                    } else {
                        if (ch == '\'') {
                            ch = obtch();
                            if (ch == '\\') {
                                ch = obtch();
                            }
                            if (ch == '\n' || fin_de_archivo == 1) {
                                log_error(11); //se esperaba un caracter
                                printf("en caracter");
                                token = nulo;
                                return;
                            }
                            int largo = 0;
                            valorCaracter = ch;
                            while ((ch = obtch()) != '\'' && fin_de_archivo != 1) {
                                largo++;
                            }

                            if (largo == 0 && ch == '\'') {
                                token = datoCaracter;
                            } else {
                                log_error(12); //esta inentano escribir una cadena o se acabo el archivo
                                token = nulo;
                                return;
                            }

                            ch = obtch();
                        } else {
                            if (ch == '!') {
                                ch = obtch();
                                if (ch == '=') {
                                    token = nig;
                                    ch = obtch();
                                } else {
                                    token = negacion;
                                }
                            } else {

                                token = espec[ch]; //hashing directo en la tabla de tokens de símbolos especiales del lenguaje
                                simbolo = ch;
                                ch = obtch();


                            }
                            /*if (ch == '/') {
                                ch = obtch();
                                if (ch == '/') {
                                    while ((ch = obtch()) != '\n') printf("%c",ch);
                                    printf("------ FIn comentario -----\n");
                                    token = comentario;
                                        //quitar blancos, caracter de cambio de línea y tabuladores
                                    while (ch == ' ' || ch == '\n' || ch == '\t') ch = obtch();
                                   // ch = obtch();
                                }else {
                                     token = espec['/'];
                                }
                            }else{
                                token = espec[ch]; //hashing directo en la tabla de tokens de símbolos especiales del lenguaje
                                ch = obtch();
                            }*/
                            /* token = espec[ch]; //hashing directo en la tabla de tokens de símbolos especiales del lenguaje
                                ch = obtch();*/
                        }
                    }

                }
            }

        }
    }
}

//obtch: obtiene el siguiente caracter del programa fuente

int obtch() {

    if (fin_de_archivo == 1) {
        //  fclose(fp); //cerrar el programa fuente
        //printf("Analisis lexicografico finalizado.");
        return '\0';
        //exit(1); //salir...
    }

    if (offset == ll - 1) {
        ll = getLine(linea, MAX_LINEA); //trae una línea del programa fuente al buffer de líneas
        if (ll == 0)
            fin_de_archivo = 1; //se retrasa en un blanco la deteccion de EOF, porque obtoken lleva un caracter adelantado. si no, en 
        //algunos casos tendríamos problemas, por ejemplo: no se reconoceria el punto final del programa (...end.)

        // printf("\n%s", linea);
        offset = -1;
    }

    ++offset;

    if ((linea[offset] == '\0') || (fin_de_archivo == 1))
        return (' ');
    else {
        PosicionLineaActual++;
        return (linea[offset]); //de esto depende si el lenguaje es sensitivo de mayúsculas o no.
    }
}

enum simbolo buscarToken(char * buscado) {
    arbol *s = lexpal;

    while (s != NULL) {
        if (strcmp(s->value, buscado) == 0) {
            //printf("buscado %s valor token = %d\n",buscado, s->token);
            return s->token;
        }
        if (strcmp(buscado, s->value) > 0) {
            s = s->derecha;
        } else {
            s = s->izq;
        }
    }

    return ident;


}
