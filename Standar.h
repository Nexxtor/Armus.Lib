#include "Parametros.h"
extern char *linea;//[MAX_LINEA];
extern FILE *fp;
extern int ll;               //contador de caracteres
extern int offset;           //corrimiento en la lectura de los caracteres del programa fuente (se usa en scanner.cpp y auxiliares.cpp)
extern int fin_de_archivo;   //bandera de fin de archivo (se usa en pl0.cpp y scanner.cpp)
extern int ch;               //último caracter leído (se usa en pl0.cpp y scanner.cpp)
extern char *lex;//[MAX_ID+1];    //último lexeme leído ( +1 para colocar "\0")
extern long int valor;       //valor numérico de una lexeme correspondiene a un número
extern double valorDoble; //
extern char *valorCadena;
extern char valorCaracter;
extern int simbolo;
extern FILE *tokenList;

int getLine(char s[], int lim);
void obtoken();
int obtch();
void inicializar_espec();
void imprime_token();
enum simbolo buscarToken(char *);
