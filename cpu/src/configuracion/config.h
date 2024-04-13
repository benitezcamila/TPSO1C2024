#ifndef _CPU_CONFIG_
#define _CPU_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <utils/conexion.h>



typedef struct {
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    uint16_t PUERTO_ESCUCHA_DISPATCH;
    uint16_t PUERTO_ESCUCHA_INTERRUPT;
    uint8_t CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;

}t_cfg;

t_cfg configuracion;

t_config* config;

void obtener_config();

t_log* logger_cpu;


void iniciar_logger();

#endif