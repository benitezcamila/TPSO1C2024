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


extern int socket_server;
int inicializar_memoria();
void escuchar_instrucciones();
void escucha_kernel();
void escucha_cpu();
void escucha_E_S();
int enviar_log(int, int);
#endif