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


 typedef struct{
    int numero_marco;
    int pid_pagina;
    int tam_disponible;
    bool escrita;
    int offSet;
}t_pagina;


typedef struct 
{
    int pid;
    t_list* paginas;
}tabla_pagina;


extern t_dictionary* tabla_global;
extern void* espacio_usuario;
extern int cantFrames;



extern int bitMap[];

void cargarEnTablaDePaginas(int);
int marcoDisponible();

void asignar_id_pagina(tabla_pagina*);
void reduccion_del_proceso(tabla_pagina*, int);
void ampliacion_del_proceso(tabla_pagina*, int); 
void* access_espacio_usuario(t_buffer*);
t_paquete* leer_espacio_usuario(uint32_t ,uint32_t , uint32_t );
bool existe_ese_marco(uint32_t, t_pagina* );
void* leer_memoria(uint32_t , uint32_t );
void* escribir_espacio_usuario(uint32_t ,uint32_t,uint32_t,t_buffer*);
bool hay_lugar_contiguo();
void escribir_memoria(t_list*, t_pagina*,uint32_t,uint32_t, void*);

t_paquete* buscar_marco_pagina (t_buffer*);
#endif