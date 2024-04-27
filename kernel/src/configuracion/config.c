#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_kernel;
t_log* logger_conexiones;

void obtener_config(){
    config = config_create("cfg/kernel.config");
    configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
    configuracion.IP_CPU = config_get_string_value(config,"IP_CPU");
    configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
    configuracion.PUERTO_ESCUCHA = config_get_int_value(config,"PUERTO_ESCUCHA");
    configuracion.PUERTO_CPU_DISPATCH = config_get_int_value(config,"PUERTO_CPU_DISPATCH");
    configuracion.PUERTO_CPU_INTERRUPT = config_get_int_value(config,"PUERTO_CPU_INTERRUPT");
    configuracion.ALGORITMO_PLANIFICACION = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
    configuracion.QUANTUM = config_get_int_value(config,"QUANTUM");
    configuracion.RECURSOS =config_get_array_value(config,"RECURSOS");
    configuracion.INSTANCIAS_RECURSOS = config_get_array_value(config,"INSTANCIAS_RECURSOS");
    configuracion.GRADO_MULTIPROGRAMACION = config_get_int_value(config,"GRADO_MULTIPROGRAMACION");

}

void iniciar_logger(){
    logger_kernel = log_create("logs/kernel.log","Kernel",1,LOG_LEVEL_INFO);
    logger_conexiones = log_create("logs/conexiones.log","Conexion",1,LOG_LEVEL_INFO);
}