#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_entrada_salida;

void obtener_config(){
    config = config_create("cfg/entrada_salida.config");
    configuracion.TIPO_INTERFAZ= config_get_string_value(config,"TIPO_INTERFAZ");
    configuracion.TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
    configuracion.IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
    configuracion.PUERTO_KERNEL = config_get_int_value(config,"PUERTO_KERNEL");
    configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
    configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
    configuracion.PATH_BASE_DIALFS = config_get_string_value(config,"PATH_BASE_DIALFS");
    configuracion.BLOCK_SIZE = config_get_int_value(config,"BLOCK_SIZE");
    configuracion.BLOCK_COUNT = config_get_int_value(config,"BLOCK_COUNT");

}

void iniciar_logger(){
    logger_entrada_salida = log_create("cfg/entrada salida.log","Entrada Salida",0,LOG_LEVEL_INFO);
}