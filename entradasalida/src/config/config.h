#ifndef _ENTRADASALIDA_CONFIG_
#define _ENTRADASALIDA_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdint.h>

typedef struct {
    char* TIPO_INTERFAZ;
    uint16_t TIEMPO_UNIDAD_TRABAJO;
    char* IP_KERNEL;
    uint16_t PUERTO_KERNEL;
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    char* PATH_BASE_DIALFS;
    uint8_t BLOCK_SIZE;
    uint16_t BLOCK_COUNT;



}t_cfg;

t_cfg configuracion;

t_config* config;

void obtener_config();

t_log* logger_entrada_salida;


void iniciar_logger();

#endif