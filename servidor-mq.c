#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include "claves.h"
#include "mensajes.h"

typedef struct {
    request_t req;
} thread_arg_t;

static void procesar_peticion(request_t *req, response_t *res) {
    memset(res, 0, sizeof(response_t));

    switch (req->operacion) {
        case OP_DESTROY:
            res->status = destroy();
            break;

        case OP_SET_VALUE:
            res->status = set_value(
                req->key,
                req->value1,
                req->N_value2,
                (float *)req->V_value2,
                req->value3
            );
            break;

        case OP_GET_VALUE:
            res->status = get_value(
                req->key,
                res->value1,
                &res->N_value2,
                res->V_value2,
                &res->value3
            );
            break;

        case OP_MODIFY_VALUE:
            res->status = modify_value(
                req->key,
                req->value1,
                req->N_value2,
                (float *)req->V_value2,
                req->value3
            );
            break;

        case OP_DELETE_KEY:
            res->status = delete_key(req->key);
            break;

        case OP_EXIST:
            res->status = exist(req->key);
            break;

        default:
            res->status = -1;
            break;
    }
}

static void *atender_peticion(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    response_t res;
    mqd_t client_queue;

    printf("Hilo atendiendo: operacion=%d, key=%s, client_queue=%s\n",
           targ->req.operacion, targ->req.key, targ->req.client_queue);
    
    procesar_peticion(&targ->req, &res);

    client_queue = mq_open(targ->req.client_queue, O_WRONLY);
    if (client_queue == (mqd_t)-1) {
        perror("Error al abrir la cola del cliente");
        free(targ);
        return NULL;
    }

    if (mq_send(client_queue, (const char *)&res, sizeof(response_t), 0) == -1) {
        perror("Error al enviar respuesta al cliente");
    }

    mq_close(client_queue);
    free(targ);
    return NULL;
}

int main(void) {
    mqd_t server_queue;
    struct mq_attr attr;
    request_t req;
    ssize_t bytes_read;
    pthread_t thread_id;
    pthread_attr_t thread_attr;

    //Atributos del hilo
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(request_t);
    attr.mq_curmsgs = 0;

    mq_unlink(SERVER_QUEUE);

    server_queue = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0660, &attr);
    if (server_queue == (mqd_t)-1) {
        perror("Error al crear/abrir la cola del servidor");
        return 1;
    }

    printf("Servidor escuchando en %s...\n", SERVER_QUEUE);

    while (1) {
        bytes_read = mq_receive(server_queue, (char *)&req, sizeof(request_t), NULL);
        if (bytes_read == -1) {
            perror("Error al recibir mensaje");
            continue;
        }

        printf("Peticion recibida: operacion=%d, key=%s, client_queue=%s\n",
               req.operacion, req.key, req.client_queue);
        
        thread_arg_t *targ = malloc(sizeof(thread_arg_t));
        if (targ == NULL) {
            perror("Error al asignar memoria para el hilo");
            continue;
        }
        targ->req = req;

        if (pthread_create(&thread_id, &thread_attr, atender_peticion, targ) != 0) {
            perror("Error al crear hilo");
            free(targ);
            continue;
        }
    }

    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE);
    pthread_attr_destroy(&thread_attr);
    return 0;
}