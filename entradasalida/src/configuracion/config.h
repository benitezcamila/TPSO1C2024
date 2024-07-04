#ifndef _ENTRADASALIDA_CONFIG_
#define _ENTRADASALIDA_CONFIG_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <commons/log.h>
#include <stdint.h>
#include <string.h>
#include "utils/conexion.h"
#include "utils/serializacion.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


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
extern t_bitarray* bitmap;
extern int tamanio_bitmap_bytes;
extern int tamanio_memoria_bloques;
extern int fd_bitmap;
extern void* bitmap_memoria;
extern int fd_bloques;
extern void ** bloques;
extern char** indice;



void iniciar_logger();
void obtener_config(char* path_config);
t_bitarray* crear_bitmap(char* path_bitmap, uint32_t cantidad_bloques );
void* crear_bloques(char* path_bloques, uint32_t tamanio_bloques, uint32_t cantidad_bloques );
t_bitarray* mapear_archivo_bitmap (int fd_bitmap, int tamanio_bitmap_bytes);
void* mapear_archivo_bloques(int fd_bloques, uint32_t tamanio_bloques, uint32_t cantidad_bloques);
char** crear_indice(char* path_indice, uint32_t cantidad_bloques);


#endif