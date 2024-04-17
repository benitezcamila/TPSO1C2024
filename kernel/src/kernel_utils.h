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


void conexion_cpu_D();

void conexion_memoria();

void conexion_cpu_I();

#endif