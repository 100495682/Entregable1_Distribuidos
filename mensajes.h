//
// Created by alejo on 11/3/26.
//
#ifndef MENSAJES_H
#define MENSAJES_H

#include "claves.h"

#define SERVER_QUEUE "/cola_servidor"
#define MAX_QUEUE_NAME 64
#define MAX_STRING 256
#define MAX_VECTOR 32

typedef enum {
    OP_DESTROY = 1,
    OP_SET_VALUE,
    OP_GET_VALUE,
    OP_MODIFY_VALUE,
    OP_DELETE_KEY,
    OP_EXIST
} operacion_t;

typedef struct {
    operacion_t operacion;
    char client_queue[MAX_QUEUE_NAME];

    char key[MAX_STRING];
    char value1[MAX_STRING];

    int N_value2;
    float V_value2[MAX_VECTOR];
    struct Paquete value3;
} request_t;

typedef struct {
    int status;

    char value1[MAX_STRING];
    int N_value2;
    float V_value2[MAX_VECTOR];
    struct Paquete value3;
} response_t;

#endif