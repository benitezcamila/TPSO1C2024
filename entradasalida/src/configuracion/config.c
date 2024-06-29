#include "config.h"

t_cfg configuracion;
t_config* config;
t_log* logger_entrada_salida;
t_log* logger_conexiones;
char * tipo_interfaz_string;
t_bitarray* bitmap = NULL;
int tamanio_bitmap_bytes = 0;
int fd_bitmap;
void* bitmap_memoria = NULL;

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

void levantar_fs(char* path_fs, uint8_t tamanio_bloques, uint32_t cantidad_bloques) {
    //leer archivos de bloques y bitmap, si no existen, crearlos
    //agregar /bloques.dat y /bitmap.dat al path a la hora de hacer el fopen
    //lock para que mas de 1 interfaz no utilice mismos archivos en simultaneo
    char* path_bloques = string_new();
    string_append(&path_bloques,path_fs);
    string_append(&path_bloques,"/bloques.dat");
    char* path_bitmap = string_new();
    string_append(&path_bitmap,path_fs);
    string_append(&path_bitmap,"/bitmap.dat");
    FILE* archivo_bloques = fopen(path_bloques,"r+");
    FILE* archivo_bitmap = fopen(path_bitmap,"r+");
    
    if(!archivo_bloques) { //si no existe el archivo aun, lo creo y corro ftruncate para inicializar bloques
        inicializar_fs(path_bloques, path_bitmap,  tamanio_bloques,  cantidad_bloques);
    } else { 
        // si los archivos existen, leo
    }


}

void inicializar_fs(char * path_bloques, char* path_bitmap, uint8_t tamanio_bloques, uint32_t cantidad_bloques ){
    crear_bloques(path_bloques,tamanio_bloques,cantidad_bloques);
    bitmap = crear_bitmap(path_bitmap,tamanio_bloques,cantidad_bloques);
    //algo_aca=mmap(NULL,tamanio_memoria_bloques,PROT_WRITE,MAP_SHARED,aca va el fd del archivo de bloques,0)
}

void crear_bloques(char* path_bloques, uint8_t tamanio_bloques, uint32_t cantidad_bloques ){
     int tamanio_memoria_bloques = tamanio_bloques * cantidad_bloques;
}

t_bitarray* crear_bitmap(char* path_bitmap, uint8_t tamanio_bloques, uint32_t cantidad_bloques ){
    //tengo en cuenta tamaño bitmap, si cantidad de bloques/8 no da redondo, redondear para arriba
    if(cantidad_bloques%8 !=0) {
        tamanio_bitmap_bytes = (cantidad_bloques/8 )+1;
    } else {
        tamanio_bitmap_bytes = cantidad_bloques/8;
    }
    //crea file descriptor a archivo generado bitmap.dat
    fd_bitmap = open(path_bitmap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd_bitmap == -1) {
        log_info(logger_entrada_salida, "Fallo la creacion del archivo bitmap");
        exit(EXIT_FAILURE);
    }
    //defino tamaño con ftruncate
    if (ftruncate(fd_bitmap, tamanio_bitmap_bytes) == -1) {
        log_info(logger_entrada_salida, "Error al truncar el archivo de bitmap");
        close(fd_bitmap);
        exit(EXIT_FAILURE);
    }
    // Mapear el archivo en memoria
    //primero genero el bitarray despues lo mapeo?
    bitmap_memoria = mmap(NULL, tamanio_bitmap_bytes,PROT_WRITE, MAP_SHARED, fd_bitmap, 0);
    if (bitmap_memoria == MAP_FAILED) {
        log_info(logger_entrada_salida, "Fallo el mmap");
        close(fd_bitmap);
        return NULL;
    }
    //prueba cantidad de bloques = 1024
    //prueba tamaño de bloques = 64
    //el tamaño del bitmap deberia ser de 1024 bits o 128 bytes
    bitmap = bitarray_create_with_mode(bitmap_memoria,tamanio_bitmap_bytes,LSB_FIRST);
    if (close(fd_bitmap) == -1) {
        log_info(logger_entrada_salida, "Error al cerrar el archivo bitmap.dat");
        exit(EXIT_FAILURE);
    }
    return bitmap;
}