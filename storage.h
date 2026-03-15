//
// Created by alejo on 11/3/26.
//
#ifndef STORAGE_H
#define STORAGE_H

#include "claves.h"

typedef struct Nodo {
    char key[256];
    char value1[256];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;
    struct Nodo *next;
} Nodo;

/* Cabecera de la lista */
extern Nodo *head;

/* Operaciones auxiliares internas */
Nodo *buscar_nodo(const char *key);
int insertar_nodo(const char *key, const char *value1, int N_value2, const float *V_value2, struct Paquete value3);
int eliminar_nodo(const char *key);
void liberar_todo(void);

#endif