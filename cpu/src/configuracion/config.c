#include "config.h"

void obtener_config(){
    config = config_create("cfg/cpu.config");    
    configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
    configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
    configuracion.PUERTO_ESCUCHA_DISPATCH = config_get_int_value(config,"PUERTO_ESCUCHA_DISPATCH");
    configuracion.PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(config,"PUERTO_ESCUCHA_INTERRUPT");
    configuracion.CANTIDAD_ENTRADAS_TLB = config_get_int_value(config,"CANTIDAD_ENTRADAS_TLB");
    configuracion.ALGORITMO_TLB = config_get_string_value(config,"ALGORITMO_TLB");
    
}

void iniciar_logger(){
    logger_cpu = log_create("cfg/cpu.log","cpu",0,LOG_LEVEL_INFO);
}