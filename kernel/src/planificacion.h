#ifndef PLANIFICACION_H
#define PLANIFICACION_H

#include <commons/string.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include "kernel_utils.h"

extern sem_t sem_grado_multiprogramacion;
extern sem_t proceso_ejecutando;
extern t_queue *cola_new;
extern t_queue *cola_ready;
extern t_queue *cola_prioritaria_VRR;
extern t_queue *bloqueado;
extern t_queue *suspendido_bloqueado;
extern t_queue *suspendido_listo; 
extern t_queue *cola_finalizados;

#endif



