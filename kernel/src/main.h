#ifndef MAIN_H_
#define MAIN_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "kernel_utils.h"
#include "pcb.h"
#include "recursos.h"

extern pthread_t levantar_kernel, escuchar_conexiones, establecer_conexion, planificador_corto_plazo, planificador_largo_plazo;

#endif