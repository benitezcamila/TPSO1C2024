#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_kernel;
t_log* logger_conexiones;
t_log* logger_recurso_ES;
t_log* logger_ingresos_ready;
t_log* logger_error;

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
    configuracion.RECURSOS = list_create();
    char** aux_recursos = config_get_array_value(config,"RECURSOS");
    for(int i = 0; aux_recursos[i] != NULL; i++){
        list_add(configuracion.RECURSOS, aux_recursos[i]);
    }
    configuracion.INSTANCIAS_RECURSOS = list_create();
    char** aux_instancias =config_get_array_value(config,"INSTANCIAS_RECURSOS");
    for(int i = 0; aux_instancias[i] != NULL; i++){
        list_add(configuracion.INSTANCIAS_RECURSOS, aux_instancias[i]);
    }
    configuracion.GRADO_MULTIPROGRAMACION = config_get_int_value(config,"GRADO_MULTIPROGRAMACION");

}

void iniciar_logger(){
    logger_kernel = log_create(PATH_ABSOLUTO("kernel/logs/kernel.log"),"Kernel",1,LOG_LEVEL_INFO);
    logger_conexiones = log_create(PATH_ABSOLUTO("kernel/logs/conexiones.log"),"Conexion",1,LOG_LEVEL_INFO);
    logger_recurso_ES = log_create(PATH_ABSOLUTO("kernel/logs/recursos_y_es.log"),"Recurso o E/S",1,LOG_LEVEL_INFO);
    logger_ingresos_ready = log_create(PATH_ABSOLUTO("kernel/logs/ingreso_ready.log"),"Ingreso a ready",1,LOG_LEVEL_INFO);
    logger_error = log_create(PATH_ABSOLUTO("kernel/logs/errores.log"),"Error",1,LOG_LEVEL_INFO);
}