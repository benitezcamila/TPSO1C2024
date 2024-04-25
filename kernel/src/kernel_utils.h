#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <utils/conexion.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <utils/utils.h>
#include <configuracion/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <pcb.h>

struct sockets{

    int socket_server;
    int socket_CPU_I;
    int socket_CPU_D;
    int socket_memoria;
    int socket_cliente_E_S;
};

extern struct sockets sockets;

extern pthread_t conexion_CPU_I,conexion_CPU_D, conexion_memoria;

void establecer_conexion_cpu_D();

void establecer_conexion_memoria();

void establecer_conexion_cpu_I();

void inicializar_kernel();

void escucha_E_S();

void establecer_conexiones();


#endif