#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <bits/pthreadtypes.h>
#include <utils/conexion.h>
#include <utils/utils.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include "configuracion/config.h"
#include <pthread.h>

void levantar_conexiones_cpu();
void levantar_conexiones_kernel();

#endif