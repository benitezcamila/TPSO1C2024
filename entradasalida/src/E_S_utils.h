#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <utils/conexion.h>
#include <utils/utils.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <configuracion/config.h>
#include <pthread.h>

void proceso_E_S();
void levantar_conexiones_memoria();
void levantar_conexiones_kernel();

#endif