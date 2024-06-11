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
#include "memoria_utils.h"
#include <commons/collections/dictionary.h>


 typedef struct
 {
    int numero_marco;
    int pid_pagina;
    int tam_disponible;
    bool escrita;
}t_pagina;


typedef struct 
{
    int pid;
    t_list* paginas;
}tabla_pagina;


extern t_dictionary* tabla_global;
extern void* espacioUsuario;
extern int cantFrames;



extern int bitMap[];

void cargarEnTablaDePaginas(int);
int marcoDisponible();

void asignar_id_pagina(tabla_pagina*);
void reduccion_del_proceso(tabla_pagina*, int);
void ampliacion_del_proceso(tabla_pagina*, int); 
void access_espacio_usuario(t_buffer*);
#endif