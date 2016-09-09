/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Standar.h
 * Author: root
 *
 * Created on 9 de septiembre de 2016, 04:09 PM
 */

#ifndef STANDAR_H
#define STANDAR_H

#ifdef __cplusplus
extern "C" {
#endif
    #define MAX_LINEA 1024
    char linea[MAX_LINEA];
    int getline(char s[], int lim);

    #include "Standar.c"

#ifdef __cplusplus
}
#endif

#endif /* STANDAR_H */

