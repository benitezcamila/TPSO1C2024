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
void establecer_conexion_memoria();
void establecer_conexion_kernel();

#endif