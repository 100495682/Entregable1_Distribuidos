//
// Created by alejo on 11/3/26.
//
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "claves.h"
#include "mensajes.h"

static int enviar_peticion_y_recibir(const request_t *req, response_t *res) {
    mqd_t server_queue;
    mqd_t client_queue;
    struct mq_attr attr;
    char client_queue_name[MAX_QUEUE_NAME];
    pid_t pid;
    request_t request_local;

    if (req == NULL || res == NULL) {
        return -2;
    }

    pid = getpid();
    snprintf(client_queue_name, sizeof(client_queue_name), "/cola_cliente_%d", pid);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(response_t);
    attr.mq_curmsgs = 0;

    mq_unlink(client_queue_name);

    client_queue = mq_open(client_queue_name, O_CREAT | O_RDONLY, 0660, &attr);
    if (client_queue == (mqd_t)-1) {
        return -2;
    }

    server_queue = mq_open(SERVER_QUEUE, O_WRONLY);
    if (server_queue == (mqd_t)-1) {
        mq_close(client_queue);
        mq_unlink(client_queue_name);
        return -2;
    }

    memset(&request_local, 0, sizeof(request_t));
    request_local = *req;
    strncpy(request_local.client_queue, client_queue_name, MAX_QUEUE_NAME - 1);
    request_local.client_queue[MAX_QUEUE_NAME - 1] = '\0';

    if (mq_send(server_queue, (const char *)&request_local, sizeof(request_t), 0) == -1) {
        mq_close(server_queue);
        mq_close(client_queue);
        mq_unlink(client_queue_name);
        return -2;
    }

    if (mq_receive(client_queue, (char *)res, sizeof(response_t), NULL) == -1) {
        mq_close(server_queue);
        mq_close(client_queue);
        mq_unlink(client_queue_name);
        return -2;
    }

    mq_close(server_queue);
    mq_close(client_queue);
    mq_unlink(client_queue_name);

    return 0;
}

int destroy(void) {
    request_t req;
    response_t res;
    int comm;

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_DESTROY;

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    return res.status;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    request_t req;
    response_t res;
    int i;
    int comm;

    if (key == NULL || value1 == NULL || V_value2 == NULL) {
        return -1;
    }

    if (strlen(key) > 255 || strlen(value1) > 255) {
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_SET_VALUE;

    strncpy(req.key, key, MAX_STRING - 1);
    req.key[MAX_STRING - 1] = '\0';

    strncpy(req.value1, value1, MAX_STRING - 1);
    req.value1[MAX_STRING - 1] = '\0';

    req.N_value2 = N_value2;
    for (i = 0; i < N_value2; i++) {
        req.V_value2[i] = V_value2[i];
    }
    req.value3 = value3;

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    return res.status;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    request_t req;
    response_t res;
    int i;
    int comm;

    if (key == NULL || value1 == NULL || N_value2 == NULL || V_value2 == NULL || value3 == NULL) {
        return -1;
    }

    if (strlen(key) > 255) {
        return -1;
    }

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_GET_VALUE;

    strncpy(req.key, key, MAX_STRING - 1);
    req.key[MAX_STRING - 1] = '\0';

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    if (res.status != 0) {
        return res.status;
    }

    strcpy(value1, res.value1);
    *N_value2 = res.N_value2;
    for (i = 0; i < res.N_value2; i++) {
        V_value2[i] = res.V_value2[i];
    }
    *value3 = res.value3;

    return res.status;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    request_t req;
    response_t res;
    int i;
    int comm;

    if (key == NULL || value1 == NULL || V_value2 == NULL) {
        return -1;
    }

    if (strlen(key) > 255 || strlen(value1) > 255) {
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32) {
        return -1;
    }

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_MODIFY_VALUE;

    strncpy(req.key, key, MAX_STRING - 1);
    req.key[MAX_STRING - 1] = '\0';

    strncpy(req.value1, value1, MAX_STRING - 1);
    req.value1[MAX_STRING - 1] = '\0';

    req.N_value2 = N_value2;
    for (i = 0; i < N_value2; i++) {
        req.V_value2[i] = V_value2[i];
    }
    req.value3 = value3;

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    return res.status;
}

int delete_key(char *key) {
    request_t req;
    response_t res;
    int comm;

    if (key == NULL) {
        return -1;
    }

    if (strlen(key) > 255) {
        return -1;
    }

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_DELETE_KEY;

    strncpy(req.key, key, MAX_STRING - 1);
    req.key[MAX_STRING - 1] = '\0';

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    return res.status;
}

int exist(char *key) {
    request_t req;
    response_t res;
    int comm;

    if (key == NULL) {
        return -1;
    }

    if (strlen(key) > 255) {
        return -1;
    }

    memset(&req, 0, sizeof(request_t));
    req.operacion = OP_EXIST;

    strncpy(req.key, key, MAX_STRING - 1);
    req.key[MAX_STRING - 1] = '\0';

    comm = enviar_peticion_y_recibir(&req, &res);
    if (comm == -2) {
        return -2;
    }

    return res.status;
}