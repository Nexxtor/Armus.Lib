#include <stdio.h>
#include <stdlib.h>
#include "Standar.h"
#include "Lexico.h"
//getline: lee la siguiente línea del fuente y regresa su tamaño (incluyendo '\n') o 0 si EOF. (por ejemplo: para VAR regresa 4)
//es probablemente la rutina de más bajo nivel del compilador
//tomada de "El Lenguaje de programación C" - Kernighan & Ritchie - pag 28                        

int getLine(char s[], int lim) {
    int c, i;

    for (i = 0; i < lim - 1 && (c = getc(fp)) != EOF && c != '\n'; ++i)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';
    return (i);
}

//inicializar_espec
//construcción de la tabla de tokens para operadores y símboles especiales de pl0
void inicializar_espec()
{
  int i;
  for (i=0;i<=254;++i) 
    espec[i]=nulo;

  espec[43]=mas;
  espec[45]=menos;
  espec[42]=por;
  espec[47]=barra;
  espec[40]=parentI;
  espec[41]=parentF;
  espec['{']=llaveI;
  espec['}']=llaveF;
  espec['[']=corcheteI;
  espec[']']=corcheteF;
  espec['\'']=caracterTok;
  espec['\"']=cadenaTok;
  espec['!']=negacion;
  espec['~']=referencia;
  espec['&']=ytok;
  espec['|']=otok;
  espec['<']=mnr;
  espec[61]=igl;
  espec[46]=punto;
  espec[44]=coma;
  espec[59]=puntoycoma;
}

//imprime_token: transforma de enumerado a string. no aparecerá más en el compilador
void imprime_token()
{
/*

 char *token_string[]={"nulo","ident","numero","mas","menos","por","barra","oddtok","igl","nig","mnr","mei","myr",
	                   "mai","parena","parenc","coma","puntoycoma","punto","asignacion","begintok","endtok","iftok",
					   "thentok","whiletok","dotok","calltok","consttok","vartok","proctok"};
 printf("(%10s) \n",token_string[token]);
  
*/
}