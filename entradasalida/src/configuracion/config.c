#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_entrada_salida;
t_log* logger_conexiones;
char * tipo_interfaz_string;

t_interfaz config_string_a_enum(char* str){
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

void obtener_config(char* path_config){
    config = config_create(path_config);
    tipo_interfaz_string = config_get_string_value(config,"TIPO_INTERFAZ");
    configuracion.TIPO_INTERFAZ = config_string_a_enum(tipo_interfaz_string);
    switch (configuracion.TIPO_INTERFAZ) {
        case GENERICA: 
            configuracion.TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
            configuracion.IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
            configuracion.PUERTO_KERNEL = config_get_int_value(config,"PUERTO_KERNEL");
            break;
        case STDIN:
            configuracion.IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
            configuracion.PUERTO_KERNEL = config_get_int_value(config,"PUERTO_KERNEL");
            configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
            configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
            break;
        case STDOUT:
            configuracion.IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
            configuracion.PUERTO_KERNEL = config_get_int_value(config,"PUERTO_KERNEL");
            configuracion.TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
            configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
            configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
            break;
        case DIALFS:
            configuracion.IP_KERNEL = config_get_string_value(config,"IP_KERNEL");
            configuracion.PUERTO_KERNEL = config_get_int_value(config,"PUERTO_KERNEL");
            configuracion.IP_MEMORIA = config_get_string_value(config,"IP_MEMORIA");
            configuracion.PUERTO_MEMORIA = config_get_int_value(config,"PUERTO_MEMORIA");
            configuracion.TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
            configuracion.PATH_BASE_DIALFS = config_get_string_value(config,"PATH_BASE_DIALFS");
            configuracion.BLOCK_SIZE = config_get_int_value(config,"BLOCK_SIZE");
            configuracion.BLOCK_COUNT = config_get_int_value(config,"BLOCK_COUNT"); 
            levantar_fs(configuracion.PATH_BASE_DIALFS,configuracion.BLOCK_SIZE,configuracion.BLOCK_COUNT);           
            break;
    }
}

void iniciar_logger(){
    logger_entrada_salida = log_create("/home/utnso/tp-2024-1c-Chiripiorca/entradasalida/logs/entrada salida.log","Entrada Salida",1,LOG_LEVEL_INFO);
    logger_conexiones = log_create("/home/utnso/tp-2024-1c-Chiripiorca/entradasalida/logs/conexiones.log","Conexion",1,LOG_LEVEL_INFO);
}

void levantar_fs(char* path_fs, uint8_t tamanio_bloques, uint8_t cantidad_bloques) {
    //leer archivos de bloques y bitmap, si no existen, crearlos
    //agregar /bloques.dat y /bitmap.dat al path a la hora de hacer el fopen
    //lock para que mas de 1 interfaz no utilice mismos archivos en simultaneo
    char* path_bloques = string_new();
    string_append(&path_bloques,path_fs);
    string_append(&path_bloques,"/bloques.dat");
    char* path_bitmap = string_new();
    string_append(&path_bitmap,path_fs);
    string_append(&path_bitmap,"/bitmap.dat");
    FILE* archivo_bloques = fopen(path_bloques,r+);
    FILE* archivo_bitmap = fopen(path_bitmap,r+);
    
    if(!archivo_bloques) { //si no existe el archivo aun, lo creo y corro ftruncate para inicializar bloques
        inicializar_fs();
    } else { 
        // si los archivos existen, leo
    }


}

void inicializar_fs(){
    //tener en cuenta tamaño bitmap, si cantidad de bloques/8 no da redondo, redondear para arriba
    return;
}