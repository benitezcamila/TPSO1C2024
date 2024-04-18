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

extern int socket_server;

void establecer_conexion_cpu_D();

void establecer_conexion_memoria();

void establecer_conexion_cpu_I();

int inicializar_kernel();

void escucha_E_S();


#endif