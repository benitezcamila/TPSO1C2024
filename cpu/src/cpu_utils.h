#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include "utils/conexion.h"
#include "utils/serializacion.h"
#include <configuracion/config.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <semaphore.h>
#include <ciclo_de_instruccion.h>
#include <utils/estructuras.h>

typedef struct{
    int socket_server_I;
    int socket_server_D;
    int socket_memoria;
}str_sockets;

extern int ind_contexto_kernel;
extern int llego_interrupcion;
extern str_sockets sockets;

void iniciar_server_kernel();
void inicializar_cpu_dispatch();
void inicializar_cpu_interrupt();
void establecer_conexion_memoria();
void atender_conexiones();
void procesar_conexion(void*);
int server_escuchar(int);
void recibir_contexto_ejecucion();
void recibir_interrupcion_de_kernel();
void enviar_contexto_a_kernel(motivo_desalojo);
void enviar_std_a_kernel(motivo_desalojo, char*, void*, uint32_t, uint32_t);
void solicitar_instruccion_a_memoria();
int solicitar_marco_a_memoria(uint32_t );
void recibir_instruccion_de_memoria();
void solicitar_leer_en_memoria(uint32_t, uint32_t);
void* leer_de_memoria(uint32_t);
void solicitar_escribir_en_memoria(uint32_t, void*, uint32_t);

#endif