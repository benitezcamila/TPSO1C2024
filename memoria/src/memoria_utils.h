#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

#include <utils/conexion.h>
#include <utils/utils.h>
#include "configuracion/config.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

struct sockets{
    int socket_server;
    int socket_cliente_CPU;
    int socket_cliente_kernel;
    int socket_cliente_E_S;
};

extern char* path_kernel;





extern struct sockets sockets;




extern int socket_server;
void inicializar_memoria();
void escuchar_instrucciones();
void escucha_kernel();
void escucha_cpu();
void escucha_E_S();
int enviar_log(int, int);
#endif