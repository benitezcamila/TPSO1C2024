#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <configuracion/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include "utils/conexion.h"
#include "utils/serializacion.h"


typedef struct {

    int socket_server;
    int socket_CPU_I;
    int socket_CPU_D;
    int socket_memoria;
    int socket_cliente_E_S;
}str_sockets;

extern str_sockets sockets;

extern pthread_t conexion_CPU_I,conexion_CPU_D, conexion_memoria;

void establecer_conexion_cpu_D();

void establecer_conexion_memoria();

void establecer_conexion_cpu_I();

void inicializar_kernel();

void escucha_E_S();

void establecer_conexiones();


#endif