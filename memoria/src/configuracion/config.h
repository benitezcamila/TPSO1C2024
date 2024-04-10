#ifndef _KERNEL_CONFIG_
#define _KERNEL_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>


typedef struct {
    uint16_t PUERTO_ESCUCHA;
    uint16_t TAM_MEMORIA;
    uint16_t TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    uint16_t RETARDO_RESPUESTA;

}t_cfg;

t_cfg configuracion;

t_config* config;

void obtener_config();

t_log* logger_memoria;


void iniciar_logger();

#endif