#ifndef _ENTRADASALIDA_CONFIG_
#define _ENTRADASALIDA_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdint.h>
#include <string.h>
#include "utils/conexion.h"
#include "utils/serializacion.h"


typedef struct {
    t_interfaz TIPO_INTERFAZ;
    uint16_t TIEMPO_UNIDAD_TRABAJO;
    char* IP_KERNEL;
    uint16_t PUERTO_KERNEL;
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    char* PATH_BASE_DIALFS;
    uint8_t BLOCK_SIZE;
    uint16_t BLOCK_COUNT;
}t_cfg;

extern t_cfg configuracion;

extern t_config* config;


extern t_log* logger_entrada_salida;
extern t_log* logger_conexiones;

void iniciar_logger();
void obtener_config(char* path_config);



#endif