//
// Created by alejo on 11/3/26.
//

#include <stdlib.h>
#include <string.h>
#include "storage.h"

Nodo *head = NULL;

Nodo *buscar_nodo(const char *key) {
    Nodo *actual = head;

    while (actual != NULL) {
        if (strcmp(actual->key, key) == 0) {
            return actual;
        }
        actual = actual->next;
    }

    return NULL;
}

int insertar_nodo(const char *key, const char *value1, int N_value2, const float *V_value2, struct Paquete value3) {
    Nodo *nuevo;
    int i;

    if (key == NULL || value1 == NULL || V_value2 == NULL) {
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    if (buscar_nodo(key) != NULL) {
        return -1;
    }

    nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        return -1;
    }

    strncpy(nuevo->key, key, 255);
    nuevo->key[255] = '\0';

    strncpy(nuevo->value1, value1, 255);
    nuevo->value1[255] = '\0';

    nuevo->N_value2 = N_value2;

    for (i = 0; i < N_value2; i++) {
        nuevo->V_value2[i] = V_value2[i];
    }

    nuevo->value3 = value3;
    nuevo->next = head;
    head = nuevo;

    return 0;
}

int eliminar_nodo(const char *key) {
    Nodo *actual = head;
    Nodo *anterior = NULL;

    if (key == NULL) {
        return -1;
    }

    while (actual != NULL) {
        if (strcmp(actual->key, key) == 0) {
            if (anterior == NULL) {
                head = actual->next;
            } else {
                anterior->next = actual->next;
            }

            free(actual);
            return 0;
        }

        anterior = actual;
        actual = actual->next;
    }

    return -1;
}

void liberar_todo(void) {
    Nodo *actual = head;
    Nodo *siguiente;

    while (actual != NULL) {
        siguiente = actual->next;
        free(actual);
        actual = siguiente;
    }

    head = NULL;
}