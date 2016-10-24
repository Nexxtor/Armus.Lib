#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tds.h"
#include "Parametros.h"
#include "Errores.h"
#include "Standar.h"
#include "Lexico.h"

tds tabla;
void clearScanner();

JNIEXPORT jobjectArray JNICALL Java_armus_lib_parser_Parser_run
(JNIEnv *env, jobject obj, jobjectArray jsLsFile) {

    //Cantidad de archivos mandados
    int cant = (*env)->GetArrayLength(env, jsLsFile);
    char **lsfiles; // Lista de archivos CONVERTIDAS a c
    void **jstrings;
    //reservado espacio para la conversion 
    lsfiles = (char **) malloc(sizeof (const char *) * cant);
    jstrings = (char **) malloc(sizeof (jstring *) * cant);
    int i;
    //Conversion 
    for (i = 0; i < cant; i++) {
        jstring string = (jstring) ((*env)->GetObjectArrayElement(env, jsLsFile, i));
        const char *file = (*env)->GetStringUTFChars(env, string, 0);

        lsfiles[i] = file;
        jstrings[i] = &string;
        printf("%s\n", lsfiles[i]);
    }

    tabla.izq = NULL;
    tabla.dch = NULL;
    tabla.valor = NULL;

    tds *incioTDS = &tabla;

    pasada1(lsfiles, cant);

    //Relase Java things
    for (i = 0; i < cant; i++) {
        (*env)->ReleaseStringUTFChars(env, *((jstring *) jstrings[i]), lsfiles[i]);
    }

    return NULL;
}

int pasada1(char **lsfiles, int cant) {
    int i;
    clearScanner();
    //Hacerlo con cada archivo
    for (i = 0; i < cant; i++) {
        fp = fopen(lsfiles[i], "r");
        if (fp == NULL) {
            return -1; //panico si no se puede abrir el archivo
        }

        ch = ' ';
        fin_de_archivo = 0;
        offset = -1;
        ll = 0;
        struct nodoArchivo *miArchivo;
        instarArchivoTDS(lsfiles[i],&tabla,&miArchivo);

        printf("Se Inserto %s\n", miArchivo->nombre);
        obtoken();
        programa();
        fclose(fp);
        fp = NULL;
    }
    return 1;
}

int pasada2(char **lsfiles, int cant) {

}

void clearScanner() {
    primerError = 0;
    inicializar_espec();
    inicializarArbolPalabras();
    iniciarParamentros();
}

void programa(struct nodoArchivo archivo){
    do{
        if(token == incluirTok){
            obtoken();
            if(token == datoCadena){
                 obtoken();
                 if(token == puntoycoma){
                     //guardar en la tabla
                     printf("Se va a guardar en la tabla %s \n", valorCadena);
                     obtoken();
                 }else{
                     log_error(2); // falto punto y coma;
                 }
            }else{
                log_error(1); // No esta bien escrita la cadena;
            }
        }
    }while(token == incluirTok);
}

//Expresion Conjuncion
void expresion_conjuncion(){
	expresion_relacional();
	while(token == ytok){
		 obtoken();
		 expresion_racional();
	}
}

//**************************************************************************
//Expresion Relacional

void expresion_relacional(){
	if(token == negacion){
		obtoken();
	expresion_aritmetica();
		if ( (token!=igl) && (token!=nig) && (token!=mnr) && (token!=mei) && (token!=myr) && (token!=mai) ){
       error(50); //error 50: Se esperaba un operador relacional 
			else {
				obtoken();
				expresion_aritmetica(); 
			}
		}	
	}
}	else{
	 expresion_aritmetica();
	if ( (token!=igl) && (token!=nig) && (token!=mnr) && (token!=mei) && (token!=myr) && (token!=mai) )
       error(51); //error 51: Se esperaba un operador relacional 
       else {
            obtoken();
	        expresion_aritmetica(); 
	   }
	}
}

//*************************************************************************
//Expresion Aritmetica

void expresion_aritmetica(){
	termino();
	while (token == mas || token == menos){
		obtoken();
		termino();
	}
}
//**************************************************************************
//Termino
void termino(){
	factor();
	while (token == por || token == barra){
		obtoken();
		factor();
	}
}


//**************************************************************************
void factor(){
	while (token == mas || token == menos){
			obtoken();
	}
	if(token == numeroEntero){
			obtoken();
	}else{
	     if(token == numeroReal){
		 	obtoken();
	   }else{
		   if (token==ident){
			obtoken();
			if(token == punto){
				obtoken();
				/*if(token == ident){
					 obtoken();
				 }else{
					 llamada_Metodo();
				 }
				 */
			}
			
		   }else{
			    if (token==parena) {
					obtoken();
					expresion_numerica();
			    if (token==parenc)
					obtoken();
				else
				//error(52); //error 52: Falta un paréntesis de cierre  
				}else{
					if(token == sistema){
						obtoken();
						if(token == punto){
							obtoken();
							 if(token == longitudCadenaTok || token == compararTok){
								 funcion_numerica_cadena();
							 }else{
								 funcion_numerica();
							 }
						}else{
						//	error(54); //Error 54: funcion numerica debe ser seguido de un punto
						}
					}else{
						//error(53);// error 53: no era ninguna de las anteriores 
					}
				}
			   
		   }  
	   }
	}				
}

//************************************************************************************
//Funcion Numerica Cadena


void funcion_numerica_cadena(){
	if(token == compararTok){
		obtoken();
		if(token == corcheteI){
			obtoken();
			dato_cadena();
			if(token == coma){
				obtoken();
				dato_cadena();
				if(token == corcheteF){
					obtoken();
				}else{
				//	error (58); // Se esperaba corchete de cierre
				}
			}else{
			//	error(56);// Error 56: Se esperaba una coma en la comparacion
			}
		}else{
		//	error(55);//Error 55: se esperaba parentesis de apertura
		}
	}else{
		if(token == longitudCadenaTok){
			obtoken();
			if(token == corcheteI){
			obtoken();
			dato_cadena();
			if(token == corcheteF){
					obtoken();
				}else{
				//	error (58); // Se esperaba corchete de cierre
				}
		}else{
		//	error(55);//Error 55: se esperaba parentesis de apertura
		}
	}
	
	
}

//************************************************************************************
//Funcion Numerica

void funcion_numerica(){
	if(token == parteEnteraTok || token == esParTok || token == decimalBinTok || token == absolutoTok ){
		obtoken();
		if(token == corcheteI){
			obtoken();
			expresion_numerica();
			if(token == corcheteF){
					obtoken();
				}else{
				//	error (58); // Se esperaba corchete de cierre
				}
		}else{
			//error(55);//Error 55: se esperaba parentesis de apertura
		}
}else{
		if(token == mayorTok || token == menorTok || token == potenciaTok || token == moduloTok ){
			obtoken();
			if(token == corcheteI){
			obtoken();
			expresion_numerica();
			if(token == coma){
				obtoken();
				expresion_numerica();
				if(token == corcheteF){
					obtoken();
				}else{
					//error (58); // Se esperaba corchete de cierre
				}
			}else{
			//	error(56);// Error 56: Se esperaba una coma en la comparacion
			}
		}else{
			//error(55);//Error 55: se esperaba parentesis de apertura
		}
	  }else{
		  //error(59); // instruccion no valida
	  }
	}		
			
}

// ***************************************************************************
// Dato Caracter

void dato_caracter(){
	if (token==ident){
			obtoken();
			if(token == punto){
				obtoken();
				/*
				 llamada_Metodo();
				 */
				 }
			
		   }else{
			   dato_caracter();
		   }
}