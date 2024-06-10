#ifndef RECURSOS_H
#define RECURSOS_H

#include "kernel_utils.h"
#include "planificacion.h"
#include <commons/collections/dictionary.h>

typedef struct{
    t_queue* cola;
    sem_t cantidad_recursos;
}str_recursos;

extern int cantidad_recursos;
extern t_dictionary* recursos;

void crear_colas_bloqueo()
void gestionar_recurso(str_recursos*);
#endif