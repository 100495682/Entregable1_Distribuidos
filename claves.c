//
// Created by alejo on 11/3/26.
//
#include <string.h>
#include <pthread.h>
#include "claves.h"
#include "storage.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int destroy(void) {
    pthread_mutex_lock(&mutex);
    liberar_todo();
    pthread_mutex_unlock(&mutex);
    return 0;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    int resultado;

    if (key == NULL || value1 == NULL || V_value2 == NULL) {
        return -1;
    }

    if (strlen(key) > 255 || strlen(value1) > 255) {
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    pthread_mutex_lock(&mutex);
    resultado = insertar_nodo(key, value1, N_value2, V_value2, value3);
    pthread_mutex_unlock(&mutex);

    return resultado;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    Nodo *nodo;
    int i;

    if (key == NULL || value1 == NULL || N_value2 == NULL || V_value2 == NULL || value3 == NULL) {
        return -1;
    }

    pthread_mutex_lock(&mutex);

    nodo = buscar_nodo(key);
    if (nodo == NULL) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    strcpy(value1, nodo->value1);
    *N_value2 = nodo->N_value2;

    for (i = 0; i < nodo->N_value2; i++) {
        V_value2[i] = nodo->V_value2[i];
    }

    *value3 = nodo->value3;

    pthread_mutex_unlock(&mutex);
    return 0;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    Nodo *nodo;
    int i;

    if (key == NULL || value1 == NULL || V_value2 == NULL) {
        return -1;
    }

    if (strlen(key) > 255 || strlen(value1) > 255) {
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    pthread_mutex_lock(&mutex);

    nodo = buscar_nodo(key);
    if (nodo == NULL) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    strncpy(nodo->value1, value1, 255);
    nodo->value1[255] = '\0';

    nodo->N_value2 = N_value2;
    for (i = 0; i < N_value2; i++) {
        nodo->V_value2[i] = V_value2[i];
    }

    nodo->value3 = value3;

    pthread_mutex_unlock(&mutex);
    return 0;
}

int delete_key(char *key) {
    int resultado;

    if (key == NULL) {
        return -1;
    }

    pthread_mutex_lock(&mutex);
    resultado = eliminar_nodo(key);
    pthread_mutex_unlock(&mutex);

    return resultado;
}

int exist(char *key) {
    Nodo *nodo;

    if (key == NULL) {
        return -1;
    }

    pthread_mutex_lock(&mutex);
    nodo = buscar_nodo(key);
    pthread_mutex_unlock(&mutex);

    if (nodo != NULL) {
        return 1;
    }

    return 0;
}
