#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

#include <utils/conexion.h>
#include "configuracion/config.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct {
    int socket_server;
    int socket_cliente_CPU;
    int socket_cliente_kernel;
    int socket_cliente_E_S;
}str_sockets;

extern char* path_kernel;

extern str_sockets sockets;

extern int socket_server;
void inicializar_memoria();
void escuchar_instrucciones();
int server_escuchar(int);
void procesar_conexion(void*);
int enviar_log(int, int);
#endif