#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <utils/conexion.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/temporal.h>
#include <unistd.h>
#include <configuracion/config.h>
#include <pthread.h>
#include "utils/conexion.h"
#include "utils/serializacion.h"
#include <semaphore.h>
typedef struct {
    int socket_memoria;
    int socket_kernel;
}str_sockets;

extern str_sockets sockets;
extern sem_t mutex_conexion;

//void proceso_E_S();
void establecer_conexiones();
void establecer_conexion_memoria(int*);
void establecer_conexion_kernel(int*);
//void iniciar_interfaz();


void enviar_info_io_a_kernel(int);

void procesar_io_gen_sleep (t_buffer*, int, char*);
void procesar_io_stdin_read(t_buffer* , uint32_t,int,int, char*);
void procesar_io_stdout_write(t_buffer* , uint32_t,int, int, char* );
void procesar_io_fs_create(t_buffer*, uint32_t, int, char*);
void procesar_io_fs_delete(t_buffer*, uint32_t,int, char*);
void procesar_io_fs_truncate(t_buffer* , uint32_t,int, char*);
void procesar_io_fs_write(t_buffer*, uint32_t , int , int,char*);
void procesar_io_fs_read(t_buffer* ,uint32_t,int,int, char*);

char* leer_consola();
void escribir_en_memoria(char* ,uint32_t ,uint32_t , int) ;
void recibir_instrucciones (int,int);
char* string_de_instruccion (t_instruccion);
int contar_bloques_libres(t_bitarray*);
int primer_bloque_libre(t_bitarray*);
void limpiar_bits(t_bitarray* , off_t , off_t);
bool hay_bits_ocupados(t_bitarray* , int , int );
void enviar_fin_de_instruccion(int, char*);
#endif