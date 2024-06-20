#ifndef CONSOLA_H
#define CONSOLA_H

#include "kernel_utils.h"
#include "planificacion.h"

extern char* mensaje_listado;

void ejecutar_consola_kernel();
void ejecutar_script(const char *path);
void iniciar_proceso(const char *path);
void finalizar_proceso(char* pid);
void detener_planificacion();
void iniciar_planificacion();
void modificar_multiprogramacion(int valor);
void listar_procesos_por_estado();
void listar_proceso(t_list*, char*);
int get_tipo_comando(const char *input);





typedef enum {
    EJECUTAR_SCRIPT,
    INICIAR_PROCESO_CONSOLA,
    FINALIZAR_PROCESO_CONSOLA,
    DETENER_PLANIFICACION,
    INICIAR_PLANIFICACION,
    MULTIPROGRAMACION,
    PROCESO_ESTADO,
    COMANDO_DESCONOCIDO
}tipo_comando ;

#endif