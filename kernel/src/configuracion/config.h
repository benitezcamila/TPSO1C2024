#ifndef _KERNEL_CONFIG_
#define _KERNEL_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdint.h>


typedef struct {
    char* IP_MEMORIA;
    char* IP_CPU;
    uint16_t PUERTO_MEMORIA;
    uint16_t PUERTO_ESCUCHA;
    uint16_t PUERTO_CPU_DISPATCH;
    uint16_t PUERTO_CPU_INTERRUPT;
    char* ALGORITMO_PLANIFICACION;
    int QUANTUM;
    t_list* RECURSOS;
    t_list* INSTANCIAS_RECURSOS;
    uint8_t GRADO_MULTIPROGRAMACION;

}t_cfg;

t_cfg configuracion;

t_config* config;

void obtener_config();

t_log* logger_kernel;


void iniciar_logger();

#endif