#ifndef _CPU_CONFIG_H
#define _CPU_CONFIG_H

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

extern t_cfg configuracion;

extern t_config* config;

void obtener_config();

extern t_log* logger_cpu;
extern t_log* logger_conexiones;

void iniciar_logger();

#endif