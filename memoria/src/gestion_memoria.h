#ifndef GESTION_MEMORIA_H
#define GESTION_MEMORIA_H

#include <utils/conexion.h>
#include "configuracion/config.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <semaphore.h>
#include <utils/estructuras.h>
#include "memoria_utils.h"



 
typedef struct{
    int numero_marco;
    int pid_pagina;
    int tamDisp;
}t_pagina;


typedef struct 
{
    int pid;
    t_list* paginas;
}tabla_pagina;


t_list* tabla_global;
void* espacioUsuario;
int cantFrames;



extern int bitMap[];

void cargarEnTablaDePaginas(int);
int marcoDisponible();

void asignar_id_pagina(tabla_pagina*);
void reduccion_del_proceso(tabla_pagina*, int);
void ampliacion_del_proceso(tabla_pagina*, int); 

#endif