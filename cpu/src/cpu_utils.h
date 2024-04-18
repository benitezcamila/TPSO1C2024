#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <utils/conexion.h>
#include <utils/utils.h>
#include <configuracion/config.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

extern int socket_server_dispatch;
extern int socket_server_interrupt;

int inicializar_cpu_dispatch();
int inicializar_cpu_interrupt();
void establecer_conexion_memoria();
void escucha_KD();
void escucha_KI();
int enviar_log_D();
int enviar_log_I();

#endif