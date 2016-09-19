#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Standar.h"
#include "Lexico.h"
#include "Errores.h"

FILE *fp = NULL;
char linea[MAX_LINEA];     //buffer de líneas 
int ll;                   //contador de caracteres
int offset;               //corrimiento en la lectura de los caracteres del programa fuente
int fin_de_archivo;       //bandera de fin de archivo (obtch)   
int ch;                   //último caracter leído
char lex[MAX_ID+1];        //último lexeme leído ( +1 para colocar "\0")
long int valor ;          //valor numérico de una lexeme correspondiene a un número


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

//obtoken: obtiene el siguiente token del programa fuente                   
void obtoken()
{
 char lexid[MAX_ID+1]; //+1 para colocar el marcador "\0"
 int i,j;
 int ok=0;

 //quitar blancos, caracter de cambio de línea y tabuladores
 while (ch==' ' || ch=='\n' || ch=='\t') ch=obtch() ;

 //si la lexeme comienza con una letra, es identificador o palabra reservada
 if (isalpha(ch)) {
    lexid[0]=ch;
    i=1;
    while ( isalpha( (ch=obtch()) ) ||  isdigit(ch)   ) 
      if (i<MAX_ID) lexid[i++]=ch;
    lexid[i]='\0';
  
    //¿es identificador o palabra reservada?.buscar en la tabla de palabras reservadas
	//una búsqueda lineal que tendrá que ser sustituída por otro tipo de búsqueda más efectiva. 
	//...en esa nueva búsqueda desaparecerá el "break"
    for (j=0;j<MAXPAL;++j) 
        if (strcmp(lexid,lexpal[j])==0) {
	       ok=1;
	       break;
        }

    if (ok==1) 
       token=tokpal[j]; //es palabra reservada
    else
       token=ident; //es identificador
 
    strcpy(lex,lexid); //copiar en lex
 }
 else //si comienza con un dígito...
    if (isdigit(ch)) {
       lexid[0]=ch;
       i=j=1;
       while ( isdigit( (ch=obtch()))) {
	         if (i<MAX_DIGIT) lexid[i++]=ch;
	         j++;
       }
       lexid[i]='\0';
       if (j>MAX_DIGIT)
          log_error(30); //este número es demasiado grande
       token=numero;
       valor=atol(lexid); //valor numérico de una lexeme correspondiene a un número	        
    }
    else //reconocimiento de símbolos especiales, incluyendo pares de simbolos (aplicamos "lookahead symbol technique")
       if (ch=='<') {
          ch=obtch();
          if (ch=='=') {
             token=mei;
             ch=obtch();
          }
          else
             if (ch=='>') {
                token=nig;
                ch=obtch();
             }
             else
                token=mnr;
       }
       else
          if (ch=='>') {
             ch=obtch();
             if (ch=='=') {
                token=mai;
                ch=obtch();
             }
             else 
                token=myr;
          }
          else 
             if (ch==':') {
                ch=obtch();
                if (ch=='=') {
	               token=asignacion;
	               ch=obtch();
                }
               else
	               token=nulo;
             }
             else {
                token=espec[ch]; //hashing directo en la tabla de tokens de símbolos especiales del lenguaje
                ch=obtch();
             }
}

//obtch: obtiene el siguiente caracter del programa fuente
int obtch()
{ 

 if (fin_de_archivo==1) {
	fclose(fp);//cerrar el programa fuente
    printf("Analisis lexicografico finalizado.");
    exit(1); //salir...
 }
  
 if (offset==ll-1) {
    ll=getLine(linea,MAX_LINEA); //trae una línea del programa fuente al buffer de líneas
    if (ll==0) 	   
       fin_de_archivo=1; //se retrasa en un blanco la deteccion de EOF, porque obtoken lleva un caracter adelantado. si no, en 
						 //algunos casos tendríamos problemas, por ejemplo: no se reconoceria el punto final del programa (...end.)

    printf("\n%s",linea);
    offset=-1;
 }

 ++offset;

 if ( (linea[offset]=='\0') || (fin_de_archivo==1) )   
    return(' '); 
 else  
    return(toupper(linea[offset])); //de esto depende si el lenguaje es sensitivo de mayúsculas o no.

}
