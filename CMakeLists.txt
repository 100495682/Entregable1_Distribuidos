cmake_minimum_required(VERSION 4.1)
project(Entrega1_Distribuidos C)

set(CMAKE_C_STANDARD 11)

add_executable(Entrega1_Distribuidos
        claves.h
        storage.h
        storage.c
        claves.c
        app_cliente1.c
        mensajes.h
        servidor-mq.c
        proxy-mq.c
        app_cliente2.c
        app_cliente4.c
        app_cliente3.c)
