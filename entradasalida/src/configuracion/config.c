#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_entrada_salida;
t_log* logger_conexiones;

/*
tipo_interfaz config_string_a_enum(char* str){
    if (strcmp(str,"GENERICA")== 0 ){
        return GENERICA;
    }
    else if (strcmp(str,"STDIN")== 0 ){
        return STDIN;
    }
    else if (strcmp(str,"STDOUT")== 0 ){
        return STDOUT;
    }
    else if (strcmp(str,"DIALFS")== 0 ){
        return DIALFS;
    }
}
*/ //Lo comente para compilar

void obtener_config(){
    char* str_tipo_interfaz;
    config = config_create("cfg/entradasalida.config");
    str_tipo_interfaz = config_get_string_value(config,"TIPO_INTERFAZ");
    //configuracion.TIPO_INTERFAZ = config_string_a_enum(str_tipo_interfaz); tambien para compilar
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
    logger_entrada_salida = log_create("logs/entrada salida.log","Entrada Salida",1,LOG_LEVEL_INFO);
    logger_conexiones = log_create("logs/conexiones.log","Conexion",1,LOG_LEVEL_INFO);
}