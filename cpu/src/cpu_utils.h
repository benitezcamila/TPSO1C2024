#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include "utils/conexion.h"
#include "utils/serializacion.h"
#include <configuracion/config.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct{
    int socket_server_I;
    int socket_server_D;
    int socket_memoria;
} str_sockets;

extern int ind_contexto_kernel
extern str_sockets sockets;
void iniciar_server_kernel();
void inicializar_cpu_dispatch();
void inicializar_cpu_interrupt();
void establecer_conexion_memoria();
void atender_conexiones();
void procesar_conexion(void*);
int server_escuchar(int);
int enviar_log_D();
int enviar_log_I();
void recibir_contexto_ejecucion();
void solicitar_instruccion_a_memoria();
void recibir_instruccion_de_memoria();
void enviar_contexto_a_kernel();

#endif