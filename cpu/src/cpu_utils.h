#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <utils/conexion.h>
#include <configuracion/config.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

struct sockets{

    int socket_server_I;
    int socket_server_D;
    int socket_memoria;
    int socket_cliente_KI;
    int socket_cliente_KD;
};

extern struct sockets sockets;
void iniciar_server_kernel();
void inicializar_cpu_dispatch();
void inicializar_cpu_interrupt();
void establecer_conexion_memoria();
void atender_conexiones();
void procesar_conexion(void*);
int server_escuchar(int);
int enviar_log_D();
int enviar_log_I();

#endif