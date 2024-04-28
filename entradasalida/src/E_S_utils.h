#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <utils/conexion.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <configuracion/config.h>
#include <pthread.h>

struct sockets{
    int socket_memoria;
    int socket_kernel;
};

extern struct sockets sockets;

void proceso_E_S();
void establecer_conexion_memoria();
void establecer_conexion_kernel();

#endif