#ifndef PCB_H
#define PCB_H

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <stdint.h>
#include <stdlib.h>
#include <configuracion/config.h>
#include <utils/conexion.h>
#include <utils/serializacion.h>
#include "kernel_utils.h"



extern t_dictionary* dicc_pcb;
extern int current_pid;
extern registros_CPU* registros;

t_pcb* crear_pcb();
registros_CPU* crear_registros();
int siguiente_PID();
t_buffer* crear_buffer_pcb();


#endif